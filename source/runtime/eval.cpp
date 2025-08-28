/*
 * Code: eval.cpp
 *
 * @Author LzzKill
 * @License GNU Public License v3.0
 *
 *
 * */

module;
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <variant>
module moonlisp.runtime.eval;

import moonlisp.runtime.value;
import moonlisp.ast;

using moonlisp::ast::Atom;
using moonlisp::ast::List;
using moonlisp::ast::Node;
using moonlisp::ast::Pair;
using moonlisp::runtime::Environment;
using moonlisp::runtime::Value_p;

Value_p moonlisp::runtime::eval(const moonlisp::ast::Node &node,
                                const std::shared_ptr<Environment> &env)
{
  return std::visit(
      [&](auto &&arg) -> Value_p {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Atom>)
          return evalAtom(arg, env);
        if constexpr (std::is_same_v<T, List>)
          return evalList(arg, env);
        if constexpr (std::is_same_v<T, Pair>)
          return evalPair(arg, env);
        throw std::runtime_error("Unknown AST");
      },
      node.node);
}
Value_p moonlisp::runtime::evalList(const List &list,
                                    const std::shared_ptr<Environment> &env)
{
  return Value_p();
}
Value_p moonlisp::runtime::evalPair(const Pair &pair,
                                    const std::shared_ptr<Environment> &env)
{
  return Value_p();
}
Value_p moonlisp::runtime::evalAtom(const Atom &atom,
                                    const std::shared_ptr<Environment> &env)
{
  switch (atom.type) {
  case ast::NodeType::FLOAT:
  case ast::NodeType::NUMBER:
    return std::make_shared<Value>(atom.value);
  case ast::NodeType::STRING:
    return std::make_shared<Value>(atom.value);
  case ast::NodeType::NAME:
    return env->get(atom.value);
  default:
    throw std::runtime_error("Unknown Type");
    return nullptr;
  }
}