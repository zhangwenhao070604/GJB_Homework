#include "clang/StaticAnalyzer/Core/BugReporter/BugReporter.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Decl.h"
#include "clang/Basic/SourceManager.h"
#include <vector>

using namespace clang;
using namespace ento;

namespace {
class GJB_A_1_8_1_Checker : public Checker<check::ASTCodeBody> {
public:
  void checkASTCodeBody(const Decl *D, AnalysisManager &Mgr,
                        BugReporter &BR) const {
    if (const auto *FD = dyn_cast<FunctionDecl>(D)) {
      if (Stmt *Body = FD->getBody()) {
        std::vector<Stmt *> Stack;
        Stack.push_back(Body);
        while (!Stack.empty()) {
          Stmt *S = Stack.back();
          Stack.pop_back();

          if (isa<NullStmt>(S)) {
            SourceLocation Loc = S->getBeginLoc();
            if (Loc.isValid() &&
                !Mgr.getSourceManager().isInSystemHeader(Loc) &&
                !Mgr.getSourceManager().isMacroBodyExpansion(Loc) &&
                !Mgr.getSourceManager().isMacroArgExpansion(Loc)) {
              PathDiagnosticLocation DiagLoc(Loc, BR.getSourceManager());
              BR.EmitBasicReport(
                  D, this, "Empty statement", "GJB8114 Rule A-1-8-1",
                  "Avoid empty statements (standalone semicolon).",
                  DiagLoc, S->getSourceRange());
            }
          }

          for (Stmt *Child : S->children())
            if (Child)
              Stack.push_back(Child);
        }
      }
    }
  }
};
} // namespace

namespace clang {
namespace ento {
void registerGJB_A_1_8_1(CheckerManager &mgr) {
  mgr.registerChecker<GJB_A_1_8_1_Checker>();
}
bool shouldRegisterGJB_A_1_8_1(const CheckerManager &mgr) { return true; }
} // namespace ento
} // namespace clang