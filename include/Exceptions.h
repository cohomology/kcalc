#ifndef KCALC_EXCEPTIONS_H
#define KCALC_EXCEPTIONS_H 

#include <exception>
#include <sstream>
#include <optional>
#include <vector>

#include "Token.h"

namespace kcalc 
{

enum ExceptionClass : unsigned int
{
  ParserErrorClass     = 0u,
  SemanticErrorClass   = 1000u,
  ArithmeticErrorClass = 2000u
};

enum class ExceptionKind : unsigned int
{
  ExponentOverflow = ExceptionClass::ArithmeticErrorClass + 0u,

  IllegalEndOfInput = ExceptionClass::ParserErrorClass + 0u,
  UnexpectedToken  = ExceptionClass::ParserErrorClass + 1u,
  IllegalCharacter = ExceptionClass::ParserErrorClass + 2u,  
};

class Exception
{
public:
  Exception(const char * file, 
            unsigned int line)
    : m_file{file}, m_line{line}
  { }
  virtual std::string what() const = 0;
  virtual ExceptionClass exceptionClass() const = 0;
  virtual ExceptionKind exceptionKind() const = 0;
  const char * file() const
  { return m_file; }
  unsigned int line() const
  { return m_line; }
private:
  const char * m_file;
  unsigned int m_line;
};

class ExponentiationOverflow : public Exception
{
public:
  ExponentiationOverflow(
      const char * file,
      unsigned int line,
      const std::string_view& exponent) :
    Exception(file, line),
    m_exponent{exponent.begin(), exponent.end()}
  { }
  ExceptionClass exceptionClass() const override
  { return ArithmeticErrorClass; }
  ExceptionKind exceptionKind() const override
  { return ExceptionKind::ExponentOverflow; }
  std::string what() const override;
  std::string exponent() const
  { return m_exponent; }
private:
  const std::string m_exponent;
};

class ParseError : public Exception
{
public:
  ParseError(
      const char * file,
      unsigned int line,
      std::optional<Token> token = std::optional<Token>()) :
    Exception(file, line),
    m_token{token}
  { } 
  ExceptionClass exceptionClass() const override
  { return ParserErrorClass; } 
  std::optional<Token> token() const
  { return m_token; }
protected:
  std::optional<Token> m_token;
};

class IllegalCharacter : public ParseError
{
public:
  IllegalCharacter(
      const char * file,
      unsigned int line,
      const Token& token) : 
    ParseError(file, line, token)
  { } 
  ExceptionKind exceptionKind() const override
  { return ExceptionKind::IllegalCharacter; } 
  std::string what() const override;  
}; 

class IllegalEndOfInput : public ParseError
{
public:
  IllegalEndOfInput(
      const char * file,
      unsigned int line,
      std::optional<Token> token,
      const std::vector<TokenKind>& expected) : 
    ParseError(file, line, token), m_expected{expected}
  { }
  ExceptionKind exceptionKind() const override
  { return ExceptionKind::IllegalEndOfInput; }
  std::string what() const override;  
private:
  const std::vector<TokenKind> m_expected; 
};

class UnexpectedToken : public ParseError
{
public:
  UnexpectedToken(
      const char * file,
      unsigned int line,
      const Token& token, 
      const std::vector<TokenKind>& expected) :  
    ParseError(file, line, token), m_expected{expected}
  { }
  
  ExceptionKind exceptionKind() const override
  { return ExceptionKind::UnexpectedToken; }
  std::string what() const override;  
private:
  const std::vector<TokenKind> m_expected;  
}; 

} /* namespace kcalc */

#endif // KCALC_EXCEPTIONS_H  
