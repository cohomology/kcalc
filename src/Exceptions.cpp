#include "Exceptions.h"

#include <cassert>

namespace kcalc
{

std::string ExponentiationOverflow::what() const   
{
  std::stringstream stream;
  stream << "Exponent \"" << m_exponent
    << "\" to large.";
  return stream.str();
} 

std::string IllegalCharacter::what() const   
{
  assert(m_token);
  unsigned int line = m_token->line();
  unsigned int offset = m_token->offset();
  std::stringstream stream;
  stream << "  Lexer error: Illegal character: \""
          << m_token->text() << "\".";
  return stream.str();
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


