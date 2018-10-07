#ifndef KCALC_SEMANTIC_ANALYZER_H
#define KCALC_SEMANTIC_ANALYZER_H 

#include "Visitor.h"

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

private:
  SymbolTable& m_symbolTable;
};

} /* namespace kcalc */

#endif // KCALC_SEMANTIC_ANALYZER_H   
