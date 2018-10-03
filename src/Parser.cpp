
#include "Parser.h"
#include "Exceptions.h"

namespace kcalc
{

std::unique_ptr<AstObject> Parser::parse()
{
  std::unique_ptr<AstObject> object = expression();
  auto la = LA();
  if (la)
    unexpectedToken(*la, { TokenKind::EndOfInput });
  return object;
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
    = powerExpression();
  auto la = LA();
  while(la && (la->kind() == TokenKind::Asterisk ||
               la->kind() == TokenKind::Slash))
  {
    match(la->kind());
    std::unique_ptr<Expression> next 
      = powerExpression(); 
    first = std::make_unique<ArithmeticExpression>(
        la->kind() == TokenKind::Asterisk ? 
          ArithmeticExpression::Multiply :
          ArithmeticExpression::Divide,
          std::move(first), std::move(next));
    la = LA();
  } 
  return first;
} 

std::unique_ptr<Expression> Parser::powerExpression()
{
  std::unique_ptr<Expression> first 
    = unaryMinusExpression();
  auto la = LA();
  if (la && la->kind() == TokenKind::Power) 
  {
    match(TokenKind::Power);
    std::unique_ptr<Expression> next 
      = powerExpression(); 
    first = std::make_unique<ArithmeticExpression>(
        ArithmeticExpression::Power,
        std::move(first), std::move(next)); 
  }
  return first; 
}  

std::unique_ptr<Expression> Parser::unaryMinusExpression()
{
  bool unaryMinus = false;
  auto la = LA();
  switch(la->kind())
  {
    case TokenKind::Minus: 
    {
      match(TokenKind::Minus);
      unaryMinus = true; 
      break;
    }
    case TokenKind::Plus:
      match(TokenKind::Plus);
      break;
    default:
      break;
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
  std::unique_ptr<Expression> expr; 
  if (la)
  {
    switch(la->kind())
    {
      case TokenKind::LeftParen:
      {
        match(TokenKind::LeftParen);
        expr = expression();
        match(TokenKind::RightParen); 
        break;
      }
      case TokenKind::Number:
      {
        match(TokenKind::Number);
        expr = std::make_unique<Number>(la->text());
        break;
      }
      default:
        unexpectedToken(*la, {TokenKind::LeftParen, 
          TokenKind::Number});  
        break;
    }
  }
  if (!expr)
    illegalEndOfInput({TokenKind::LeftParen, 
        TokenKind::Number}); 
  return expr;
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
      unexpectedToken(token, { kind });
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
    const std::vector<TokenKind>& expected)  
{
  throw UnexpectedToken(__FILE__, __LINE__,
          token, expected); 
}

} /* namespace kcalc */
