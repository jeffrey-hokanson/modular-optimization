#include "linesearch/Base.hpp"

#include <string_view>
#include <variant>

#include "Factory.hpp"
#include "linesearch/Base.hpp"
#include "linesearch/Backtracking.hpp"

namespace moe::linesearch {

using Children = std::variant<Backtracking>;

std::unique_ptr<Base> fromJson(std::string_view json)
{
  return factoryFromJson<Base, Children>(json); 
}


} // namespace moe::linesearch
