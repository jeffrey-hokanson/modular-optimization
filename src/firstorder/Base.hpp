#pragma once

#include <expected>
#include <memory>
#include <functional>
#include <string_view>

#include <Eigen/Dense>

namespace moe::firstorder {

struct Input {
  Eigen::Ref<Eigen::VectorXd> initial;
  std::function<double(const Eigen::Ref<const Eigen::VectorXd>)> objective;
  std::function<Eigen::VectorXd(const Eigen::Ref<const Eigen::VectorXd>)> gradient;
};

struct Output {
  Eigen::VectorXd minimizer;
};

enum class Error {
  unknown
};

class Base {
 public:
  virtual std::expected<Output, Error> run(const Input& input) const = 0;
};

} // namespace moe::firstorder
