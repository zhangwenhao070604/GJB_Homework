// 规则 A-1-6-4：避免使用 strcpy，应使用 strncpy 替代
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
class GJB_A_1_6_4_Checker : public Checker<check::PreCall> {
  mutable std::unique_ptr<BugType> BT;

public:
  void checkPreCall(const CallEvent &Call, CheckerContext &C) const {
    const IdentifierInfo *II = Call.getCalleeIdentifier();
    if (!II)
      return;
    if (II->getName() == "strcpy") {
      if (!BT) {
        BT.reset(new BugType(this, "Avoid strcpy()", "GJB8114 Rule A-1-6-4"));
      }
      ExplodedNode *N = C.generateNonFatalErrorNode();
      if (!N)
        return;
      auto R = std::make_unique<PathSensitiveBugReport>(
          *BT, "Prefer strncpy() over strcpy() to avoid buffer overflow", N);
      R->addRange(Call.getSourceRange());
      C.emitReport(std::move(R));
    }
  }
};
} // namespace

namespace clang {
namespace ento {
void registerGJB_A_1_6_4(CheckerManager &mgr) {
  mgr.registerChecker<GJB_A_1_6_4_Checker>();
}
bool shouldRegisterGJB_A_1_6_4(const CheckerManager &mgr) { return true; }
} // namespace ento
} // namespace clang