#include "Ast.h"
#include "Exceptions.h"

#include <cassert>
#include <iterator>

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
    result.append(" ( ");
  result.append(left().to_string());
  if (leftBracket)
    result.append(" ) ");
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
    result.append(" ( ");
  result.append(right().to_string());
  if (rightBracket)
    result.append(" ) ");
  return result; 
}

std::string UnaryMinusExpression::to_string() const 
{
  std::string result;
  result.append(" - ");
  result.append(inner().to_string());
  return result; 
} 

} // namespace kcalc 
