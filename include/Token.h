#ifndef KCALC_TOKEN_H
#define KCALC_TOKEN_H  

#include <string_view>

namespace kcalc
{

enum class TokenKind : unsigned short
{
#define DEFINE_TOKENKIND(a) \
  a,
#include "TokenKind.h"
#undef DEFINE_TOKENKIND
};

class SourcePosition
{
public:
  constexpr SourcePosition(unsigned short line = 0,
                           unsigned short offset = 0)
    : m_line{ line }, m_offset{ offset }
  { }

  constexpr unsigned short line() const
  { return m_line; }

  constexpr unsigned short offset() const
  { return m_offset; }

  constexpr void nextLine() 
  { ++m_line; m_offset = 0; }

  void operator++()
  { ++m_offset; }

  SourcePosition operator+(unsigned int offset) const
  { return SourcePosition(m_line, m_offset + offset); } 

private:
  unsigned short m_line;
  unsigned short m_offset;
};

class Token
{
public:
  Token(const TokenKind kind, 
        const SourcePosition& pos)
    : m_kind{kind}, m_pos{pos}, m_text()
  { }  
  Token(const TokenKind kind, 
        const SourcePosition& pos,
        const char * ptr,
        unsigned int len)
    : m_kind{kind}, m_pos{pos}, m_text{ptr, len}
  { }

  Token(Token&& original)
    : m_kind { original.m_kind },
      m_pos { original.m_pos },
      m_text { std::move(original.m_text) }
  { }

  Token(const Token&) = default;
  Token& operator=(const Token&) = default;

  TokenKind kind() const  
  { return m_kind; }

  const SourcePosition& position() const 
  { return m_pos; }

  const std::string_view& text() const
  { return m_text; }

private:
  TokenKind        m_kind;
  SourcePosition   m_pos;
  std::string_view m_text;
};  

} /* namespace kcalc */

#endif // KCALC_TOKEN_H
