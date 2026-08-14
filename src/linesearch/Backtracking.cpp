#include "linesearch/Backtracking.hpp"

namespace moe::linesearch {

std::expected<Output, Error> Backtracking::run(const Input& input) const {
  const double grad0 = input.gradient_on_line(0.);
  const double obj0 = input.objective_on_line(0.);
  double step = starting_step;

  while(step > smallest_step) {
    const double obj1 = input.objective_on_line(step);
    const bool armijo_condition = (obj0 - obj1) >= step * (-grad0 * step * armijo_constant);
    if (armijo_condition) {
      return Output{.step_length = step};
    }
    step *= shrink_factor;
  }
  return std::unexpected(Error::step_too_small); 
}

} // namespace moe::linesearch
