#include <gtest/gtest.h>

#include "Problem.hpp"

TEST(SanityCheck, Basic) {
  auto obj = [](Eigen::Ref<Eigen::VectorXd> state) {
    return state[0] * state[0];
  };

  moe::Problem prob;
  prob.setObjective(obj);

  EXPECT_EQ(prob.objectiveCalls(), 0);

  Eigen::VectorXd x(1);
  x << 1.0;
  auto wrapped_objective = prob.getObjective();
  wrapped_objective(x);
  EXPECT_EQ(prob.objectiveCalls(), 1);

  // check that repeated calls for the same values are identical
  wrapped_objective(x);
  EXPECT_EQ(prob.objectiveCalls(), 1);
}

