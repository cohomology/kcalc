#ifndef KCALC_LEXER_H
#define KCALC_LEXER_H 

#include "Token.h"

#include <string_view>
#include <optional>

#include <boost/iterator/iterator_adaptor.hpp>

namespace kcalc 
{

class TokenIterator 
  : public ::boost::iterator_adaptor<
        TokenIterator, 
        std::string_view::const_iterator, Token,
        boost::forward_traversal_tag,
        Token>
{
public:
  TokenIterator()
    : TokenIterator::iterator_adaptor_{},
    m_end{}, m_pos{}, m_invalid{true}
  { }
  
  TokenIterator(const std::string_view& view) :
    TokenIterator::iterator_adaptor_{view.begin()}, 
    m_end{view.end()}, m_pos{},
    m_invalid{view.empty()}
  { }

  TokenIterator(const TokenIterator& other)
    : TokenIterator::iterator_adaptor_{other.base()},
    m_end{other.m_end}, m_pos{other.m_pos}, 
    m_invalid{other.m_invalid}
  { }

private:
  struct CurrentToken
  {
    TokenKind kind;
    long length;
    SourcePosition after;
  }; 
  friend class boost::iterator_core_access;
  Token dereference() const; 
  void increment();
  bool equal(const TokenIterator& other) const
  { 
    return (m_invalid && other.m_invalid) ||
      (!m_invalid && !other.m_invalid && 
         base() == other.base() && 
         m_end == other.m_end && 
         m_pos == other.m_pos);
  }

  std::optional<CurrentToken>& current() const;

  std::optional<CurrentToken> 
  matchRegex(TokenKind tokenKind, const char * regex) const; 

  std::string_view::const_iterator m_end;
  SourcePosition m_pos;
  mutable std::optional<CurrentToken> m_current;
  bool m_invalid;
}; 

class Lexer 
{
public:
  Lexer(const std::string_view& input)
    : m_input(input)
  { }

  Lexer(const Lexer&) = delete;
  Lexer& operator=(const Lexer&) = delete;

  TokenIterator begin() const
  { return TokenIterator(m_input); }

  TokenIterator end() const
  { return TokenIterator(); }

private:
  const std::string_view& m_input;
};

} /* namespace kcalc */

#endif // KCALC_LEXER_H
