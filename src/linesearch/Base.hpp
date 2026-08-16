#pragma once
#include <expected>
#include <memory>
#include <functional>
#include <string_view>

#include <Eigen/Dense>

namespace moe::linesearch {

struct Input {
  std::function<double(double)> objective_on_line;
  std::function<double(double)> gradient_on_line;

  static Input make(
    const Eigen::Ref<const Eigen::VectorXd> state,
    const Eigen::Ref<const Eigen::VectorXd> direction,
    std::function<double(const Eigen::Ref<const Eigen::VectorXd>)> objective,
    std::function<Eigen::VectorXd(const Eigen::Ref<const Eigen::VectorXd>)> gradient
  );
};

struct Output {
  double step_length = 0;
};

enum class Error {
  step_too_small 
};

class Base {
 public:
  virtual std::expected<Output, Error> run(const Input& input) const = 0;
};

// helper methods
std::unique_ptr<Base> fromJson(std::string_view json);

}// namespace moe::linesearch
