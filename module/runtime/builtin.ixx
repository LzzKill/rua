/*
 * Code: builtin.ixx
 *
 * @Author LzzKill
 * @License GNU Public License v3.0
 *
 *
 * */
module;

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>

export module moonlisp.runtime.builtin;

import moonlisp.runtime.value;
// import moonlisp.exception;

export namespace moonlisp::runtime::builtin
{
  auto print = util::make_native(
      [](const ListValue &value) -> Value_p { // 整个函数返回 Value_p
        for (const auto &i : value) {         // 使用 const auto&
          if (!i) {
            // 或者抛出 RuntimeError，如果你定义了它并且包含了头文件
            throw std::runtime_error(
                "Internal error: null value in print arguments");
          }
          std::visit(
              [](const auto &m) { // Lambda 不需要返回 Value_p，因为它内部不返回
                using T = std::decay_t<decltype(m)>;
                if constexpr (std::is_same_v<T, int> ||
                              std::is_same_v<T, double> ||
                              std::is_same_v<T, std::string>) {
                  std::cout << m;
                } else {
                  // 对于不支持打印的类型，可以选择忽略或抛出异常
                  // 这里为了简化，我们抛出异常
                  throw std::runtime_error("Type not printable by 'print'");
                }
              },
              i->data);
        }
        std::cout << std::flush; // 确保输出立即显示
        // print 函数本身通常不返回有意义的值，所以返回 nullptr
        return nullptr; // 或者 return
                        // util::make_value<std::monostate>(std::monostate{});
      });

  // --- 添加 + 函数 ---
  //auto add = util::make_native([](const ListValue &args) -> Value_p {
  //  if (args.size() != 2) {
  //     同样，需要抛出 RuntimeError 或 std::runtime_error
  //    throw std::runtime_error("Wrong number of arguments for + (requires 2)");
  //  }
  //  if (!args[0] || !args[1]) {
  //    throw std::runtime_error("Internal error: null argument to +");
  //  }

  //   使用嵌套 visit 来处理两个参数的不同组合
  //  return std::visit(
  //      [](auto &a) -> Value_p { // 第一个参数的 visitor
  //        using T = std::decay_t<decltype(a)>;
  //        return [a](auto &b) -> Value_p { // 捕获 a, 处理第二个参数 b
  //          using U = std::decay_t<decltype(b)>;
  //          if constexpr ((std::is_same_v<T, int> ||
  //                         std::is_same_v<T, double>) &&
  //                        (std::is_same_v<U, int> ||
  //                         std::is_same_v<U, double>)) {
  //             简单的类型提升 (都转成 double)
  //            return util::make_float(static_cast<double>(a) +
  //                                    static_cast<double>(b));
  //          } else {
  //            throw std::runtime_error("Invalid types for + operator");
  //          }
  //        };
  //      },
  //      args[0]->data)(
  //      args[1]->data); // 对第一个参数 visit，然后对结果 lambda 应用第二个参数
  //});

  std::shared_ptr<Environment> getBuiltinEnvironment()
  {
    auto m = std::make_shared<Environment>();
    m->setGlobal("print", print);
    //m->setGlobal("+", add); // 注册 + 函数
    return m;
  }

} // namespace moonlisp::runtime::builtin