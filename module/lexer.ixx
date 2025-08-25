/*
 * Code: lexer.ixx
 *
 * @Author LzzKill
 * @License GNU Public License v3.0
 *
 *
 * */

module;

#include <memory>
#include <string>
#include <string_view>
#include <vector>

export module moonlisp.lexer;

import moonlisp.constant;

export namespace moonlisp
{

  struct LexerStruct
  {
    LexerType type;
    std::string word;
    unsigned int line;
    unsigned int column;
    unsigned int pos;
  };


  // TODO: 迭代器
  class Lexer
  {
    std::string_view input;
    unsigned int line, column, inputPos;
    char current;

    public:
    explicit Lexer(std::string_view);
    std::vector<std::unique_ptr<LexerStruct>> getGroupStruct();
    std::unique_ptr<LexerStruct> getNext();
    
    private:
    char next();
    char peek();

    std::string findSymbol();
    std::unique_ptr<moonlisp::LexerStruct> findNumber();
    std::string findString();

    inline std::unique_ptr<LexerStruct> makeLexerStruct(LexerType, std::string);
  };

} // namespace moonlisp
