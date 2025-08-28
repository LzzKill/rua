/*
 * Code: parser.ixx
 *
 * @Author LzzKill
 * @License BSD4-Clause License
 *
 *
 * */

module;
#include <memory>
#include <utility>
export module moonlisp.parser;

import moonlisp.ast;
import moonlisp.lexer;
import moonlisp.constant;

export namespace moonlisp
{
  class Parser
  {
    std::unique_ptr<Lexer> lexer;
    ast::TopNode node;
    LexerStruct_p lex;

    public:
    explicit Parser(std::unique_ptr<Lexer> lexer)
        : lexer(std::move(lexer)), lex(nullptr)
    {
      this->parse();
    };
    ast::TopNode getAST() { return std::move(this->node); }

    private:
    void parse();
    void getNext();

    ast::Node parseList();
    ast::Node parsePair();
    inline ast::Node parseAtom();

    bool isBracket();
  };
} // namespace moonlisp
