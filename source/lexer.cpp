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

import moonlisp.constant;
import moonlisp.exception;


char moonlisp::Lexer::next()
{
  if (this->inputPos < this->input.length()) {
    this->current = this->input[this->inputPos++]; 
    this->column++;
    if (this->current == '\r') {
      this->line++;
      this->column = 0;
      if (this->peek() == '\n') // CRLF AND CR
        column++;
    }
    if (this->current == '\n') { // ONLY LF
      this->line++;
      this->column = 0;
    }
  }
  else this->current = EOF;
  return this->current;
}

char moonlisp::Lexer::peek()
{
  if (this->inputPos + 1 < this->input.length()) {
    return this->input[this->inputPos + 1];
  }
  return EOF;
}

std::string moonlisp::Lexer::findSymbol()
{
  std::string temp{this->current};
  switch (this->current) {
  case '!': //!=
  case '<': //<=
  case '>': //>=
  {
    if (this->peek() == '=')
      temp.push_back(this->next());
  }
  }
  this->next();
  return temp;
}

std::unique_ptr<moonlisp::LexerStruct> moonlisp::Lexer::findNumber()
{
  std::string result{this->current};
  bool isFloat = false, dotInEnd = false;
  while (util::isNumber(this->peek()) or
         this->peek() == '.') { // 保证下一个是数字或者点
    if (this->next() == '.') {
      if (isFloat) {
        throw LexerError(this->line, this->column, "multiple dots");
        continue;
      }
      isFloat = true;
      dotInEnd = true;
      result.push_back(this->current);
    }
    if (isFloat and dotInEnd)
      dotInEnd = false;
    result.push_back(this->current);
  }
  if (dotInEnd) {
    throw LexerError(this->line, this->column, "dot in end");
    result.push_back(0);
  }
  this->next();
  return this->makeLexerStruct(isFloat? FLOAT : NUMBER, result);
}

std::string moonlisp::Lexer::findString()
{
  std::string result;
  char endChar = this->current;
  while (this->next() != endChar and this->current != EOF) {
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
    throw LexerError(this->line, this->column, "string not closed");
  }
  this->next();
  return result;
}

std::unique_ptr<moonlisp::LexerStruct>
moonlisp::Lexer::makeLexerStruct(LexerType type, std::string word)
{
  return std::make_unique<LexerStruct>(
      LexerStruct{type, std::move(word), this->line, this->column});
}

moonlisp::Lexer::Lexer(std::string_view input)
    : input(input), line(1), column(0), inputPos(0), current(EOF)
{
  this->next();
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

// 我们判定需要的是this-> current,不是this->next()
//

std::unique_ptr<moonlisp::LexerStruct> moonlisp::Lexer::getNext()
{
  std::string text{};
  while (this->current != EOF) {
    if (util::isNext(this->current)) {
      this->next();
      if (!text.empty())
        return this->makeLexerStruct(NAME, text);
      continue;
    }
    if (util::isSymbol(this->current)) {
      if (!text.empty())
        return this->makeLexerStruct(NAME, text);
      return this->makeLexerStruct(SYMBOL, findSymbol());
    }

    if (util::isNumber(this->current)) {
      if (!text.empty())
        goto _DEFAULT;
      return findNumber(); // include double able: number or float;
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
      while (!util::isNext(this->current) and this->current != EOF)
        this->next();
      continue;
    }
    if (this->current == '\"' or this->current == '\'') {
      if (!text.empty())
        return this->makeLexerStruct(NAME, text);
      return this->makeLexerStruct(STRING, findString());
    }
  _DEFAULT:
    text.push_back(this->current);
    this->next();
  }
  return this->makeLexerStruct(_EOF, {});
}