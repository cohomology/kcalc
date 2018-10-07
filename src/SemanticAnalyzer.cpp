#include "SemanticAnalyzer.h"
#include "Ast.h"
#include "SymbolTable.h" 

#include <numeric>

namespace kcalc
{ 

void SemanticAnalyzer::visit(Assignment& assignment) 
{
  assert(assignment.left().kind() == ObjectKind::Variable);
  const Variable& var 
    = static_cast<const Variable &>(assignment.left());
  m_symbolTable.insert(std::string(var.name()), 
      assignment.right());
}

std::unique_ptr<Expression> 
SemanticAnalyzer::evaluate(const Expression& expr)
{
  return expr.containsVariables() ? nullptr : 
      expr.eval(m_symbolTable);
}

static bool isPlusMinus(
    const ArithmeticExpression& expression)
{
  return expression.operation() == ArithmeticExpression::Add ||
    expression.operation() == ArithmeticExpression::Subtract; 
}

static void invertPlusMinus(
    ArithmeticExpression::Operation& op)
{
  switch(op)
  {
    case ArithmeticExpression::Add:
      op = ArithmeticExpression::Subtract;
      break;
    case ArithmeticExpression::Subtract:
      op = ArithmeticExpression::Add;
      break;
    default:
      assert(1 == 0);
      break;
  }
}

static bool optimizePlusSetOperation(
    const Expression& expression,
    const Expression *& slot1,
    const Expression *& slot2,
    ArithmeticExpression::Operation& op)
{
  bool success = true;
  switch(expression.kind())
  {
    case ObjectKind::ArithmeticExpression:
    {
      auto& inner = 
        static_cast<const ArithmeticExpression&>(expression);
      slot1 = &inner.left();
      slot2 = &inner.right();
      op = inner.operation();
      success = isPlusMinus(inner);
      break;
    }
    case ObjectKind::UnaryMinus:
    {
      auto& inner = 
        static_cast<const UnaryMinusExpression&>(expression); 
      slot1 = nullptr; 
      slot2 = &inner.inner();
      op = ArithmeticExpression::Subtract;
      break;
    }
    case ObjectKind::Number:
    {
      slot1 = &expression;
      slot2 = nullptr;
      op = ArithmeticExpression::Add;
      break;
    }
    default:
      success = false;
      break;
  } 
  return success;
}

struct BalancePlusMinus
{
  const Expression * m_expr;
  ArithmeticExpression::Operation m_sign;

  BalancePlusMinus() :
    m_expr{nullptr}, m_sign{ArithmeticExpression::Add}
  { }
};

static std::unique_ptr<Expression> createMinusExpression(
    const Expression& expr)
{
  if (expr.kind() == ObjectKind::UnaryMinus)
  {
    auto& unary = 
      static_cast<const UnaryMinusExpression&>(expr);
    return unary.inner().cloneExpression();
  }
  else
    return std::make_unique<UnaryMinusExpression>(
        std::move(expr.cloneExpression()));
} 

static std::unique_ptr<Expression> createArithmeticExpression(
    const BalancePlusMinus& first,
    const BalancePlusMinus& second)
{
  if (first.m_expr == nullptr)
  {
    assert(second.m_expr != nullptr);
    return second.m_sign ? createMinusExpression(
        *second.m_expr) : second.m_expr->cloneExpression();
  }
  else if (second.m_expr == nullptr)
  {
    assert(first.m_expr != nullptr);
    return first.m_sign ? createMinusExpression(
        *first.m_expr) : first.m_expr->cloneExpression(); 
  }
  else
    return std::make_unique<ArithmeticExpression>(
      second.m_sign, 
      first.m_sign == ArithmeticExpression::Subtract ? 
        std::move(createMinusExpression(*first.m_expr)) :
        std::move(first.m_expr->cloneExpression()),
        std::move(second.m_expr->cloneExpression())); 
}

void SemanticAnalyzer::balanceVariablesPlusMinus(
    ArithmeticExpression& expression)
{
  std::array<BalancePlusMinus, 4> exprs;
  if (isPlusMinus(expression) &&
      optimizePlusSetOperation(expression.left(), 
        exprs[0].m_expr, exprs[1].m_expr, exprs[1].m_sign) &&
      optimizePlusSetOperation(expression.right(), exprs[2].m_expr,
        exprs[3].m_expr, exprs[3].m_sign))
  {
    exprs[2].m_sign = expression.operation();
    if (exprs[2].m_sign == ArithmeticExpression::Subtract)
      invertPlusMinus(exprs[3].m_sign);
    unsigned int numberOfVariables =
      std::accumulate(exprs.begin(), exprs.end(), 0,  
      [](int accumulator, const BalancePlusMinus& e) -> int {
        return accumulator + ( e.m_expr != nullptr && e.m_expr->kind() != ObjectKind::Number ) ? 1 : 0;
      });
    if (1 <= numberOfVariables && numberOfVariables <= 2)
    {
      std::stable_sort(exprs.begin(), exprs.end(), [](auto& op1, auto& op2) {
        return (((op1.m_expr != nullptr && op1.m_expr->kind() != ObjectKind::Number) &&
                (op2.m_expr == nullptr || op2.m_expr->kind() == ObjectKind::Number)) ||
                (op1.m_expr == nullptr && op2.m_expr != nullptr));
      });
      expression.replaceLeft(createArithmeticExpression(exprs[0], exprs[1]));
      if (exprs[2].m_sign == ArithmeticExpression::Subtract &&
          exprs[3].m_sign == ArithmeticExpression::Subtract) 
      {
        expression.operation(ArithmeticExpression::Subtract); 
        exprs[2].m_sign = ArithmeticExpression::Add;
        exprs[3].m_sign = ArithmeticExpression::Add;
      }
      else
        expression.operation(ArithmeticExpression::Add);
      std::unique_ptr<Expression> newRight 
        = createArithmeticExpression(exprs[2], exprs[3])->eval(m_symbolTable); 
      expression.replaceRight(std::move(newRight));
    }
  }
}

void SemanticAnalyzer::visit(
    ArithmeticExpression& expression)
{
  std::unique_ptr<Expression> left 
    = evaluate(expression.left());
  if (left)
    expression.replaceLeft(std::move(left));
  std::unique_ptr<Expression> right 
    = evaluate(expression.right()); 
  if (right)
    expression.replaceRight(std::move(right));

  balanceVariablesPlusMinus(expression);
}

} /* namespace kcalc */
