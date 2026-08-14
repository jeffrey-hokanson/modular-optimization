# Modular Optimization Experiment

This is an experiment in software design for scientific applications. We
defined scientific applications as those for which there are many different
algorithms---sometimes nested several layers deep---where there is no universal
best method. The question becomes: which combination of algorithms is best for
a particular problem family? Often this is just combination of both parameter
tuning **and** algorithm picking. Tuning parameters is straightforward, but 
algorithm picking is much harder. How can you reach several layers down and 
swap out an algorithm for another? The goal of this experiment is to show how
C++ projects can be designed to make algorithm picking as easy as parameter tuning.

## Application Area
This project focuses on numerical optimization due to my familiarity.

## Architecture
An algorithm family consists of those algorithms that map the same inputs to
the same outputs. Each algorithm family gets its own namespace, a pair of input
and output structs, an error type, and an abstract base class that all methods in the family
inherit. Here is a minimalist implementation for a one-dimensional line search.

```C
#include <expected>
#include <functional>
namespace linesearch {
struct Input {
  std::function<double(double)> objective_restricted_to_line;
  std::function<double(double)> gradient_restrict_to_line;
};

struct Output {
  double step_length;
};

enum class Error {
  no_step_found;
};

class Linesearch {
 public:
  virtual std::expected<Output, Error> run(const Input& input) const = 0;
};
} // namespace linesearch
```

Given this architecture here's an implementation of a simple backtracking line
search.

```C
namespace linesearch {
class Backtracking {
 public:
  Output run(const Input& input) const override {

    const double grad0 = input.gradient_restricted_to_line(0.);
    const double obj0 = input.objective_restricted_to_line(0.);
    double step = starting_step;
  
    while(step > smallest_step) {
      const double obj1 = input.objective_restricted_to_line(step);
      const bool armijo_condition = (obj0 - obj1) >= step * (-grad0 * step * armijo_constant);
      if (armijo_condition) {
        return {.step_length = step};
      }
      step *= shrink_factor;
    }
    return std::unexpected(Error::no_step_found); 
  }

  // Constants in the algorithm are public for serialization via Glaze.
  double starting_step = 1;
  double shrink_factor = 0.5;
  double smallest_step = 1e-7;
  double armijo_constant = 1e-3;
};
} // namespace linesearch 
```

Runtime setup comes via [glaze](https://github.com/stephenberry/glaze) which, 
as of C++26, provides automatic hydration/dehydration from several formats including
YAML---a natural format for parameter tuning. 



