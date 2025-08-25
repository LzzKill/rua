/*
 * Code: ast.ixx
 *
 * @Author LzzKill
 * @License BSD4-Clause License
 *
 *
 * */

module;
#include <string>
#include <variant>
#include <vector>
#include <memory>

export module moonlisp.ast;

import moonlisp.constant;

export namespace moonlisp
{
  struct Atom
  {
    moonlisp::LexerType type;
    std::string value;
  };

  struct List; struct Pair;

  using Node = std::variant<Atom, List, Pair>;

  struct List
  {
    std::vector<Node> elements;
  };

  struct Pair
  {
    std::vector<Node> elements;
  };

  using TopNode = std::vector<std::unique_ptr<Node>>;
} // namespace moonlisp::AST
