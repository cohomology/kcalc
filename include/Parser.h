#ifndef KCALC_PARSER_H
#define KCALC_PARSER_H 

#include "Lexer.h"

#include <boost/iterator/filter_iterator.hpp>

namespace kcalc 
{

struct IsWhiteSpace 
{
  bool operator()(const Token& token)
  {
    return token.kind() == TokenKind::Whitespace ||
        token.kind() == TokenKind::Newline;
  }
}; 

class Parser
{
public:
  typedef boost::filter_iterator<IsWhiteSpace, TokenIterator>
    IteratorType;

  Parser(const Lexer& lexer) :
    Parser{lexer.begin(), lexer.end()}
  { }

  Parser(const TokenIterator begin,
         const TokenIterator end) :
    m_begin{begin, end}, m_end{end, end}
  { }

private:
  IteratorType m_begin;
  IteratorType m_end;
};

} /* namespace kcalc */

#endif // KCALC_PARSER_H 
