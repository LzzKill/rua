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
#include <string>
#include <string_view>
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
    return this->current;
  }
  return EOF;
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
  return temp;
}

std::string moonlisp::Lexer::findNumber()
{
  std::string result{this->current};
  bool isFloat = false, dotInEnd = false;
  while (util::isNumber(this->next()) or this->current == '.') {
    if (this->current == '.') {
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
  if (dotInEnd)
    throw LexerError(this->line, this->column, "dot in end");
  return result;
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
  return result;
}

moonlisp::Lexer::Lexer(std::string_view input)
    : input(input), line(1), column(0), inputPos(0),
      current((input.empty()) ? EOF : input[0])
{
}

std::vector<moonlisp::LexerStruct> moonlisp::Lexer::getGroupStruct()
{
  std::vector<moonlisp::LexerStruct> result;
  LexerStruct res;
  do {
    res = this->getNext();
    result.emplace_back(res);
  } while (res.type != _EOF);
  return result;
}

moonlisp::LexerStruct moonlisp::Lexer::getNext()
{
  std::string text{};
  while (this->current != EOF) {
    if (util::isNext(this->current)) {
      this->next();
      if (!text.empty())
        return LexerStruct{NAME, text, this->line, this->column};
      continue;
    }
    if (util::isSymbol(this->current)) {
      if (!text.empty())
        return LexerStruct{NAME, text, this->line, this->column};
      this->next();
      return LexerStruct{SYMBOL, this->findSymbol(), this->line, this->column};
    }

    if (util::isNumber(this->current)) {
      if (!text.empty())
        goto _DEFAULT;
      return LexerStruct{NUMBER, findNumber(), this->line, this->column};
    }
    if (util::isWhitespace(this->current)) {
      if (!text.empty())
        return LexerStruct{NAME, text, this->line, this->column};
      this->next();
      continue;
    }
    if (util::isNote(this->current)) {
      if (!text.empty())
        return LexerStruct{NAME, text, this->line, this->column};
      this->next();
      while (!util::isNext(this->current) and this->current != EOF)
        this->next();
      continue;
    }
    if (this->current == '\"' or this->current == '\'') {
      if (!text.empty())
        return LexerStruct{NAME, text, this->line, this->column};
      return LexerStruct{STRING, findString(), this->line, this->column};
    }
  _DEFAULT:
    text.push_back(this->current);
    this->next();
  }
  return LexerStruct{_EOF, {}, this->line, this->column};
}