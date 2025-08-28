/*
 * Code: lexer.cpp
 *
 * @Author LzzKill
 * @License GNU Public License v3.0
 *
 *
 * */

module;
#include <cstdio>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

module moonlisp.lexer;

import moonlisp.exception;

using moonlisp::LexerStruct_p;
using moonlisp::LexerStructValue_t;
using moonlisp::Place;
;

char moonlisp::Lexer::next()
{
  auto &line = this->place[0];
  auto &column = this->place[1];
  auto &pos = this->place[2];
  if (pos < this->input.length()) {
    this->current = this->input[pos++];
    column++;
    if (this->current == '\r') {
      line++;
      column = 0;
      if (this->peek() == '\n') // CRLF AND CR
        column++;
    }
    if (this->current == '\n') { // ONLY LF
      line++;
      column = 0;
    }
  } else
    this->current = EOF;
  return this->current;
}

char moonlisp::Lexer::peek()
{
  auto &pos = this->place[2];
  if (pos < this->input.length()) {
    return this->input[pos];
  }
  return EOF;
}

std::vector<std::unique_ptr<moonlisp::LexerStruct>>
moonlisp::Lexer::getGroupStruct()
{
  std::vector<std::unique_ptr<LexerStruct>> result;
  std::unique_ptr<LexerStruct> res;
  bool e = true;
  while (e) {
    res = this->getNext();
    if (res->type == _EOF)
      e = false;
    result.emplace_back(std::move(res));
  }
  return result;
}

std::unique_ptr<moonlisp::LexerStruct> moonlisp::Lexer::getNext()
{
  std::string text{};
  while (this->current != EOF) {
    if (util::isNextLine(this->current)) {
      this->next();
      if (!text.empty())
        return this->makeLexerStruct(NAME, text);
      continue;
    }
    if (util::isSymbol(this->current)) { //
      if (!text.empty())
        return this->makeLexerStruct(NAME, text);
      return this->makeStringLexerStruct();
    }

    if (util::isNumber(this->current)) {
      if (!text.empty())
        goto _DEFAULT;
      return makeNumberLexerStruct<false>(); // include double able: number or
                                             // float;
    }
    if (util::isWhitespace(this->current)) {
      if (!text.empty())
        return this->makeLexerStruct(NAME, text);
      this->next();
      continue;
    }
    if (util::isNote(this->current)) {
      if (!text.empty())
        return this->makeLexerStruct(NAME, text);
      this->next();
      while (!util::isNextLine(this->current) and this->current != EOF)
        this->next();
      continue;
    }
    if (this->current == '\"') {
      if (!text.empty())
        return this->makeLexerStruct(NAME, text);
      return makeStringLexerStruct();
    }
  _DEFAULT:
    text.push_back(this->current);
    this->next();
  }
  return this->makeLexerStruct(_EOF, {});
}

LexerStruct_p moonlisp::Lexer::makeSymbolLexerStruct()
{
  std::string temp{this->current};
  switch (this->current) {
  case '!': // !=
  case '<': // <=
  case '>': // >=
  case '=': // ==
  {
    if (this->peek() == '=')
      temp.push_back(this->next());
    break;
  }
  case '-': // 负数？
    if (util::isNumber(this->peek()))
      return this->makeNumberLexerStruct<true>();
  case '+': // ++ 或者 --
    if (this->peek() == this->current)
      temp.push_back(this->next());
    break;
  }
  this->next();
  return this->makeLexerStruct(SYMBOL, temp);
}

template <bool minus> LexerStruct_p moonlisp::Lexer::makeNumberLexerStruct()
{
  bool isFloat = false;
  std::string result;

  if constexpr (minus)
    result.push_back('-');

  while (util::isNumber(this->peek()) or
         this->peek() == '.') { // 保证下一个是数字或者点
    if (this->next() == '.') {
      if (isFloat) {
        throw LexerError(this->line, this->column, "multiple dots");
        continue;
      }
      isFloat = true;
      result.push_back(this->current);
      continue;
    }
    result.push_back(this->current);
  }
  this->next();
  if (isFloat) {
    return this->makeLexerStruct(FLOAT,
                                 std::stod(result)); // 末尾是dot，可以通过吗？
  } else {
    return this->makeLexerStruct(NUMBER, std::stoi(result););
  }
}

LexerStruct_p moonlisp::Lexer::makeStringLexerStruct()
{
  std::string result;
  while (this->next() != '"' and this->current != EOF) {
    if (this->current == '\\') {
      switch (this->next()) {
      case 'n':
        result.push_back('\n');
        break;
      case 'r':
        result.push_back('\r');
        break;
      case 't':
        result.push_back('\t');
        break;
      case '\\':
        result.push_back('\\');
        break;
      case '\"':
        result.push_back('\"');
        break;
      case '\'':
        result.push_back('\'');
        break;
      default:
        result.push_back(this->current);
        break;
      }
    } else
      result.push_back(this->current);
  }
  if (this->current == EOF) {
    throw LexerError(this->place, "Unclosed string");
  }
  this->next();
  return this->makeLexerStruct(STRING, result);
}

LexerStruct_p moonlisp::Lexer::makeLexerStruct(LexerType type,
                                               LexerStructValue_t value)
{
  return std::make_unique<LexerStruct>(type, std::move(value), this->place);
}