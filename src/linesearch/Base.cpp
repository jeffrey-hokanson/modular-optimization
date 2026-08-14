#include "linesearch/Base.hpp"

#include <string_view>
#include <variant>


#include "Factory.hpp"
#include "linesearch/Children.hpp"
#include "linesearch/Base.hpp"
#include "linesearch/Backtracking.hpp"

namespace moe::linesearch {


std::unique_ptr<Base> fromJson(std::string_view json)
{
  return factoryFromJson<Base, Children>(json); 
}

Input make(
  const Eigen::Ref<const Eigen::VectorXd> state,
  const Eigen::Ref<const Eigen::VectorXd> direction,
  std::function<double(const Eigen::Ref<const Eigen::VectorXd>)> objective,
  std::function<Eigen::VectorXd(const Eigen::Ref<const Eigen::VectorXd>)> gradient
) {
  return {
    .objective_on_line = [&](double x) -> double{ return objective(state + x * direction);},
    .gradient_on_line = [&](double x) -> double { return direction.dot(gradient(state + x * direction));}
  };
}

} // namespace moe::linesearch
