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

class Backtracking : public LineSearch{
 public:
   std::expected<Output, Error> run(const Input& input) const override;
  
  // Constants in the algorithm are public for serialization via Glaze.
  double starting_step = 1;
  double shrink_factor = 0.5;
  double smallest_step = 1e-7;
  double armijo_constant = 1e-3;
};

}// namespace moe::linesearch
