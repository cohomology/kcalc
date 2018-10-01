#ifndef KCALC_PARSER_H
#define KCALC_PARSER_H 

#include <memory>
#include <vector>

#include "Lexer.h"
#include "Ast.h"

#include <boost/iterator/filter_iterator.hpp>

namespace kcalc 
{

struct IsNoWhiteSpace 
{
  bool operator()(const Token& token)
  {
    return token.kind() != TokenKind::Whitespace &&
        token.kind() != TokenKind::Newline;
  }
}; 

class Parser
{
public:
  typedef boost::filter_iterator<IsNoWhiteSpace, TokenIterator>
    IteratorType;

  Parser(const Lexer& lexer) :
    Parser{lexer.begin(), lexer.end()} 
  { }

  Parser(const TokenIterator begin,
         const TokenIterator end) :
    m_begin{begin, end}, m_end{end, end}, 
    m_last{}
  { }

  std::unique_ptr<AstObject> parse(); 

protected:
  void match(TokenKind kind);
  std::optional<Token> LA();
  std::unique_ptr<Expression> expression(); 
  std::unique_ptr<Expression> multiplicativeExpression(); 
  std::unique_ptr<Expression> unaryMinusExpression(); 
  std::unique_ptr<Expression> atomicExpression(); 
  std::unique_ptr<Expression> powerExpression(); 
  [[noreturn]] void illegalEndOfInput(
      const std::vector<TokenKind>& expected);
  [[noreturn]] void unexpectedToken(const Token& token, 
      const TokenKind expected);
  [[noreturn]] void illegalToken(const Token& token); 
private:
  IteratorType         m_begin;
  IteratorType         m_end;
  std::optional<Token> m_last;
};

} /* namespace kcalc */

#endif // KCALC_PARSER_H 
