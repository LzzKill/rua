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
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

export module moonlisp.lexer;

import moonlisp.constant;

export namespace moonlisp
{

  using Place = std::array<int, 3>; // line column, pos
  
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
    Place place;
  };

  using LexerStruct_p = std::unique_ptr<moonlisp::LexerStruct>;

  // TODO: 迭代器
  class Lexer
  {
    std::string input;
    Place place;
    char current;

    public:
    explicit Lexer(std::string input) : input(input), place(0, 0, 0), current(EOF)
    {
      this->next();
    }
    std::vector<LexerStruct_p> getGroupStruct();
    LexerStruct_p getNext();

    private:
    char next();
    char peek();

    LexerStruct_p makeSymbolLexerStruct();
    template <bool> LexerStruct_p makeNumberLexerStruct();
    LexerStruct_p makeStringLexerStruct();

    inline LexerStruct_p makeLexerStruct(LexerType, std::string &);
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
    export constexpr bool isNextLine(char c)
    {
      return isInTable(NEXT_TABLE, c);
    }
    export constexpr bool isNote(char c) { return isInTable(NOTE_TABLE, c); }
  } // namespace util

} // namespace moonlisp
