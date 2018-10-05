#ifndef KCALC_TOKEN_H
#define KCALC_TOKEN_H  

#include <string_view>
#include <ostream>

namespace kcalc
{

enum class TokenKind : unsigned short
{
#define DEFINE_TOKENKIND(a) \
  a,
#include "TokenKind.h"
#undef DEFINE_TOKENKIND
};

inline std::ostream& operator<<(std::ostream& out, TokenKind kind)
{ 
  switch(kind)
  {
#define DEFINE_TOKENKIND(a) \
    case TokenKind::a:      \
      out << #a;            \
      break;
#include "TokenKind.h"
#undef DEFINE_TOKENKIND
  }
  return out;
} 

class SourcePosition
{
public:
  constexpr SourcePosition(unsigned short line = 1,
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

  SourcePosition& operator+=(unsigned int offset) 
  { 
    m_offset += offset;
    return *this;
  }  
  
  friend std::ostream& operator<<(std::ostream& out, 
                                  const SourcePosition& pos)
  { 
    out << "( line = " << pos.m_line << ", " 
        << "offset = " << pos.m_offset << ")"; 
    return out;
  }

  bool operator==(const SourcePosition& other) const 
  { 
    return m_line == other.m_line &&
        m_offset == other.m_offset; 
  }

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
        const std::string_view& view)
    : m_kind{kind}, m_pos{pos}, m_text{view}
  { }

  Token(const Token&) = default;
  Token(Token&&) = default; 
  Token& operator=(const Token&) = default;

  TokenKind kind() const  
  { return m_kind; }

  const SourcePosition& position() const 
  { return m_pos; }

  const std::string_view& text() const
  { return m_text; }

  friend std::ostream& operator<<(std::ostream& out, 
                                  const Token& token)
  { 
    out << "( kind =" << token.m_kind << ", "
        << "pos = " << token.m_pos << ", " 
        << "text = \"" << token.m_text << "\")";
    return out;
  } 

  constexpr unsigned short line() const
  { return m_pos.line(); }

  constexpr unsigned short offset() const
  { return m_pos.offset(); } 

  constexpr unsigned short length() const
  { return m_text.length(); }  

  bool operator==(const Token& other) const
  { 
    return m_kind == other.m_kind &&
      m_pos == other.m_pos &&
      m_text == other.m_text;
  }

private:
  TokenKind        m_kind;
  SourcePosition   m_pos;
  std::string_view m_text;
};  

} /* namespace kcalc */

#endif // KCALC_TOKEN_H
