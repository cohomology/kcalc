#ifndef KCALC_LEXER_H
#define KCALC_LEXER_H 

#include "Token.h"

#include <memory>  
#include <functional>
#include <string>

#include <boost/iterator/iterator_adaptor.hpp>

namespace kcalc 
{

class Lexer 
  : public ::boost::iterator_adaptor<
        Lexer, const char *, Token,
        boost::forward_traversal_tag,
        Token >
{
public:
  Lexer()
    : Lexer::iterator_adaptor_{nullptr},
    m_pos{}
  { }
  
  Lexer(
      const char * input) :
      Lexer::iterator_adaptor_{input}, m_pos{}
  { }

  Lexer(const Lexer& other)
    : Lexer::iterator_adaptor_{other.base()} 
  { }

  struct CurrentToken
  {
    TokenKind      kind;
    unsigned int   length;
    SourcePosition after;
  };    

private:
  friend class boost::iterator_core_access;
  Token dereference() const; 
  void increment();

  std::optional<CurrentToken>& current() const;

  std::optional<CurrentToken> universalMatch(
      TokenKind tokenKind,
      SourcePosition current,
      std::function<bool(char)> matchFirst,
      std::function<bool(char)> matchFurther) const; 
  std::optional<CurrentToken> matchNumberNoDecimal(
    SourcePosition start) const; 
  std::optional<CurrentToken> matchNumber() const; 
  std::optional<CurrentToken> matchIdentifier() const; 
  std::optional<CurrentToken> matchWhitespace() const;  

  SourcePosition m_pos;
  mutable std::optional<CurrentToken> m_current;
}; 

} /* namespace kcalc */

#endif // KCALC_LEXER_H
