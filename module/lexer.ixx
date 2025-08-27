/*
 * Code: lexer.ixx
 *
 * @Author LzzKill
 * @License GNU Public License v3.0
 *
 *
 * */

module;

#include <array>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

export module moonlisp.lexer;

import moonlisp.constant;

export namespace moonlisp
{
  enum LexerType
  {
    NUMBER,
    FLOAT,
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

  namespace util
  {
    template <typename Table>
    constexpr bool isInTable(const Table &table, char c)
    {
      return std::find(table.begin(), table.end(), c) != table.end();
    }

    export constexpr bool isSymbol(char c)
    {
      return isInTable(SYMBOL_TABLE, c);
    }
    export constexpr bool isWhitespace(char c)
    {
      return isInTable(SPACE_TABLE, c);
    }
    export constexpr bool isNumber(char c)
    {
      return isInTable(NUMBER_TABLE, c);
    }
    export constexpr bool isNext(char c) { return isInTable(NEXT_TABLE, c); }
    export constexpr bool isNote(char c) { return isInTable(NOTE_TABLE, c); }
  } // namespace util

} // namespace moonlisp
