#include "Lexer.h"

#include <cctype>

namespace kcalc
{

std::unique_ptr<Token> Lexer::next()
{
  if (m_index >= m_length)
    return std::make_unique<Token>(
        TokenKind::EndOfInput, m_pos);
  switch(m_input[m_index])
  {
#define DEFINE_TOKENKIND_SIMPLE(kind,chr)  \
    case chr:                              \
    {                                      \
      std::unique_ptr<Token> token =       \
          std::make_unique<Token>(         \
              TokenKind::kind, m_pos,      \
              m_input+m_index, 1);         \
      ++m_index;                           \
      ++m_pos;                             \
      break;                               \
    }
#define DEFINE_TOKENKIND_MANUAL(kind)
#define DEFINE_TOKENKIND_COMPLEX(kind,fn)        
#include "TokenKind.h"
#undef DEFINE_TOKENKIND_SIMPLE
#undef DEFINE_TOKENKIND_MANUAL
#undef DEFINE_TOKENKIND_COMPLEX
    default:
    {
      std::unique_ptr<Token> token; 
#define DEFINE_TOKENKIND_SIMPLE(kind,chr)
#define DEFINE_TOKENKIND_MANUAL(kind)
#define DEFINE_TOKENKIND_COMPLEX(kind,fn) \
      token = fn();                       \
      if (token)                          \
        return token;                     
#include "TokenKind.h"
#undef DEFINE_TOKENKIND_SIMPLE
#undef DEFINE_TOKENKIND_MANUAL
#undef DEFINE_TOKENKIND_COMPLEX 
      break;
    }
  }
  return std::make_unique<Token>(
      TokenKind::Unknown, m_pos); 
}  

std::unique_ptr<Token> Lexer::matchNumber()
{
  return std::make_unique<Token>(
      TokenKind::Unknown, m_pos);  
}

std::unique_ptr<Token> Lexer::matchIdentifier() 
{
  if (isalpha(m_input[m_index]))
  {
    unsigned int startIdx = m_index;
    while (++m_index < m_length && 
           isalnum(m_input[m_index]));
    std::unique_ptr<Token> token =
      std::make_unique<Token>(TokenKind::Identifier,
          m_pos, m_input+startIdx, (m_index - startIdx));
    m_pos.advance(m_index - startIdx);
    return token;
  } 
  else
    return nullptr;
}

std::unique_ptr<Token> Lexer::matchWhitespace() 
{
  return std::make_unique<Token>(
      TokenKind::Unknown, m_pos);  
} 

} // namespace kcalc


