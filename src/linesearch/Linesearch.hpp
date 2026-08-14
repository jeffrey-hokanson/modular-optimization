#pragma once
#include <expected>
#include <functional>

namespace moe::linesearch {

struct Input {
  std::function<double(double)> objective_on_line;
  std::function<double(double)> gradient_on_line;
};

struct Output {
  double step_length = 0;
};

enum class Error {
  step_too_small 
};

class LineSearch {
 public:
  virtual std::expected<Output, Error> run(const Input& input) const = 0;
};

}// namespace moe::linesearch
