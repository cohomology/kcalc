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
  DivisionByZero = ExceptionClass::ArithmeticErrorClass + 1u, 
  ModuloComplexNumber = ExceptionClass::ArithmeticErrorClass + 2u,  
  PowerIllegalExponent =  ExceptionClass::ArithmeticErrorClass + 3u,   

  IllegalEndOfInput = ExceptionClass::ParserErrorClass + 0u,
  UnexpectedToken  = ExceptionClass::ParserErrorClass + 1u,
  IllegalCharacter = ExceptionClass::ParserErrorClass + 2u,  
  AssignmentToExpression = ExceptionClass::ParserErrorClass + 3u   
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

class ModuloComplexNumberException : public Exception
{
public:
  ModuloComplexNumberException(
      const char * file,
      unsigned int line,
      std::string divident,
      std::string divisor) :
    Exception(file, line), m_divident{divident},
    m_divisor{divisor}
  { }
  ExceptionClass exceptionClass() const override
  { return ArithmeticErrorClass; }
  ExceptionKind exceptionKind() const override
  { return ExceptionKind::ModuloComplexNumber; }
  std::string what() const override;
  std::string divident() const
  { return m_divident; }
  std::string divisor() const
  { return m_divisor; } 
private:
  std::string m_divident;
  std::string m_divisor;
}; 

class PowerIllegalExponentException : public Exception
{
public:
  enum ErrorKind
  {
    ComplexExponent = 0u,
    RationalExponent = 1u
  };

  PowerIllegalExponentException(
      const char * file,
      unsigned int line,
      ErrorKind kind,
      std::string exponent) :
    Exception(file, line), m_kind{kind}, m_exponent{exponent}
  { }
  ExceptionClass exceptionClass() const override
  { return ArithmeticErrorClass; }
  ExceptionKind exceptionKind() const override
  { return ExceptionKind::PowerIllegalExponent; }
  std::string what() const override;
  std::string exponent() const
  { return m_exponent; }  
  ErrorKind kind() const
  { return m_kind; }
private:
  ErrorKind m_kind;
  std::string m_exponent;
}; 

class DivisionByZeroException : public Exception
{
public:
  DivisionByZeroException(
      const char * file,
      unsigned int line) :
    Exception(file, line)
  { }
  ExceptionClass exceptionClass() const override
  { return ArithmeticErrorClass; }
  ExceptionKind exceptionKind() const override
  { return ExceptionKind::DivisionByZero; }
  std::string what() const override;
private:
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

class AssignmentToExpressionException : public ParseError
{
public:
  AssignmentToExpressionException(
      const char * file,
      unsigned int line,
      const Token& token) : 
    ParseError(file, line, token)
  { } 
  ExceptionKind exceptionKind() const override
  { return ExceptionKind::AssignmentToExpression; } 
  std::string what() const override;  
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
