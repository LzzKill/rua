/*
 * Code: main.cpp
 *
 * @Author LzzKill
 * @License GNU Public License v3.0
 *
 *
 * */
#include <cstdlib>
#include <exception>
#include <format>
#include <iosfwd>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

import moonlisp.constant;
import moonlisp.lexer;
import moonlisp.parser;
import moonlisp.ast;
import moonlisp.exception;

std::string_view get_string(moonlisp::LexerType lexer_type)
{
  switch (lexer_type) {
  case moonlisp::NUMBER:
    return "NUMBER";
  case moonlisp::NAME:
    return "NAME";
  case moonlisp::STRING:
    return "STRING";
  case moonlisp::SYMBOL:
    return "SYMBOL";
  case moonlisp::_EOF:
    return "_EOF";
  }
}

std::string_view get_string(moonlisp::ast::NodeType node_type)
{
  using moonlisp::ast::NodeType;
  switch (node_type) {
  case NodeType::FLOAT:
    return "FLOAT";
  case NodeType::LIST:
    return "LIST";
  case NodeType::PAIR:
    return "PAIR";
  case NodeType::NUMBER:
    return "NUMBER";
  case NodeType::STRING:
    return "STRING";
  case NodeType::NAME:
    return "NAME";
  case NodeType::DOT:
    return "DOT";
  }
  return "UNKNOWN"; // ← 兜底，避免 UB
}

// 小工具：缩进
inline std::string indent(int depth) { return std::string(depth * 2, ' '); }

// 萃取：判断是否 unique_ptr<T>
template <typename T> struct is_unique_ptr : std::false_type
{
};
template <typename T> struct is_unique_ptr<std::unique_ptr<T>> : std::true_type
{
  using pointee = T;
};

// 获取指针或对象的“引用”统一接口
template <typename T> decltype(auto) deref(T &x)
{
  if constexpr (is_unique_ptr<std::decay_t<T>>::value) {
    return *x;
  } else {
    return x;
  }
}

// 递归打印单个 Node
inline void viewAST(const moonlisp::ast::Node &node, int depth = 0,
                    std::ostream &os = std::cout)
{
  std::visit(
      [&](auto const &arg_raw) {
        // 兼容 unique_ptr 与非指针持有
        auto const &arg = deref(arg_raw);
        using U = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<U, moonlisp::ast::Atom>) {
          os << indent(depth) << "Atom(" << get_string(arg.type) << "): \""
             << arg.value << "\"\n";
        } else if constexpr (std::is_same_v<U, moonlisp::ast::List>) {
          os << indent(depth) << "List:\n";
          for (auto const &elem : arg.elements) {
            viewAST(elem, depth + 1, os);
          }
        } else if constexpr (std::is_same_v<U, moonlisp::ast::Pair>) {
          os << indent(depth) << "Pair:\n";
          for (auto const &elem : arg.elements) {
            viewAST(elem, depth + 1, os);
          }
        } else {
          static_assert(!sizeof(U *),
                        "Unknown variant alternative in moonlisp::Node");
        }
      },
      node.node);
}

// 打印 TopNode
inline void viewAST(const moonlisp::ast::TopNode &top,
                    std::ostream &os = std::cout)
{
  os << "=== AST BEGIN ===\n";
  int idx = 0;
  for (auto const &n : top) {
    os << "-- Node #" << idx++ << " --\n";
    viewAST(n, /*depth*/ 1, os);
  }
  os << "=== AST END ===\n";
}

int main()
{
  std::cout << std::format("Moonlisp VERSION: {} \n", moonlisp::VERSION);
  std::string text;
  for (;;) {
    std::cout << "moonlisp> ";
    std::getline(std::cin, text);
    if (text == "(exit)" or text == "(quit)")
      return 0;

    try {
      auto lexer = std::make_unique<moonlisp::Lexer>(text);
      auto parser = std::make_unique<moonlisp::Parser>(std::move(lexer));
      auto m = parser->getAST();
      for (auto &i : m) {
        viewAST(i);
      }
    }
    catch (moonlisp::ParserError &e) {
      std::cerr << e.what() << '\n';
    }
    catch (moonlisp::LexerError &e) {
      std::cerr << e.what() << '\n';
    }
    std::cerr << std::flush;
  }

  return EXIT_SUCCESS;
}
