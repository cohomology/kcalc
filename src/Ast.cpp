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

std::unique_ptr<Expression> ArithmeticExpression::eval() const
{
  assert(m_left && m_right); 
  std::unique_ptr<Expression> left_ptr = m_left->eval();
  std::unique_ptr<Expression> right_ptr = m_right->eval();
  if (left_ptr->kind() == ObjectKind::Number &&
      right_ptr->kind() == ObjectKind::Number)
  {
    const Number * left 
      = static_cast<const Number *>(left_ptr.get());
    const Number * right 
      = static_cast<const Number *>(right_ptr.get()); 
    switch(m_operation)
    {
      case Add:
        return std::make_unique<Number>(
            left->number() + right->number());
      case Subtract:
        return std::make_unique<Number>(
            left->number() - right->number());
      case Multiply:
        return std::make_unique<Number>(
            left->number() * right->number());
      case Divide:
        return std::make_unique<Number>(
            left->number() / right->number());
      case Power:
        return std::make_unique<Number>(
            left->number() ^ right->number());
      case Modulo:
        return std::make_unique<Number>(
            left->number() % right->number());
      default:
        assert(1 == 0);
        break;
    } 
  }
  else
    return std::make_unique<ArithmeticExpression>(
        m_operation, std::move(left_ptr), 
        std::move(right_ptr));
  throw 0;
}

std::string UnaryMinusExpression::to_string() const 
{
  std::string result;
  result.append("- ");
  result.append(inner().to_string());
  return result; 
} 

std::unique_ptr<Expression> UnaryMinusExpression::eval() const 
{
  assert(m_inner);
  std::unique_ptr<Expression> inner_ptr = m_inner->eval(); 
  if (inner_ptr->kind() == ObjectKind::Number)
  {
    const Number * inner 
      = static_cast<const Number *>(inner_ptr.get()); 
    return std::make_unique<Number>(
        ComplexNumber(-1,0) * inner->number());
  }
  else
    return std::make_unique<UnaryMinusExpression>(
        std::move(inner_ptr)); 
}

} // namespace kcalc 
