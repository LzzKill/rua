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
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#include <iostream>
module moonlisp.parser;

import moonlisp.lexer;
import moonlisp.exception;

using moonlisp::ast::Atom;
using moonlisp::ast::List;
using moonlisp::ast::Node;
using moonlisp::ast::Pair;

void moonlisp::Parser::parse()
{
  this->getNext(); // 得到第一个 token
  while (this->lex and this->lex->type != _EOF) {
    switch (this->lex->type) {
    case SYMBOL:
      std::visit(
          [&](std::string &args) {
            if (args == "(")
              this->parseList();
          },
          this->lex->word);
      break;
    case _EOF:
      break;
    default:
      auto s = std::visit(
          [&](auto &&a) -> std::string {
            using T = std::decay_t<decltype(a)>;
            if constexpr (std::is_same_v<T, int> or std::is_same_v<T, double>)
              return std::to_string(a);
            else // std::string
              return a;
          },
          this->lex->word);
      throw ParserError(this->lex->place,
                        std::format("Fields that shouldn't appear: {}", s));
    }
    this->getNext();
  }
}

void moonlisp::Parser::getNext()
{
  try {
    this->lex = this->lexer->getNext();
  }
  catch (const LexerError &err) {
    std::cerr << err.what() << '\n'; 
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