// 规则 A-1-12-1：避免使用未初始化的指针
// ========== 头文件包含顺序很重要 ==========
#include "clang/AST/ASTContext.h" // ← 添加：必须在 Type.h 之前
#include "clang/AST/Decl.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Type.h" // ← Type.h 放在 ASTContext.h 之后
#include "clang/Analysis/PathDiagnostic.h"
#include "clang/Basic/SourceManager.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"

using namespace clang;
using namespace ento;

namespace {
class GJB_A_1_12_1_Visitor : public RecursiveASTVisitor<GJB_A_1_12_1_Visitor> {
public:
  explicit GJB_A_1_12_1_Visitor(BugReporter &BR, AnalysisManager &Mgr,
                                const CheckerBase *Checker)
      : BR(BR), Mgr(Mgr), Checker(Checker) {}

  bool VisitVarDecl(VarDecl *VD) {
    if (!VD->isLocalVarDecl())
      return true;

    // 检查是否为指针类型且未初始化
    if (VD->getType()->isPointerType() && !VD->hasInit()) {
      SourceLocation Loc = VD->getLocation();
      if (Loc.isInvalid() || Mgr.getSourceManager().isInSystemHeader(Loc))
        return true;

      PathDiagnosticLocation PDLoc(Loc, Mgr.getSourceManager());

      BR.EmitBasicReport(
          VD, Checker, "Uninitialized pointer", "GJB8114 Rule A-1-12-1",
          "Pointer '" + VD->getNameAsString() + "' is not initialized.", PDLoc);
    }
    return true;
  }

private:
  BugReporter &BR;
  AnalysisManager &Mgr;
  const CheckerBase *Checker;
};

class GJB_A_1_12_1_Checker : public Checker<check::ASTCodeBody> {
public:
  void checkASTCodeBody(const Decl *D, AnalysisManager &Mgr,
                        BugReporter &BR) const {
    GJB_A_1_12_1_Visitor Visitor(BR, Mgr, this);
    Visitor.TraverseDecl(const_cast<Decl *>(D));
  }
};
} // namespace

namespace clang {
namespace ento {
void registerGJB_A_1_12_1(CheckerManager &mgr) {
  mgr.registerChecker<GJB_A_1_12_1_Checker>();
}

bool shouldRegisterGJB_A_1_12_1(const CheckerManager &mgr) { return true; }
} // namespace ento
} // namespace clang