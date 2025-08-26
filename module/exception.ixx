/*
 * Code: exception.ixx
 *
 * @Author LzzKill
 * @License GNU Public License v3.0
 *
 *
 * */

module;
#include <exception>
#include <format>
#include <iostream>
#include <string>
export module moonlisp.exception;

export namespace moonlisp
{
  class LexerError : std::exception
  {
    private:
    std::string message;

    public:
    LexerError(unsigned int line, unsigned int column, std::string message)
        : message(std::format("{}, in Line: {}, Column: {}", message, line,
                              column)) { };
    void show() { std::cerr << this->message << '\n'; };
    const char *what() const override { return message.c_str(); };
  };
  class ParseError : std::exception
  {
    private:
    std::string message;

    public:
    ParseError(unsigned int line, unsigned int column, std::string message)
        : message(std::format("{}, in Line: {}, Column: {}", message, line,
                              column)) { };
    void show() { std::cerr << this->message << '\n'; };
    const char *what() const override { return message.c_str(); };
  };
} // namespace moonlisp