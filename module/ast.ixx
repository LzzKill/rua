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
#include <utility>
#include <variant>
#include <vector>

export module moonlisp.ast;

import moonlisp.constant;

export namespace moonlisp
{

  struct Atom;
  struct List;
  struct Pair;

  using Node = std::variant<std::unique_ptr<Atom>, std::unique_ptr<List>,
                            std::unique_ptr<Pair>>;



  struct Atom
  {
    ASTNodeType type;
    std::string value;
  };

  struct List
  {
    ASTNodeType type = ASTNodeType::LIST;
    std::vector<Node> elements;
  };

  struct Pair
  {
    ASTNodeType type = ASTNodeType::PAIR;
    std::vector<Node> elements;
  };

  using TopNode = std::vector<Node>;

  // 辅助函数
  
  ASTNodeType getNodeType(moonlisp::LexerType type)
  {
    switch (type) {
    case FLOAT:
      return ASTNodeType::FLOAT;
    case NUMBER:
      return ASTNodeType::NUMBER;
    case STRING:
      return ASTNodeType::STRING;
    default:
      return ASTNodeType::NAME;
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

  TopNode
  {
    Node
    {
      Node(NAME, "print"),
      Node(NUMBER, "1")
    },
    Node
    {
      Node(NAME, "print"),
      Node
      {
        Node(NAME, "+"),
        Node(NUMBER, "1"),
        Node(NUMBER, "2")
      }
    }
  }
  */
} // namespace moonlisp
