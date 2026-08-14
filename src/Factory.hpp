#pragma once

#include <variant>

#include <glaze/glaze.hpp>

namespace moe {


template<typename Parent, typename Children>
std::unique_ptr<Parent> factory(Children&& children) {
  return std::visit(
      [](const auto& c) -> std::unique_ptr<Parent> {
        using Child = std::decay_t<decltype(c)>; // extract which of the child classes we've rehydrated
        return std::make_unique<Child>(std::move(c));
      });
}


template<typename Parent, typename Children>
std::unique_ptr<Parent> factoryFromJson(std::string_view json) 
{
  Children child;
  auto ec = glz::read_json(child, json);
  assert(ec && "failed to parse json");
  return std::visit(
      [](const auto& c) -> std::unique_ptr<Parent> {
        using Child = std::decay_t<decltype(c)>; // extract which of the child classes we've rehydrated
        return std::make_unique<Child>(std::move(c));
      }, child);
}

} // namespace moe
