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

namespace moonlisp
{
  struct bracket
  {
    int max = 0, mid = 0, min = 0;

    bool isEmpty() { return max == 0 and mid == 0 and min == 0; }
  };
  class Parser
  {
    std::unique_ptr<moonlisp::Lexer> lexer;
    TopNode node;
    bracket count;
    std::unique_ptr<LexerStruct> lex;

    public:
    Parser(std::unique_ptr<Lexer>);
    TopNode getAST() { return std::move(this->node); }
    const TopNode &viewAST() const { return this->node; }

    private:
    void parse();
    void getNext();

    Node parseList();
    Node parseAtom();
    Node parsePair();

    template <bool allMode> // true: match all bracket, false: match ( ) only
    bool is_bracket();

    bool isAtom();
  };
} // namespace moonlisp