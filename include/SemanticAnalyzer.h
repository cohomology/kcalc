#ifndef KCALC_SEMANTIC_ANALYZER_H
#define KCALC_SEMANTIC_ANALYZER_H 

#include "Visitor.h"

#include <memory>

namespace kcalc 
{

class SymbolTable;

class SemanticAnalyzer : public Visitor
{
public:
  SemanticAnalyzer(SymbolTable& symbolTable) :
    Visitor{VisitorOrdering::PreOrder, 
      ParentHandling::BeforeParent},
    m_symbolTable{symbolTable}
  { }

  void visit(Assignment& assignment) override;
  void visit(ArithmeticExpression& expression) override;

private:
  std::unique_ptr<Expression> 
  evaluate(const Expression& expr); 

  void balanceVariablesPlusMinus(
      ArithmeticExpression& expression);

  SymbolTable& m_symbolTable;
};

} /* namespace kcalc */

#endif // KCALC_SEMANTIC_ANALYZER_H   
