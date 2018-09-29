#include "Lexer.h"

#include <cctype>

namespace kcalc
{

Token TokenIterator::dereference() const
{
  Token token(TokenKind::EndOfInput, m_pos, base(), 0);
  if (!m_invalid)
  {
    auto current = this->current();
    if (current)
      token = Token(current->kind, m_pos,
          this->base_reference(), current->length);
    else
      token = Token(TokenKind::Unknown, m_pos,
          this->base_reference(), 1);
  }
  return token;
}  

void TokenIterator::increment() 
{
  if (!m_invalid)
  {
    auto current = this->current();
    if (current)
    {
      m_pos = current->after;
      base_reference() += current->length;
      m_current.reset();
      if (*base() == 0) 
        m_invalid = true;
    }
    else
      m_invalid = true;
  }
}

std::optional<TokenIterator::CurrentToken>& 
TokenIterator::current() const
{
  if (!m_current)
  {
    switch(*base())
    {
#define DEFINE_TOKENKIND_SIMPLE(kind,chr)               \
      case chr:                                         \
      {                                                 \
        m_current = CurrentToken { TokenKind::kind, 1,  \
          m_pos + 1 };                                  \
      }
#define DEFINE_TOKENKIND_MANUAL(kind)
#define DEFINE_TOKENKIND_COMPLEX(kind,fn)        
#include "TokenKind.h"
#undef DEFINE_TOKENKIND_SIMPLE
#undef DEFINE_TOKENKIND_MANUAL
#undef DEFINE_TOKENKIND_COMPLEX
      default:
      {
#define DEFINE_TOKENKIND_SIMPLE(kind,chr)
#define DEFINE_TOKENKIND_MANUAL(kind)
#define DEFINE_TOKENKIND_COMPLEX(kind,fn)   \
        m_current = fn();                   \
        if (m_current)                      \
          break;
#include "TokenKind.h"
#undef DEFINE_TOKENKIND_SIMPLE
#undef DEFINE_TOKENKIND_MANUAL
#undef DEFINE_TOKENKIND_COMPLEX 
        break;
      }
    }  
  }
  return m_current;
}

std::optional<TokenIterator::CurrentToken> 
TokenIterator::universalMatch(
  TokenKind tokenKind,
  SourcePosition current,
  std::function<bool(char)> matchFirst,
  std::function<bool(char)> matchFurther) const
{
  std::optional<CurrentToken> token;
  const char * it = base();
  if (it != nullptr && 
      *it != 0 &&
      matchFirst(*it))
  {
    token = CurrentToken { tokenKind, 1, 
      current + 1 };
    while (*(++it) != 0 &&
           matchFurther(*it))
    {
      if (*it == '\n') 
        token->after.nextLine();
      else if (*it != '\r')
        ++token->after; 
      ++token->length;
    }
  }  
  return token;
}

std::optional<TokenIterator::CurrentToken> 
TokenIterator::matchNumberNoDecimal(
    SourcePosition start) const
{
  auto matchDigit = [](char c) { return isdigit(c); };
  return universalMatch(TokenKind::Number, start, matchDigit, 
      matchDigit);
}

std::optional<TokenIterator::CurrentToken> 
TokenIterator::matchNumber() const
{
  auto token = matchNumberNoDecimal(m_pos);
  if (token)
  {
    auto decimal = 
      universalMatch(TokenKind::Number, token->after, 
                     [](char c) { return c == '.'; },
                     [](char) { return false; });
    if (decimal)
    {
      auto afterDecimal =
        matchNumberNoDecimal(decimal->after); 
      token = CurrentToken { TokenKind::Number,
        token->length + 1 + 
          (afterDecimal ? afterDecimal->length : 0),
        afterDecimal ? afterDecimal->after :
          decimal->after };
    }
  }
  return token;
}

std::optional<TokenIterator::CurrentToken> 
TokenIterator::matchIdentifier() const 
{
  auto matchAlpha = [](char c) { return isalpha(c); }; 
  auto matchAlphaNum = [](char c) { return isalnum(c); };
  return universalMatch(TokenKind::Identifier, m_pos,
      matchAlpha, matchAlphaNum); 
}

std::optional<TokenIterator::CurrentToken> 
TokenIterator::matchWhitespace() const
{
  auto matchSpace = [](char c) { return isspace(c); }; 
  return universalMatch(TokenKind::Whitespace, m_pos, 
      matchSpace, matchSpace);  
} 

} // namespace kcalc


