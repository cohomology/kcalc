#ifndef KCALC_EXCEPTIONS_H
#define KCALC_EXCEPTIONS_H 

#include <exception>
#include <sstream>

namespace kcalc 
{

enum ExceptionClass : unsigned int
{
  LexerError      = 0u,
  ParserError     = 1000u,
  SemanticError   = 2000u,
  ArithmeticError = 4000u
};

enum class ExceptionKind : unsigned int
{
  ExponentOverflow = ExceptionClass::ArithmeticError + 0u
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
  { return ArithmeticError; }
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

} /* namespace kcalc */

#endif // KCALC_EXCEPTIONS_H  
