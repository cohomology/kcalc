#include "Lexer.h"

#include <regex>
#include <cassert>

namespace kcalc
{

Token TokenIterator::dereference() const
{
  Token token(TokenKind::EndOfInput, m_pos, std::string_view());
  if (!m_invalid)
  {
    auto current = this->current();
    if (current)
      token = Token(current->kind, m_pos, 
          std::string_view(&*base(), current->length));
    else if (base() != m_end)
    {
      token = Token(TokenKind::Unknown, m_pos,
          std::string_view(this->base(), 1));
    }
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
      if (base() == m_end) 
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
    assert(base() != m_end);
    switch(*base())
    {
#define DEFINE_TOKENKIND_SIMPLE(kind,chr)               \
      case chr:                                         \
      {                                                 \
        m_current = CurrentToken { TokenKind::kind, 1,  \
          m_pos + 1 };                                  \
        break;                                          \
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
#define DEFINE_TOKENKIND_COMPLEX(kind,rx)             \
        m_current = matchRegex(TokenKind::kind, rx);  \
        if (m_current)                                \
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
TokenIterator::matchRegex(TokenKind tokenKind,
    const char * regex) const
{
  assert(base() != m_end);
  std::optional<TokenIterator::CurrentToken> token;  
  std::match_results<std::string_view::const_iterator> match; 
  if (std::regex_search(base(), m_end, match,  std::regex(regex), 
      std::regex_constants::match_continuous)) 
  { 
    SourcePosition after = m_pos;
    if (tokenKind == TokenKind::Newline)
      after.nextLine();
    else
      after += match.length(); 
    token = CurrentToken { tokenKind, match.length(),
      after };
  } 
  return token;
}

} // namespace kcalc
