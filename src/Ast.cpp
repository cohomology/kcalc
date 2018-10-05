#include "Ast.h"
#include "Exceptions.h"

#include <cassert>

namespace kcalc
{

std::string Assignment::to_string() const
{
  std::string result;
  result.append(left().to_string());
  result.append(" = ");
  result.append(right().to_string());
  return result;
}

std::string ArithmeticExpression::to_string() const 
{
  std::string result;
  bool leftBracket = !left().isAtomicExpression();
  bool rightBracket = !right().isAtomicExpression();
  if (leftBracket)
    result.append("( ");
  result.append(left().to_string());
  if (leftBracket)
    result.append(" )");
  switch(m_operation)
  {
    case Add:
      result.append(" + ");
      break;
    case Subtract:
      result.append(" - ");
      break; 
    case Multiply:
      result.append(" * ");
      break; 
    case Divide:
      result.append(" / ");
      break; 
    case Power:
      result.append(" ^ ");
      break; 
    case Modulo:
      result.append(" % ");
      break;  
    default:
      assert(1 == 0);
      break;
  }
  if (rightBracket)
    result.append("( ");
  result.append(right().to_string());
  if (rightBracket)
    result.append(" )");
  return result; 
}

ComplexNumber ArithmeticExpression::eval() const
{
  assert(m_left && m_right); 
  ComplexNumber left = m_left->eval();
  ComplexNumber right = m_right->eval();
  switch(m_operation)
  {
    case Add:
      return left + right;
    case Subtract:
      return left - right;
    case Multiply:
      return left * right;
    case Divide:
      return left / right;
    case Power:
      return left ^ right;
    case Modulo:
      return left % right;
    default:
      assert(1 == 0);
      break;
  } 
  throw 0;
}

std::string UnaryMinusExpression::to_string() const 
{
  std::string result;
  result.append("- ");
  result.append(inner().to_string());
  return result; 
} 

ComplexNumber UnaryMinusExpression::eval() const 
{
  assert(m_inner);
  ComplexNumber inner = m_inner->eval();
  inner *= ComplexNumber(-1, 0);
  return inner;
}

} // namespace kcalc 
