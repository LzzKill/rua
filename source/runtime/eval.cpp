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
#include <string>
#include <type_traits>
#include <variant>
#include <vector>
#include <format>
module moonlisp.runtime.eval;

import moonlisp.runtime.value;
import moonlisp.ast;
import moonlisp.exception;

using moonlisp::ast::Atom;
using moonlisp::ast::List;
using moonlisp::ast::Node;
using moonlisp::ast::Pair;
using moonlisp::runtime::Environment;
using moonlisp::runtime::Value_p;

// eval.cpp 中的实现思路
Value_p moonlisp::runtime::eval(const ast::Node &node,
                                const std::shared_ptr<Environment> &env)
{
  // 使用 std::visit 来处理 Node_t (variant) 的不同情况
  return std::visit(
      [&](const auto &node_ptr) -> Value_p {
        using T = std::decay_t<decltype(node_ptr)>;
        if constexpr (std::is_same_v<T, std::unique_ptr<ast::Atom>>) {
          return evalAtom(*node_ptr, env);
        } else if constexpr (std::is_same_v<T, std::unique_ptr<ast::List>>) {
          return evalList(*node_ptr, env);
        } else if constexpr (std::is_same_v<T, std::unique_ptr<ast::Pair>>) {
          return evalPair(*node_ptr, env);
        } else {
          // 不应该到达这里
          throw RuntimeError(node.place,
                             "Unknown AST node type for evaluation");
        }
      },
      node.node); // node.node 是 Node_t variant
}

Value_p moonlisp::runtime::evalAtom(const ast::Atom &atom,
                                    const std::shared_ptr<Environment> &env)
{
  switch (atom.type) {
  case ast::NodeType::NUMBER:
    // 假设 NUMBER 是整数，使用 make_float 是错误的，这里纠正
    return util::make_number(std::stoi(atom.value));
  case ast::NodeType::FLOAT:
    return util::make_float(std::stod(atom.value));
  case ast::NodeType::STRING:
    return util::make_string(atom.value);
  case ast::NodeType::NAME: {
    Value_p val = env->get(atom.value); // 在环境中查找
    if (!val) {
        throw std::runtime_error(std::format("{} not found", atom.value)); // 需要 place 信息
    }
    return val; // 返回找到的值
  }
  case ast::NodeType::DOT:
    // ... 处理点号 ...
    throw std::runtime_error("Evaluation of '.' not implemented yet");
  default:
    throw std::runtime_error("Unknown Atom type for evaluation");
  }
}

Value_p moonlisp::runtime::evalList(const ast::List &list,
                                    const std::shared_ptr<Environment> &env)
{
  if (list.elements.empty()) {
    // 空列表通常求值为自己或特殊处理
    return util::make_list({});
  }

  const ast::Node &first_node = list.elements.front();
  // 简化：假设 first_node 是一个 Atom (NAME)
  // 更健壮的实现需要检查 first_node 的类型
  if (first_node.node.index() == 0) { // index 0 对应 unique_ptr<Atom>
    const ast::Atom *first_atom =
        std::get<std::unique_ptr<ast::Atom>>(first_node.node).get();
    if (first_atom && first_atom->type == ast::NodeType::NAME) {
      const std::string &op_name = first_atom->value;

      // --- 处理特殊形式 ---
      if (op_name == "define") {
        // 检查参数数量 etc.
        if (list.elements.size() != 3) {
                  throw std::runtime_error("define needs 2 arguments");
        }
        // 假设第二个元素是变量名 Atom
        const ast::Node &var_node = list.elements[1];
        const ast::Node &val_node = list.elements[2];
        if (var_node.node.index() == 0) {
          const ast::Atom *var_atom =
              std::get<std::unique_ptr<ast::Atom>>(var_node.node).get();
          if (var_atom && var_atom->type == ast::NodeType::NAME) {
            std::string var_name = var_atom->value;
            Value_p val = eval(val_node, env); // 求值右侧表达式
            env->setLocal(var_name, val);      // 在当前环境设置变量
            return val;                        // define 通常返回设置的值或 void
          }
        }
              throw std::runtime_error("Invalid variable name in define");
      }
      // else if (op_name == "lambda") { ... }
      // else if (op_name == "if") { ... }
      // ... 其他特殊形式 ...

      // --- 处理普通函数调用 ---
      else {
        Value_p func_val = eval(first_node, env); // 求值函数名
        // 检查 func_val 是否为函数类型
        // ... (检查逻辑) ...
        if (!func_val ||
            !(std::holds_alternative<NativeFunction>(func_val->data) ||
              std::holds_alternative<LambdaFunction>(func_val->data))) {
                   throw std::runtime_error("First element of list is not a function");
        }

        // 求值参数
        std::vector<Value_p> args;
        for (size_t i = 1; i < list.elements.size(); ++i) {
          args.push_back(eval(list.elements[i], env));
        }

        // 应用函数
        if (std::holds_alternative<NativeFunction>(func_val->data)) {
          const NativeFunction &nf = std::get<NativeFunction>(func_val->data);
          return nf(args); // 调用 C++ 函数
        } else if (std::holds_alternative<LambdaFunction>(func_val->data)) {
          const LambdaFunction &lf = std::get<LambdaFunction>(func_val->data);
          // 创建新环境
          auto new_env =
              std::make_shared<Environment>(lf.env); // 闭包环境作为父环境
          // 绑定参数 (需要检查参数数量匹配)
          for (size_t i = 0; i < lf.params.size() && i < args.size(); ++i) {
            new_env->setLocal(lf.params[i], args[i]);
          }
          Value_p result = nullptr;
          // 在新环境中求值函数体
          for (const auto &body_expr : lf.body) {
            result = eval(body_expr, new_env);
          }
          return result; // 返回最后一个表达式的值
        }
      }
    }
  }
  // 如果 first_node 不是 NAME Atom，或者不是已知的特殊形式/函数名
  throw std::runtime_error("First element of list cannot be evaluated as a function or special form");
}

// evalPair 的实现取决于语言设计，暂时可以抛出未实现错误
Value_p moonlisp::runtime::evalPair(const ast::Pair &pair,
                                    const std::shared_ptr<Environment> &env)
{
    throw std::runtime_error("Evaluation of Pair not implemented yet");
}