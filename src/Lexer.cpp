#include "Lexer.h"

std::unique_ptr<Token> Lexer::next()
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


