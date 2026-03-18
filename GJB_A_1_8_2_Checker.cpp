#include "clang/AST/Decl.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"

using namespace clang;
using namespace ento;

namespace {
class GJB_A_1_8_2_Visitor : public RecursiveASTVisitor<GJB_A_1_8_2_Visitor> {
public:
  explicit GJB_A_1_8_2_Visitor(BugReporter &BR, AnalysisManager &Mgr,
                               const CheckerBase *Checker, const Decl *D)
      : BR(BR), Mgr(Mgr), Checker(Checker), CurrentDecl(D) {}

  bool VisitVarDecl(VarDecl *VD) {
    if (!VD->isLocalVarDecl() && !VD->isFileVarDecl())
      return true;
    if (VD->isExternC() || VD->hasExternalStorage() || VD->isStaticLocal())
      return true;
    if (VD->isUsed() || VD->isReferenced())
      return true;

    SourceLocation Loc = VD->getLocation();
    if (Loc.isInvalid() || Mgr.getSourceManager().isInSystemHeader(Loc))
      return true;

    PathDiagnosticLocation DiagLoc(Loc, BR.getSourceManager());
    BR.EmitBasicReport(
        CurrentDecl, Checker, "Unused variable", "GJB8114 Rule A-1-8-2",
        "Variable '" + VD->getNameAsString() + "' is never used.", DiagLoc,
        VD->getSourceRange());
    return true;
  }

private:
  BugReporter &BR;
  AnalysisManager &Mgr;
  const CheckerBase *Checker;
  const Decl *CurrentDecl;
};

class GJB_A_1_8_2_Checker : public Checker<check::ASTCodeBody> {
public:
  void checkASTCodeBody(const Decl *D, AnalysisManager &Mgr,
                        BugReporter &BR) const {
    GJB_A_1_8_2_Visitor Visitor(BR, Mgr, this, D);
    Visitor.TraverseDecl(const_cast<Decl *>(D));
  }
};
} // namespace

namespace clang {
namespace ento {
void registerGJB_A_1_8_2(CheckerManager &mgr) {
  mgr.registerChecker<GJB_A_1_8_2_Checker>();
}
bool shouldRegisterGJB_A_1_8_2(const CheckerManager &mgr) { return true; }
} // namespace ento
} // namespace clang