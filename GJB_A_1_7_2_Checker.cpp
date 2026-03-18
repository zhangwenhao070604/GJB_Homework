// 规则 A-1-7-2：函数中避免存在未被使用的调用参数
#include "clang/AST/Decl.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Analysis/PathDiagnostic.h"
#include "clang/Basic/SourceManager.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"

using namespace clang;
using namespace ento;

namespace {
class GJB_A_1_7_2_Visitor : public RecursiveASTVisitor<GJB_A_1_7_2_Visitor> {
public:
  // 修复：添加 Checker 指针参数
  explicit GJB_A_1_7_2_Visitor(BugReporter &BR, AnalysisManager &Mgr,
                               const CheckerBase *Checker)
      : BR(BR), Mgr(Mgr), Checker(Checker) {}

  bool VisitFunctionDecl(FunctionDecl *FD) {
    // 跳过没有函数体的声明
    if (!FD->hasBody())
      return true;

    // 收集函数体内所有引用过的 ParmVarDecl
    class ParmRefCollector : public RecursiveASTVisitor<ParmRefCollector> {
    public:
      bool VisitDeclRefExpr(DeclRefExpr *DRE) {
        if (auto *PVD = dyn_cast<ParmVarDecl>(DRE->getDecl())) {
          UsedParms.insert(PVD);
        }
        return true;
      }
      llvm::SmallPtrSet<const ParmVarDecl *, 8> UsedParms;
    } collector;
    collector.TraverseDecl(FD);

    // 检查每个参数是否被使用
    for (auto *PVD : FD->parameters()) {
      if (!PVD)
        continue;
      if (PVD->isUsed() || collector.UsedParms.count(PVD))
        continue;

      SourceLocation Loc = PVD->getLocation();
      if (Loc.isInvalid() || Mgr.getSourceManager().isInSystemHeader(Loc))
        continue;

      // 修复：正确创建 PathDiagnosticLocation
      PathDiagnosticLocation PDLoc(Loc, Mgr.getSourceManager());

      // 修复：正确构造 ArrayRef<SourceRange>
      SourceRange Range = PVD->getSourceRange();
      ArrayRef<SourceRange> Ranges = Range;

      // 修复：正确的参数顺序和类型
      BR.EmitBasicReport(FD,      // const Decl *DeclWithIssue
                         Checker, // const CheckerBase * (不是 &Checker)
                         "Unused parameter",     // StringRef BugName
                         "GJB8114 Rule A-1-7-2", // StringRef BugCategory
                         "Parameter '" + PVD->getNameAsString() +
                             "' is not used in function.", // StringRef BugStr
                         PDLoc // PathDiagnosticLocation Loc
                               // Ranges 和 Fixits 使用默认值，或显式传递
                               // , Ranges
                               // , {}
      );
    }
    return true;
  }

private:
  BugReporter &BR;
  AnalysisManager &Mgr;
  const CheckerBase *Checker; // 现在会被正确初始化
};

class GJB_A_1_7_2_Checker : public Checker<check::ASTCodeBody> {
public:
  void checkASTCodeBody(const Decl *D, AnalysisManager &Mgr,
                        BugReporter &BR) const {
    // 修复：传递 this 作为 Checker 指针
    GJB_A_1_7_2_Visitor Visitor(BR, Mgr, this);
    Visitor.TraverseDecl(const_cast<Decl *>(D));
  }
};
} // namespace

namespace clang {
namespace ento {
void registerGJB_A_1_7_2(CheckerManager &mgr) {
  mgr.registerChecker<GJB_A_1_7_2_Checker>();
}
bool shouldRegisterGJB_A_1_7_2(const CheckerManager &mgr) { return true; }
} // namespace ento
} // namespace clang