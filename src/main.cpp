#include <iostream>
#include <string>
#include <functional>

#include "linesearch/Base.hpp"

int main() 
{
  std::string config = R"(
    {"armijo_constant_constant" : 0.1}  
  )";

  moe::linesearch::Input input{
    .objective_on_line = [](double x) { return (-x+1)*(-x+1);},
    .gradient_on_line = [](double x) { return 2*(-x+1);},
  };

  auto line_search = moe::linesearch::fromJson(config);

  auto output = line_search->run(input);
  if(output) {
    std::cout << "step: " << (*output).step_length;
  }
  else {
    std::cout << "failed";
  }
  std::cout << "\n";
  return 0;
}
