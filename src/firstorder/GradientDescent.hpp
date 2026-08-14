#pragma once

#include "firstorder/Base.hpp"
#include "linesearch/Children.hpp"

namespace moe::firstorder {

class GradientDescent: public Base {
 public:
  std::expected<Output, Error> run(const Input& input) const override;

  moe::linesearch::Children line_search;
};


} // namespace moe::firstorder
