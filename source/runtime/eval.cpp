module;
#include <memory>
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
  return Value_p();
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
  return Value_p();
}