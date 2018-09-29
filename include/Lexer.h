#ifndef KCALC_LEXER_H
#define KCALC_LEXER_H 

#include "Token.h"

#include <memory>  
#include <functional>
#include <string>

#include <boost/iterator/iterator_adaptor.hpp>

namespace kcalc 
{

class TokenIterator 
  : public ::boost::iterator_adaptor<
        TokenIterator, const char *, Token,
        boost::forward_traversal_tag,
        Token >
{
public:
  TokenIterator()
    : TokenIterator::iterator_adaptor_{nullptr},
    m_pos{}, m_invalid{true}
  { }
  
  TokenIterator(
      const char * input) :
      TokenIterator::iterator_adaptor_{input}, m_pos{},
    m_invalid{input == nullptr || *input == 0}
  { }

  TokenIterator(const TokenIterator& other)
    : TokenIterator::iterator_adaptor_{other.base()},
    m_pos{0, 0}, m_invalid{other.m_invalid}
  { }

private:
  struct CurrentToken
  {
    TokenKind      kind;
    unsigned int   length;
    SourcePosition after;
  }; 
  friend class boost::iterator_core_access;
  Token dereference() const; 
  void increment();
  bool equal(const TokenIterator& other) const
  { 
    return (m_invalid && other.m_invalid) ||
      (!m_invalid && !other.m_invalid && 
         base() == other.base());
  }

  std::optional<CurrentToken>& current() const;

  std::optional<CurrentToken> universalMatch(
      TokenKind tokenKind,
      SourcePosition current,
      std::function<bool(char)> matchFirst,
      std::function<bool(char)> matchFurther,
      const char * base = nullptr) const; 
  std::optional<CurrentToken> matchNumberNoDecimal(
    SourcePosition start,
    const char * base = nullptr) const; 
  std::optional<CurrentToken> matchNumber() const; 
  std::optional<CurrentToken> matchIdentifier() const; 
  std::optional<CurrentToken> matchWhitespace() const;  

  SourcePosition m_pos;
  mutable std::optional<CurrentToken> m_current;
  bool m_invalid;
}; 

class Lexer 
{
public:
  Lexer(const char * input) 
    : m_input(input)
  { }

  Lexer(const Lexer&) = delete;
  Lexer& operator=(const Lexer&) = delete;

  TokenIterator begin() 
  { return TokenIterator(m_input); }

  TokenIterator end() 
  { return TokenIterator(); }

private:
  const char * const m_input;
};

} /* namespace kcalc */

#endif // KCALC_LEXER_H
