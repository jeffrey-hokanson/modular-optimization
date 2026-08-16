#pragma once

#include <functional>
#include <optional>

#include <Eigen/Dense>


namespace moe {

class Problem {
 public:
  Problem() = default;

 
  // ---  Objective  ---
  void setObjective(std::function<double(Eigen::Ref<Eigen::VectorXd>)>&& obj)
  {
    _objective = obj;
  }


  std::function<double(Eigen::Ref<Eigen::VectorXd>)> getObjective() 
  {
    return [&](Eigen::Ref<Eigen::VectorXd> state) {
      ++_objective_calls;
      return _objective(state);
    };
  }

  size_t objectiveCalls() const {
    return _objective_calls;
  }

 private:
  std::function<double(Eigen::Ref<Eigen::VectorXd>)> _objective;
  std::optional<std::function<Eigen::VectorXd(Eigen::VectorXd)>> _gradient;
  std::optional<std::function<Eigen::MatrixXd(Eigen::VectorXd)>> _hessian;

  size_t _objective_calls = 0;

};

} // namespace moe
