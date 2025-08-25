/*
 * Code: lexer.ixx
 *
 * @Author LzzKill
 * @License GNU Public License v3.0
 *
 *
 * */

module;

#include <string>
#include <string_view>
#include <vector>

export module moonlisp.lexer;

export namespace moonlisp
{
  enum LexerType
  {
    NUMBER,
    NAME,
    STRING,
    SYMBOL,
    _EOF
  };

  struct LexerStruct
  {
    LexerType type;
    std::string word;
    unsigned int line;
    unsigned int column;
  };

  class Lexer
  {
    private:
    std::string_view input;
    unsigned int line, column, inputPos;
    char current;

    char next();
    char peek();

    std::string findSymbol();
    std::string findNumber();
    std::string findString();

    public:
    explicit Lexer(std::string_view);
    std::vector<LexerStruct> getGroupStruct();
    LexerStruct getNext();
  };

} // namespace moonlisp
