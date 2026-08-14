#include "linesearch/Children.hpp"

namespace moe::linesearch {

Output run(const Children& child, const Input& input) {
  // I'm not thrilled with using a variant, but constructing a shared ptr is swimming upstream
  // when using glaze to directly hydrate each class
  return std::visit([input](auto child) {
    auto res = child.run(input);
    assert(res && "line search failed");
    return *res;
  }, child);
}


} // namespace moe::linesearch
