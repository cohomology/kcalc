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
  LexerErrorClass      = 0u,
  ParserErrorClass     = 1000u,
  SemanticErrorClass   = 2000u,
  ArithmeticErrorClass = 4000u
};

enum class ExceptionKind : unsigned int
{
  ExponentOverflow = ExceptionClass::ArithmeticErrorClass + 0u,

  IllegalEndOfInput = ExceptionClass::ParserErrorClass + 0u,
  UnexpectedToken  = ExceptionClass::ParserErrorClass + 1u
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
  std::string what() const override  
  {
    std::stringstream stream;
    stream << "Exponent \"" << m_exponent
           << "\" to large.";
    return stream.str();
  }
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
  std::string what() const override  
  {
    unsigned int line = m_token ? m_token->line() : 0;
    unsigned int offset = m_token ? m_token->offset() +
      m_token->length() : 0;
    std::stringstream stream;
    stream << "Parse error: Illegal end of input at " 
           << "line \"" << line << "\" "
           << "offset \"" << offset
           << "."  << std::endl;
    stream << " Expected: { ";
    bool first = true;
    for (auto v : m_expected)
    {
      if (!first)
        stream << ", ";
      else
        first = false;
      stream << v; 
    }
    stream << "}";
    return stream.str();
  }
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
      const TokenKind expected) : 
    ParseError(file, line, token), m_expected{expected}
  { }
  
  ExceptionKind exceptionKind() const override
  { return ExceptionKind::UnexpectedToken; }
  std::string what() const override  
  {
    assert(m_token);
    std::stringstream stream;
    stream << "Parse error: Unexpected token: \""
           << *m_token << ". Expected was: \"" 
           << m_expected << "\"."; 
    return stream.str();
  }
private:
  TokenKind m_expected;
}; 

} /* namespace kcalc */

#endif // KCALC_EXCEPTIONS_H  
