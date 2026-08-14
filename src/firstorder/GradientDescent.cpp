#include "firstorder/GradientDescent.hpp"

namespace moe::firstorder {

std::expected<Output, Error> GradientDescent::run(const Input& input) const {
  Eigen::VectorXd state = input.initial;
  for(int it = 0; it < 10; ++it) {
    Eigen::VectorXd grad = input.gradient(state);
    Eigen::VectorXd direction = -grad;
    moe::linesearch::Input line_search_input = moe::linesearch::Input::make(state, direction, input.objective, input.gradient);
    moe::linesearch::Output line_search_output = moe::linesearch::run(line_search, line_search_input);
    state += direction * line_search_output.step_length;
  }

  Output output = {.minimizer = state};
  return output;
}

} // namespace moe::firstorder
