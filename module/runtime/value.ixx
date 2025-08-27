/*
 * Code: value.ixx
 * @Module moonlisp.runtime: value
 * @Author LzzKill
 * @License GNU Public License v3.0
 *
 *
 * */

module;
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>
export module moonlisp.runtime.value;

import moonlisp.constant;
import moonlisp.ast;

export namespace moonlisp::runtime
{

  struct Value;
  // 运行时期的值
  using Value_p = std::shared_ptr<Value>;
  using ListValue = std::vector<Value_p>;
  using PairValue = std::vector<Value_p>;
  using NativeFunction = std::function<Value_p(const ListValue)>; // 原生函数

  struct LambdaFunction // lisp 函数，或者叫 lambda 函数
  {
    std::shared_ptr<struct Environment> env; // 闭包
    std::vector<std::string> params;         // 参数
    std::vector<ast::Node> body;
  };

  struct Value
  {
    using Variant =
        std::variant<std::string, ListValue, NativeFunction, LambdaFunction>;

    Variant data;
    explicit Value(Variant v) : data(std::move(v)) { }
  };

  struct Environment : std::enable_shared_from_this<Environment>
  {
    std::unordered_map<std::string, Value_p> table; // 变量表
    std::shared_ptr<Environment> parent;            // 外层作用域
    explicit Environment(std::shared_ptr<Environment> p = nullptr)
        : parent(std::move(p))
    {
    }

    bool exists(const std::string &);

    // if exists, return value; else return nullptr
    Value_p get(const std::string &);

    inline void setLocal(const std::string &, Value_p);
    inline void setGlobal(const std::string &, Value_p);
  };

  namespace util
  {
    template <typename T> inline Value_p make_value(T x)
    {
      return std::make_shared<Value>(std::move(x));
    }

    export auto make_number = make_value<std::string>;
    export auto make_string = make_value<std::string>;
    export auto make_list = make_value<ListValue>;
    export auto make_pair = make_value<PairValue>;
    export auto make_native = make_value<NativeFunction>;
    export auto make_lambda = make_value<LambdaFunction>;
  } // namespace util
} // namespace moonlisp::runtime
