#ifndef KCALC_LEXER_H
#define KCALC_LEXER_H 
#include <memory> 

#include "Token.h"

namespace kcalc 
{

class Lexer
{
public:
  constexpr Lexer(const char * input,
                  unsigned int length)
    : m_input{input}, m_length{length}, 
    m_pos{}, m_index{0}
  { }

  std::unique_ptr<Token> next();
private:

  std::unique_ptr<Token> matchNumber();
  std::unique_ptr<Token> matchIdentifier();
  std::unique_ptr<Token> matchWhitespace(); 

  const char *   m_input;
  unsigned int   m_length;
  SourcePosition m_pos;
  unsigned int   m_index;
}; 

} /* namespace kcalc */

#endif // KCALC_LEXER_H
