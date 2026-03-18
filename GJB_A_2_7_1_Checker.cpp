#include "clang/AST/DeclCXX.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"

using namespace clang;
using namespace ento;

namespace {
class GJB_A_2_7_1_Visitor : public RecursiveASTVisitor<GJB_A_2_7_1_Visitor> {
public:
  explicit GJB_A_2_7_1_Visitor(BugReporter &BR, AnalysisManager &Mgr,
                               const CheckerBase *Checker, const Decl *D)
      : BR(BR), Mgr(Mgr), Checker(Checker), CurrentDecl(D) {}

  bool VisitCXXRecordDecl(CXXRecordDecl *RD) {
    if (!RD->isCompleteDefinition())
      return true;

    for (auto *Method : RD->methods()) {
      if (Method->isImplicit() || Method->isOutOfLine())
        continue;
      if (Method->doesThisDeclarationHaveABody()) {
        SourceLocation Loc = Method->getLocation();
        if (Loc.isInvalid() || Mgr.getSourceManager().isInSystemHeader(Loc))
          continue;

        PathDiagnosticLocation DiagLoc(Loc, BR.getSourceManager());
        BR.EmitBasicReport(
            CurrentDecl, Checker, "Inline member definition",
            "GJB8114 Rule A-2-7-1",
            "Member function '" + Method->getNameAsString() +
                "' is defined inside the class; move definition out of line.",
            DiagLoc, Method->getSourceRange());
      }
    }
    return true;
  }

private:
  BugReporter &BR;
  AnalysisManager &Mgr;
  const CheckerBase *Checker;
  const Decl *CurrentDecl;
};

class GJB_A_2_7_1_Checker : public Checker<check::ASTCodeBody> {
public:
  void checkASTCodeBody(const Decl *D, AnalysisManager &Mgr,
                        BugReporter &BR) const {
    GJB_A_2_7_1_Visitor Visitor(BR, Mgr, this, D);
    Visitor.TraverseDecl(const_cast<Decl *>(D));
  }
};
} // namespace

namespace clang {
namespace ento {
void registerGJB_A_2_7_1(CheckerManager &mgr) {
  mgr.registerChecker<GJB_A_2_7_1_Checker>();
}
bool shouldRegisterGJB_A_2_7_1(const CheckerManager &mgr) { return true; }
} // namespace ento
} // namespace clang