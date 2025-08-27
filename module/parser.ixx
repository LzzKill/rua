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
    std::unique_ptr<moonlisp::Lexer> lexer;
    ast::TopNode node;
    std::unique_ptr<LexerStruct> lex;

    public:
    explicit Parser(std::unique_ptr<Lexer>);
    ast::TopNode getAST() { return std::move(this->node); }
    const ast::TopNode &viewAST() const { return this->node; }

    private:
    void getNext();

    ast::Node parseList();
    ast::Node parsePair();
    inline ast::Node parseAtom();

    bool isBracket();
  };
} // namespace moonlisp
