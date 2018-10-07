#ifndef KCALC_VISITOR_H
#define KCALC_VISITOR_H 

#include <utility>

namespace kcalc 
{

class AstObject;
class Expression;
class Assignment;
class ArithmeticExpression;
class UnaryMinusExpression;
class Variable;
class Number;

enum class VisitorOrdering 
{
  PreOrder = 0u,  // visit children before current object
  PostOrder = 1u  // current object before children
};

enum class ParentHandling
{
  BeforeParent = 1u, // visit object before parent
  AfterParent = 2u  // visit parent before object
};

class Visitor
{
public:
  template
  <
    typename Parent,   
    typename Class,  
    typename ... Children
  >
  void accept(Class& current, Children&&... children)
  {
    if (m_ordering == VisitorOrdering::PreOrder)
      acceptChildren(std::forward<Children>(children)...);
    if (m_parentHandling == ParentHandling::AfterParent)
      current.Parent::accept(*this);
    visit(current);
    if (m_parentHandling == ParentHandling::BeforeParent)
      current.Parent::accept(*this); 
    if (m_ordering == VisitorOrdering::PostOrder)
      acceptChildren(std::forward<Children>(children)...); 
  }
protected:
  Visitor(const VisitorOrdering ordering, 
          const ParentHandling parentHandling) :
    m_ordering{ordering}, m_parentHandling{parentHandling}
  { }
  virtual void visit(AstObject&) { }
  virtual void visit(Expression&) { }
  virtual void visit(Assignment&) { }
  virtual void visit(ArithmeticExpression&) { } 
  virtual void visit(UnaryMinusExpression&) { }  
  virtual void visit(Variable&) { }   
  virtual void visit(Number&) { }    

private:
  template<typename First, typename ... Children>
  void acceptChildren(First& child, Children&&... others)
  {
    child.accept(*this);
    acceptChildren(std::forward<Children>(others)...);
  }
  void acceptChildren(...)
  { }  

  const VisitorOrdering m_ordering;
  const ParentHandling m_parentHandling; 
};

} /* namespace kcalc */

#endif // KCALC_VISITOR_H  
