// 规则 A-1-11-1：建议变量在声明的同时进行初始化
#include "clang/AST/Decl.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"
#include "llvm/ADT/StringRef.h"

using namespace clang;
using namespace ento;

namespace {
class GJB_A_1_11_1_Visitor : public RecursiveASTVisitor<GJB_A_1_11_1_Visitor> {
public:
  explicit GJB_A_1_11_1_Visitor(BugReporter &BR, AnalysisManager &Mgr,
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

    // 修复：使用 StringRef，调整参数顺序
    std::string Desc = "Variable '" + VD->getNameAsString() +
                       "' should be initialized at declaration.";

    BR.EmitBasicReport(
        VD, Checker,
        "GJB8114 Rule A-1-11-1",    // BugCategory
        "Variable not initialized", // BugType
        Desc, PathDiagnosticLocation::createBegin(VD, BR.getSourceManager()));
    return true;
  }

private:
  BugReporter &BR;
  AnalysisManager &Mgr;
  const CheckerBase *Checker;
};

class GJB_A_1_11_1_Checker : public Checker<check::ASTCodeBody> {
public:
  void checkASTCodeBody(const Decl *D, AnalysisManager &Mgr,
                        BugReporter &BR) const {
    GJB_A_1_11_1_Visitor Visitor(BR, Mgr, this);
    Visitor.TraverseDecl(const_cast<Decl *>(D));
  }
};
} // namespace

namespace clang {
namespace ento {
void registerGJB_A_1_11_1(CheckerManager &mgr) {
  mgr.registerChecker<GJB_A_1_11_1_Checker>();
}
bool shouldRegisterGJB_A_1_11_1(const CheckerManager &mgr) { return true; }
} // namespace ento
} // namespace clang