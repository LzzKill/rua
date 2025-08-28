/*
 * Code: eval.ixx
 *
 * @Author LzzKill
 * @License GNU Public License v3.0
 *
 *
 * */

module;
#include <memory>
export module moonlisp.runtime.eval;

import moonlisp.runtime.value;
import moonlisp.ast;

export namespace moonlisp::runtime
{
// 评估 ast 节点
  Value_p eval(const ast::Node &node, const std::shared_ptr<Environment> &env);

  Value_p evalList(const ast::List &list,
                   const std::shared_ptr<Environment> &env);
  Value_p evalPair(const ast::Pair &pair,
                   const std::shared_ptr<Environment> &env);
  Value_p evalAtom(const ast::Atom &atom,
                   const std::shared_ptr<Environment> &env);
  
} // namespace moonlisp::runtime