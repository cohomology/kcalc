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

void Lexer::universalMatch(
  std::function<bool(char)> matchFirst,
  std::function<bool(char)> matchFurther)
{
  if (m_index < m_length && 
      matchFirst(m_input[m_index]))
  {
    while (++m_index < m_length && 
           matchFurther(m_input[m_index]))
    {
      if (m_input[m_index] == '\n') 
        m_pos.nextLine();  
      else
        ++m_pos; 
    }
  }  
}

std::unique_ptr<Token> Lexer::universalMatchReturn(
  TokenKind tokenKind,
  std::function<bool(char)> matchFirst,
  std::function<bool(char)> matchFurther)
{
  std::unique_ptr<Token> token;
  unsigned int startIdx = m_index;   
  universalMatch(matchFirst, matchFurther);
  return startIdx < m_index ? 
    std::make_unique<Token>(tokenKind,
          m_pos, m_input+startIdx, (m_index - startIdx)) :
    nullptr;
} 

void Lexer::matchNumberNoDecimal()
{
  auto matchDigit = [](char c) { return isdigit(c); };
  universalMatch(matchDigit, matchDigit);
}

std::unique_ptr<Token> Lexer::matchNumber()
{
  std::unique_ptr<Token> token;  
  unsigned int startIdx = m_index;  
  matchNumberNoDecimal();
  if (startIdx < m_index)
  {
    universalMatch([](char c) { return c == '.'; },
                   [](char) { return false; });
    matchNumberNoDecimal();
    token =
      std::make_unique<Token>(TokenKind::Number,
          m_pos, m_input+startIdx, (m_index - startIdx)); 
  }
  return token;
}

std::unique_ptr<Token> Lexer::matchIdentifier() 
{
  auto matchAlpha = [](char c) { return isalpha(c); }; 
  auto matchAlphaNum = [](char c) { return isalnum(c); };
  return universalMatchReturn(TokenKind::Number, 
      matchAlpha, matchAlphaNum); 
}

std::unique_ptr<Token> Lexer::matchWhitespace() 
{
  auto matchSpace = [](char c) { return isspace(c); }; 
  return universalMatchReturn(TokenKind::Whitespace, 
      matchSpace, matchSpace);  
} 

} // namespace kcalc


