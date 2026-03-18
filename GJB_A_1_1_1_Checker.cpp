#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"

namespace {
class GJB_A_1_1_1_Checker : public clang::ento::Checker<
                                clang::ento::check::PreStmt<clang::CallExpr>> {
  mutable std::unique_ptr<clang::ento::BugType> BT;

public:
  void checkPreStmt(const clang::CallExpr *CE,
                    clang::ento::CheckerContext &C) const {
    if (!BT)
      BT.reset(
          new clang::ento::BugType(this, "GJB A_1_1_1 violation", "GJB8114"));

    clang::ento::ExplodedNode *N = C.generateErrorNode();
    auto R = std::make_unique<clang::ento::PathSensitiveBugReport>(
        *BT, "Function call detected (示例检测)", N);
    C.emitReport(std::move(R));
  }
};
} // namespace

namespace clang {
namespace ento {
void registerGJB_A_1_1_1(CheckerManager &mgr) {
  mgr.registerChecker<GJB_A_1_1_1_Checker>();
}

bool shouldRegisterGJB_A_1_1_1(const CheckerManager &mgr) { return true; }
} // namespace ento
} // namespace clang