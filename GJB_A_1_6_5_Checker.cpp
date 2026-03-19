// 规则 A-1-6-5：避免使用 strcat，应使用 strncat 替代
#include "clang/AST/Decl.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "llvm/ADT/StringRef.h"

using namespace clang;
using namespace ento;

namespace {
class GJB_A_1_6_5_Checker : public Checker<check::PreCall> {
  mutable std::unique_ptr<BugType> BT;

public:
  void checkPreCall(const CallEvent &Call, CheckerContext &C) const {
    const IdentifierInfo *II = Call.getCalleeIdentifier();
    if (!II)
      return;
    if (II->getName() == "strcat") {
      if (!BT) {
        BT.reset(new BugType(this, "Avoid strcat()", "GJB8114 Rule A-1-6-5"));
      }
      ExplodedNode *N = C.generateNonFatalErrorNode();
      if (!N)
        return;
      auto R = std::make_unique<PathSensitiveBugReport>(
          *BT, "Prefer strncat() over strcat() to avoid buffer overflow", N);
      R->addRange(Call.getSourceRange());
      C.emitReport(std::move(R));
    }
  }
};
} // namespace

namespace clang {
namespace ento {
void registerGJB_A_1_6_5(CheckerManager &mgr) {
  mgr.registerChecker<GJB_A_1_6_5_Checker>();
}
bool shouldRegisterGJB_A_1_6_5(const CheckerManager &mgr) { return true; }
} // namespace ento
} // namespace clang