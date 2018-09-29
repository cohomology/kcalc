enum class TokenKind : unsigned short
{
#define TOKEN_KIND
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
