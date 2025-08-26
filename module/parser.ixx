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
    TopNode node;
    std::unique_ptr<LexerStruct> lex;

    public:
    Parser(std::unique_ptr<Lexer>);
    TopNode getAST() { return std::move(this->node); }
    const TopNode &viewAST() const { return this->node; }

    private:
    void getNext();

    Node parseList();
    Node parsePair();
    inline Node parseAtom();

    bool isBracket();
  };
} // namespace moonlisp
