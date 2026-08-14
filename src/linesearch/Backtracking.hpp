#pragma once
#include "linesearch/Base.hpp"

namespace moe::linesearch {

class Backtracking : public Base{
 public:
   std::expected<Output, Error> run(const Input& input) const override;
  
  // Constants in the algorithm are public for serialization via Glaze.
  double starting_step = 1;
  double shrink_factor = 0.5;
  double smallest_step = 1e-7;
  double armijo_constant = 1e-3;
};

}// namespace moe::linesearch
