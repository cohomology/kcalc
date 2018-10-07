#include "SemanticAnalyzer.h"
#include "Ast.h"
#include "SymbolTable.h" 

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

} /* namespace kcalc */
