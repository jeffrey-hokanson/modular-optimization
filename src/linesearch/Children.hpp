#pragma once

#include <variant>

#include "Factory.hpp"
#include "linesearch/Backtracking.hpp"
#include "linesearch/Base.hpp"

namespace moe::linesearch {

using Children = std::variant<Backtracking>;

static_assert(all_variant_types_derive_from<Base, Children>, "All variants must derrive from base");

Output run(const Children& child, const Input& input);


} // moe::linesearch
