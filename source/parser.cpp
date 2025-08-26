/*
 * Code: parser.cpp
 *
 * @Author LzzKill
 * @License BSD4-Clause License
 *
 *
 * */

module;
#include <format>
#include <memory>
#include <utility>
#include <vector>
module moonlisp.parser;

import moonlisp.lexer;
import moonlisp.exception;

moonlisp::Parser::Parser(std::unique_ptr<Lexer> lexer)
    : lexer(std::move(lexer)), lex(nullptr)
{
  this->parse();
}

void moonlisp::Parser::parse()
{
  for (this->lex = nullptr; this->lex->type != _EOF; this->getNext()) {
    switch (this->lex->type) {
    case SYMBOL:
      if (is_bracket<false>()) // 入口点！！
        this->node.push_back(this->parseList());
      break;
    default:
      throw ParseError(
          this->lex->line, this->lex->column,
          std::format("Fields that shouldn't appear: {}", this->lex->word));
    }
  }
  // 如果退出后没有闭合：
  if (!this->count.isEmpty()) {
    throw LexerError(this->lex->line, this->lex->column,
                     "Brackets are not closed");
  }
}

// private
void moonlisp::Parser::getNext()
{
  try {
    this->lex = this->lexer->getNext();
  }
  catch (LexerError err) {
    err.show();
    this->lex = nullptr;
  }
}

moonlisp::Node moonlisp::Parser::parseList()
{
  auto node = std::make_unique<List>();
  for (; this->lex->type != _EOF; this->getNext()) { // 默认情况下，EOF退出；
    if (this->lex->type == SYMBOL) { // 对于剩下的情况，单独处理
      if (is_bracket<true>()) {
        char a = this->lex->word[0];
        switch (a) {
        case ')':
          return node;
        case '(': { // 子对象
          node->elements.push_back(this->parseList());
        }
        case '[':
          node->elements.push_back(this->parsePair());
        case ']':
          throw ParseError(this->lex->line, this->lex->column, "Unmatched ']'");
        }
        continue;
      }
      node->elements.push_back(this->parseAtom());
      continue;
    }
  }
  throw ParseError(this->lex->line, this->lex->column,
                   "List not closed with ')'");
  return node;
}

moonlisp::Node moonlisp::Parser::parseAtom()
{
  return Node(std::make_unique<Atom>(
      Atom{getNodeType(this->lex->type), this->lex->word}));
}

moonlisp::Node moonlisp::Parser::parsePair()
{ // 只处理 pair
  auto node = std::make_unique<Pair>();
  for (; this->lex->type != _EOF; this->getNext()) { // 默认情况下，EOF退出；
    // list or pair or end char?
    if (this->lex->type == SYMBOL) {
      if (is_bracket<true>()) {
        char a = this->lex->word[0];
        switch (a) {
        case ']':
          return node;
        case '[': { // 子对象
          node->elements.push_back(this->parsePair());
        }
        case '(':
          node->elements.push_back(this->parseList());
        case ')':
          throw ParseError(this->lex->line, this->lex->column, "Unmatched ')'");
        }
        continue;
      }
      node->elements.push_back(this->parseAtom());
      continue;
    }
  }
  throw ParseError(this->lex->line, this->lex->column,
                   "Pair not closed with ']'");
  return node;
}

template <bool allMode> bool moonlisp::Parser::is_bracket()
{
  if (this->lex->word.length() != 1) {
    return false;
  }
  auto c = this->lex->word[0];
  if (allMode) {
    switch (c) {
    case '(':
      this->count.min++;
      return true;
    case ')':
      this->count.min--;
      if (this->count.min < 0)
        throw ParserError(this->lex->line, this->lex->column, "Too many ')'");
      return true;
    case '[':
    case ']':
    case '{':
    case '}':
      throw ParserError(this->lex->line, this->lex->column,
                        std::format("Unmatched bracket '{}'", c));
      return false;
    }
  }
  switch (c) {
  // case '{':
  //   this->count.max++;
  //   return true;
  // case '}':
  //   this->count.max--;
  //   if (this->count.max < 0)
  //     throw LexerError(this->lex->line, this->lex->column, "Too many '}'");
  //   return true;
  case '[':
    this->count.mid++;
    return true;
  case ']':
    this->count.mid--;
    if (this->count.mid < 0)
      throw LexerError(this->lex->line, this->lex->column, "Too many ']'");
    return true;
  case '(':
    this->count.min++;
    return true;
  case ')':
    this->count.min--;
    if (this->count.min < 0)
      throw LexerError(this->lex->line, this->lex->column, "Too many ')'");
    return true;
  }

  return false;
}

bool moonlisp::Parser::isAtom()
{
  switch (this->lex->type) {
  case NAME:
  case NUMBER:
  case FLOAT:
  case STRING:
    return true;
  default:
    return false;
  }
}