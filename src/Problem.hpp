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

  // ---  Gradient  ---
  void setGradient(std::function<Eigen::VectorXd(Eigen::Ref<Eigen::VectorXd>)>&& grad)
  {
    _gradient = grad;
  }

  std::function<Eigen::VectorXd(Eigen::Ref<Eigen::VectorXd>)> getGradient() 
  {
    assert (_gradient && "Must have a gradient");
    return [&](Eigen::Ref<Eigen::VectorXd> state) {
      ++_gradient_calls;
      return (*_gradient)(state);
    };
  }

  size_t gradientCalls() const {
    return _gradient_calls;
  }

 private:
  std::function<double(Eigen::Ref<Eigen::VectorXd>)> _objective;
  std::optional<std::function<Eigen::VectorXd(Eigen::Ref<Eigen::VectorXd>)>> _gradient;
  std::optional<std::function<Eigen::MatrixXd(Eigen::Ref<Eigen::VectorXd>)>> _hessian;

  size_t _objective_calls = 0;
  size_t _gradient_calls = 0;

};

} // namespace moe
