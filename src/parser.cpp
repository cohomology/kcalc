#include <string>
#include <memory>

enum class TokenKind : unsigned short
{
  Unknown      = 0u,
  Identifier   = 1u,
  Equals       = 2u,
  Plus         = 3u,
  Minus        = 4u,
  Asterisk     = 5u,
  Slash        = 6u,
  Number       = 7u,
  LeftParen    = 8u,
  RightParen   = 9u,
  LeftBracket  = 10u,
  RightBracket = 11u,

  EndOfInput   = 99u
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
  template<typename InputIterator>
  Token(const TokenKind kind, 
        const SourcePosition& pos,
        InputIterator first,
        InputIterator last)
    : m_kind{kind}, m_pos{pos}, m_text{first, last}
  { }

  Token(Token&& original)
    : m_kind { original.m_kind },
      m_pos { original.m_pos },
      m_text { std::move(original.m_text) }
  { }

  Token(const Token&) = delete;
  Token& operator=(const Token&) = delete;

  TokenKind kind() const  
  { return m_kind; }

  const SourcePosition& position() const 
  { return m_pos; }

  const std::string& text() const
  { return m_text; }

private:
  const TokenKind      m_kind;
  const SourcePosition m_pos;
  const std::string    m_text;
};

class Lexer
{
public:
  constexpr Lexer(const char * input,
                  unsigned int length)
    : m_input{input}, m_length{length}, 
    m_pos{}, m_index{0}
  { }

  std::unique_ptr<Token> next()
  {
    if (m_index >= m_length)
      return std::make_unique<Token>(
          TokenKind::EndOfInput, m_pos);
    switch(m_input[m_index])
    {
      case Identifier:
      case Equals:
      case Plus:
      case Minus:
      case Asterisk:
      case Slash:
      case Number:
      case LeftParen:
      case RightParen:
      case LeftBracket:
      case RightBracket:
    }
  }
private:
  const char *   m_input;
  unsigned int   m_length;
  SourcePosition m_pos;
  unsigned int   m_index;
};
