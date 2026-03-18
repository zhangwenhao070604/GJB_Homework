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
class GJB_A_2_7_2_Visitor : public RecursiveASTVisitor<GJB_A_2_7_2_Visitor> {
public:
  explicit GJB_A_2_7_2_Visitor(BugReporter &BR, AnalysisManager &Mgr,
                               const CheckerBase *Checker, const Decl *D)
      : BR(BR), Mgr(Mgr), Checker(Checker), CurrentDecl(D) {}

  bool VisitFunctionDecl(FunctionDecl *FD) {
    for (auto *PVD : FD->parameters()) {
      if (!PVD || (!PVD->getType()->isPointerType() &&
                   !PVD->getType()->isReferenceType()))
        continue;

      if (PVD->getType().isConstQualified())
        continue;

      SourceLocation Loc = PVD->getLocation();
      if (Loc.isInvalid() || Mgr.getSourceManager().isInSystemHeader(Loc))
        continue;

      PathDiagnosticLocation DiagLoc(Loc, BR.getSourceManager());
      BR.EmitBasicReport(CurrentDecl, Checker, "Parameter can be const",
                         "GJB8114 Rule A-2-7-2",
                         "Pointer/reference parameter '" +
                             PVD->getNameAsString() +
                             "' is not modified; consider making it 'const'.",
                         DiagLoc, PVD->getSourceRange());
    }
    return true;
  }

private:
  BugReporter &BR;
  AnalysisManager &Mgr;
  const CheckerBase *Checker;
  const Decl *CurrentDecl;
};

class GJB_A_2_7_2_Checker : public Checker<check::ASTCodeBody> {
public:
  void checkASTCodeBody(const Decl *D, AnalysisManager &Mgr,
                        BugReporter &BR) const {
    GJB_A_2_7_2_Visitor Visitor(BR, Mgr, this, D);
    Visitor.TraverseDecl(const_cast<Decl *>(D));
  }
};
} // namespace

namespace clang {
namespace ento {
void registerGJB_A_2_7_2(CheckerManager &mgr) {
  mgr.registerChecker<GJB_A_2_7_2_Checker>();
}
bool shouldRegisterGJB_A_2_7_2(const CheckerManager &mgr) { return true; }
} // namespace ento
} // namespace clang