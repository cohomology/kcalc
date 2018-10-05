#include "Exceptions.h"

#include <cassert>

#include <boost/format.hpp>

namespace kcalc
{

std::string ExponentiationOverflow::what() const   
{
  return (boost::format("  Arithmetic overflow: Exponent %1% too large.") 
      % m_exponent).str();
} 

std::string ModuloComplexNumberException::what() const   
{
  return (boost::format("  Arithmetic error: Modulo with complex divisor"
                        " \"%1% %% %2%\" is not yet supported")
           % m_divident % m_divisor).str();
} 

std::string PowerIllegalExponentException::what() const   
{
  static const char * complexExponent = 
      "  Arithmetic error: Power with complex exponent \"%1%\" not yet supported.";  
  static const char * rationalExponent = 
      "  Arithmetic error: Power with rational exponent \"%1%\" not yet supported.";   
  return (boost::format(m_kind == ComplexExponent ? complexExponent :
      rationalExponent) % m_exponent).str();
} 

std::string DivisionByZeroException::what() const   
{
  return "  Arithmetic error: Division by zero.";
} 

std::string IllegalCharacter::what() const   
{
  assert(m_token);
  return (boost::format("  Lexer error: Illegal character: \""
                        "%1%\".") % m_token->text()).str();
} 

std::string IllegalEndOfInput::what() const   
{
  unsigned int line = m_token ? m_token->line() : 0;
  unsigned int offset = m_token ? m_token->offset() +
    m_token->length() : 0;
  std::stringstream stream;
  stream << "  Parse error: Illegal end of input."
         << std::endl;
  stream << "    Expected: { ";
  bool first = true;
  for (auto v : m_expected)
  {
    if (!first)
      stream << ", ";
    else
      first = false;
    stream << v; 
  }
  stream << " }";
  return stream.str();
} 

std::string UnexpectedToken::what() const   
{
  assert(m_token);
  std::stringstream stream;
  stream << "  Parse error: Unexpected token: \""
          << m_token->text() << "\". " << std::endl;
  stream << "    Expected: { ";
  bool first = true;
  for (auto v : m_expected)
  {
    if (!first)
      stream << ", ";
    else
      first = false;
    stream << v; 
  }
  stream << " }";   
  return stream.str();
} 

} /* namespace kcalc */


