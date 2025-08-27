/*
 * Code: ast.ixx
 *
 * @Author LzzKill
 * @License BSD4-Clause License
 *
 *
 * */

module;
#include <memory>
#include <string>
#include <variant>
#include <vector>

export module moonlisp.ast;

import moonlisp.lexer;

export namespace moonlisp::ast
{
  enum class NodeType
  {
    // atom;
    FLOAT,
    NUMBER,
    NAME,
    STRING,
    LIST,
    PAIR,
  };

  struct Atom;
  struct List;
  struct Pair;

  using Node = std::variant<std::unique_ptr<Atom>, std::unique_ptr<List>,
                            std::unique_ptr<Pair>>;



  struct Atom
  {
    NodeType type;
    std::string value;
  };

  struct List
  {
    NodeType type = NodeType::LIST;
    std::vector<Node> elements;
  };

  struct Pair
  {
    NodeType type = NodeType::PAIR;
    std::vector<Node> elements;
  };

  using TopNode = std::vector<Node>;

  // 辅助函数
  
  NodeType getNodeType(moonlisp::LexerType type)
  {
    switch (type) {
    case FLOAT:
      return NodeType::FLOAT;
    case NUMBER:
      return NodeType::NUMBER;
    case STRING:
      return NodeType::STRING;
    default:
      return NodeType::NAME;
    }
  };

  /*
  (print 1)
  (print (+ 1 2))

  //

  TopNode
  {
    List
    {
      Atom(NAME, "print"),
      Atom(NUMBER, "1")
    },
    {
      List
      {
        Atom(NAME, "print"),
        List
        {
          Atom(NAME, "+"),
          Atom(NUMBER, "1"),
          Atom(NUMBER, "2")
        }
      }
    }
  }
  */
} // namespace moonlisp
