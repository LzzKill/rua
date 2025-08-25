/*
 * Code: main.cpp
 *
 * @Author LzzKill
 * @License GNU Public License v3.0
 *
 *
 * */
#include <format>
#include <iosfwd>
#include <iostream>
#include <string>
#include <string_view>

import moonlisp.constant;
import moonlisp.lexer;

std::string_view get_string(moonlisp::LexerType lexer_type)
{
  switch (lexer_type) {
  case moonlisp::NUMBER:
    return "NUMBER";
  case moonlisp::NAME:
    return "NAME";
  case moonlisp::STRING:
    return "STRING";
  case moonlisp::SYMBOL:
    return "SYMBOL";
  case moonlisp::_EOF:
    return "_EOF";
  }
}

int main()
{
  std::cout << std::format("Moonlisp VERSION: {} \n", moonlisp::VERSION);
  moonlisp::Lexer *lexer;
  std::string text;
  for (;;) {
    std::getline(std::cin, text);
    if (text == "(exit)" or text == "(quit)")
      return 0;
    lexer = new moonlisp::Lexer(text);
    auto group = lexer->getGroupStruct();
    for (const auto &item : group) {
      std::cout << std::format("Type: {}, Word: {}, Line: {}, Column: {}\n",
                               get_string(item.type), item.word, item.line,
                               item.column);
    }
    delete lexer;
  }
}
