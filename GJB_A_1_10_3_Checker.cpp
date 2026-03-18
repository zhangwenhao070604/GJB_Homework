// 规则 A-1-10-3：变量使用前必须初始化
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
class GJB_A_1_10_3_Visitor : public RecursiveASTVisitor<GJB_A_1_10_3_Visitor> {
public:
  explicit GJB_A_1_10_3_Visitor(BugReporter &BR, AnalysisManager &Mgr,
                                const CheckerBase *Checker)
      : BR(BR), Mgr(Mgr), Checker(Checker) {}

  bool VisitVarDecl(VarDecl *VD) {
    if (!VD->isLocalVarDecl())
      return true;
    if (VD->hasInit() || VD->isStaticLocal() || VD->hasExternalStorage())
      return true;

    SourceLocation Loc = VD->getLocation();
    if (Loc.isInvalid() || Mgr.getSourceManager().isInSystemHeader(Loc))
      return true;

    std::string Desc = "Variable '" + VD->getNameAsString() +
                       "' should be initialized before use.";

    // ========== 修复：使用基础 API 创建 PathDiagnosticLocation ==========

    // 【方案 1】使用 PathDiagnosticLocation 构造函数（最兼容）
    PathDiagnosticLocation PDLoc(Loc, Mgr.getSourceManager());

    // 【方案 2】使用 createBegin 的 Stmt 版本（如果方案 1 不行）
    // PathDiagnosticLocation PDLoc =
    //     PathDiagnosticLocation::createBegin(VD->getBeginLoc(),
    //     Mgr.getSourceManager());

    // 【方案 3】使用 create（如果方案 1 不行）
    // PathDiagnosticLocation PDLoc =
    //     PathDiagnosticLocation::create(Loc, Mgr.getSourceManager());

    // =======================================================

    BR.EmitBasicReport(VD,                         // const Decl *DeclWithIssue
                       Checker,                    // const CheckerBase *Checker
                       "Variable not initialized", // StringRef BugName
                       "GJB8114 Rule A-1-10-3",    // StringRef BugCategory
                       Desc,                       // StringRef BugStr
                       PDLoc                       // PathDiagnosticLocation Loc
    );

    return true;
  }

private:
  BugReporter &BR;
  AnalysisManager &Mgr;
  const CheckerBase *Checker;
};

class GJB_A_1_10_3_Checker : public Checker<check::ASTCodeBody> {
public:
  void checkASTCodeBody(const Decl *D, AnalysisManager &Mgr,
                        BugReporter &BR) const {
    GJB_A_1_10_3_Visitor Visitor(BR, Mgr, this);
    Visitor.TraverseDecl(const_cast<Decl *>(D));
  }
};
} // namespace

namespace clang {
namespace ento {
void registerGJB_A_1_10_3(CheckerManager &mgr) {
  mgr.registerChecker<GJB_A_1_10_3_Checker>();
}
bool shouldRegisterGJB_A_1_10_3(const CheckerManager &mgr) { return true; }
} // namespace ento
} // namespace clang