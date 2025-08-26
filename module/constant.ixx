/*
 * Code: constant.ixx
 *
 * @Author LzzKill
 * @License BSD4-Clause License
 *
 *
 * */

module;
#include <array>
#include <string_view>
export module moonlisp.constant;

export namespace moonlisp
{
  constexpr std::string_view VERSION{"0.1.0"};
  constexpr std::string_view AUTHOR{"LzzKill"};
  constexpr std::string_view LICENSE{"BSD-4-Clause"};

  constexpr std::array SYMBOL_TABLE = {
      '[', ']', '(', ')', '!', '+', '=', '*', '<',
      '>', '*', '&', '^', '%', ',', '.', '\''}; // {}可作为字面量一部分
  constexpr std::array SPACE_TABLE = {' ', '\f', '\t'};
  constexpr std::array NUMBER_TABLE = {'0', '1', '2', '3', '4',
                                       '5', '6', '7', '8', '9'};
  constexpr std::array NEXT_TABLE = {'\n', '\r'};
  constexpr std::array NOTE_TABLE = {';', '#'};

  enum LexerType
  {
    NUMBER,
    FLOAT,
    NAME,
    STRING,
    SYMBOL,
    _EOF
  };

  enum class ASTNodeType
  {
    // atom;
    FLOAT,
    NUMBER,
    NAME,
    STRING,
    LIST,
    PAIR,
  };

  export namespace util
  {
    template <typename Table>
    constexpr bool isInTable(const Table &table, char c)
    {
      return std::find(table.begin(), table.end(), c) != table.end();
    }

    constexpr bool isSymbol(char c) { return isInTable(SYMBOL_TABLE, c); }
    constexpr bool isWhitespace(char c) { return isInTable(SPACE_TABLE, c); }
    constexpr bool isNumber(char c) { return isInTable(NUMBER_TABLE, c); }
    constexpr bool isNext(char c) { return isInTable(NEXT_TABLE, c); }
    constexpr bool isNote(char c) { return isInTable(NOTE_TABLE, c); }
  } // namespace util
} // namespace moonlisp
