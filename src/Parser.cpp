
#include "Parser.h"
#include "Exceptions.h"

namespace kcalc
{

std::unique_ptr<AstObject> Parser::parse()
{
  return expression();
}

std::unique_ptr<Expression> Parser::expression()
{
  std::unique_ptr<Expression> first 
    = multiplicativeExpression();
  auto la = LA();
  while(la && (la->kind() == TokenKind::Plus ||
               la->kind() == TokenKind::Minus))
  {
    match(la->kind());
    std::unique_ptr<Expression> next 
      = multiplicativeExpression(); 
    first = std::make_unique<ArithmeticExpression>(
        la->kind() == TokenKind::Plus ? 
          ArithmeticExpression::Add :
          ArithmeticExpression::Subtract,
          std::move(first), std::move(next));
    la = LA();
  }
  return first;
}

std::unique_ptr<Expression> Parser::multiplicativeExpression()
{
  std::unique_ptr<Expression> first 
    = unaryMinusExpression();
  auto la = LA();
  while(la && (la->kind() == TokenKind::Asterisk ||
               la->kind() == TokenKind::Slash))
  {
    match(la->kind());
    std::unique_ptr<Expression> next 
      = unaryMinusExpression(); 
    first = std::make_unique<ArithmeticExpression>(
        la->kind() == TokenKind::Asterisk ? 
          ArithmeticExpression::Multiply :
          ArithmeticExpression::Divide,
          std::move(first), std::move(next));
    la = LA();
  } 
  return first;
} 

std::unique_ptr<Expression> Parser::unaryMinusExpression()
{
  bool unaryMinus = false;
  auto la = LA();
  if (la->kind() == TokenKind::Minus)
  {
    match(TokenKind::Minus);
    unaryMinus = true;
  }
  std::unique_ptr<Expression> expr 
    = atomicExpression();
  if (unaryMinus)
    expr = std::make_unique<UnaryMinusExpression>(
        std::move(expr));  
  return expr;
} 

std::unique_ptr<Expression> Parser::atomicExpression()
{
  auto la = LA();
  if (!la)
    illegalEndOfInput({TokenKind::LeftParen, 
        TokenKind::Number});
  switch(la->kind())
  {
    case TokenKind::LeftParen:
    {
      match(TokenKind::LeftParen);
      std::unique_ptr<Expression> expr = 
        expression();
      match(TokenKind::RightParen); 
      return expr;
    }
    case TokenKind::Number:
    {
      return nullptr;
    }
    default:
      break;
  }
  illegalEndOfInput({TokenKind::LeftParen, 
        TokenKind::Number}); 
} 

void Parser::match(TokenKind kind) 
{
  if (m_begin == m_end)
    illegalEndOfInput({kind});
  else
  {
    Token token = *m_begin;
    if (token.kind() == kind)
    {
      m_last = token; 
      ++m_begin; 
    }
    else
      unexpectedToken(token, kind);
  }
}

std::optional<Token> Parser::LA()
{
  return m_begin != m_end ? 
    *m_begin : std::optional<Token>();
}

void Parser::illegalEndOfInput( 
    const std::vector<TokenKind>& expected) 
{
  throw IllegalEndOfInput(__FILE__, __LINE__, m_last,
      expected);
}

void Parser::unexpectedToken(const Token& token, 
      const TokenKind expected)
{
  throw UnexpectedToken(__FILE__, __LINE__,
          token, expected); 
}

} /* namespace kcalc */
