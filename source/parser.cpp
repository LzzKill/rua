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

using moonlisp::ast::Atom;
using moonlisp::ast::List;
using moonlisp::ast::Node;
using moonlisp::ast::Pair;

moonlisp::Parser::Parser(std::unique_ptr<Lexer> lexer)
    : lexer(std::move(lexer)), lex(nullptr)
{
  this->getNext(); // 得到第一个 token
  while (this->lex and this->lex->type != _EOF) {
    switch (this->lex->type) {
    case SYMBOL:
      if (this->lex->word == "(") // 入口点！！
      {
        this->node.push_back(this->parseList());
      }
      break;
    case _EOF:
      break;
    default:
      throw ParserError(
          this->lex->line, this->lex->column,
          std::format("Fields that shouldn't appear: {}", this->lex->word));
    }
    this->getNext();
  }
}

// private
void moonlisp::Parser::getNext()
{
  try {
    this->lex = this->lexer->getNext();
  }
  catch (LexerError& err) {
    err.show();
  }
}

Node moonlisp::Parser::parseList()
{
  this->getNext();
  auto node = std::make_unique<List>();
  while (this->lex->type != _EOF) {
    if (this->isBracket()) { // 只要看是不是 list 或者 pair 即可
      char a = this->lex->word[0];
      switch (a) {
      case ')':
        return node;
      case '(': { // 子对象
        node->elements.push_back(this->parseList());
        break;
      }
      case '[':
        node->elements.push_back(this->parsePair());
        break;
      case ']':
        throw ParserError(this->lex->line, this->lex->column, "Unmatched ']'");
      }
    } else
      node->elements.push_back(this->parseAtom());
    this->getNext();
  }
  throw ParserError(this->lex->line, this->lex->column,
                    "List not closed with ')'");
  return node;
}

Node moonlisp::Parser::parsePair()
{ // 只处理 pair
  auto node = std::make_unique<Pair>();
  this->getNext();
  while (this->lex->type != _EOF) { // list or pair or end char?
    if (this->isBracket()) {
      char a = this->lex->word[0];
      switch (a) {
      case ']':
        return node;
      case '[': { // 子对象
        node->elements.push_back(this->parsePair());
        break;
      }
      case '(':
        node->elements.push_back(this->parseList());
        break;
      case ')':
        throw ParserError(this->lex->line, this->lex->column, "Unmatched ')'");
      }

    } else
      node->elements.push_back(this->parseAtom());
    this->getNext();
  }
  throw ParserError(this->lex->line, this->lex->column,
                    "Pair not closed with ']'");
  return node;
}

Node moonlisp::Parser::parseAtom()
{
  return Node(std::make_unique<Atom>(
      Atom{ast::getNodeType(this->lex->type), std::move(this->lex->word)}));
}

bool moonlisp::Parser::isBracket()
{
  if (this->lex->word.length() != 1) {
    return false;
  }
  auto c = this->lex->word[0];
  switch (c) {
  case '(':
  case ')':
  case '[':
  case ']':
    return true;
  default:
    return false;
  }
}