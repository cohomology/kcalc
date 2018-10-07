#ifndef KCALC_AST_H
#define KCALC_AST_H 

#include <memory>
#include <cassert>
#include <ostream>

#include "Arithmetic.h"
#include "Visitor.h"

namespace kcalc 
{

enum class ObjectKind : unsigned short
{
  ArithmeticExpression = 0u,
  Assignment = 1u,
  Variable = 2u,
  Number = 3u,
  UnaryMinus = 4u
};

class Expression;
class SymbolTable;

class AstObject
{
public:
  virtual void accept(Visitor& visitor)
  { }
  virtual std::string to_string() const = 0; 
  virtual ObjectKind kind() const = 0;
  virtual bool equals(const AstObject&) const = 0;
  virtual std::unique_ptr<AstObject> clone() const = 0;
  virtual std::unique_ptr<Expression> eval(SymbolTable&) const = 0; 
  friend std::ostream& operator<<(std::ostream& out, 
      const AstObject& object) 
  {
    out << object.to_string();
    return out; 
  }
};

class Expression : public AstObject
{ 
public:
  void accept(Visitor& visitor) override
  { visitor.accept<AstObject>(*this); }
  virtual bool isAtomicExpression() const
  { return false; } 
  virtual std::unique_ptr<Expression> cloneExpression() const = 0; 
  std::unique_ptr<AstObject> clone() const override 
  { return cloneExpression(); }
  virtual bool containsVariables() const = 0;
};

class Assignment : public AstObject
{
public:
  Assignment(std::unique_ptr<Expression> left, 
             std::unique_ptr<Expression> right)
    : m_left{std::move(left)}, 
    m_right{std::move(right)}
  { }

  ObjectKind kind() const override
  { return ObjectKind::Assignment; }

  void accept(Visitor& visitor) override
  { 
    assert(m_left && m_right);
    visitor.accept<AstObject, Assignment>(*this, 
       *m_left.get(), *m_right.get()); 
  } 
  
  bool equals(const AstObject& other) const override
  { 
    if (this == &other)
      return true;
    else if (other.kind() != 
        ObjectKind::Assignment)
      return false;
    const Assignment& oAss =
      static_cast<const Assignment&>(other);
    return left().equals(oAss.left()) &&
      right().equals(oAss.right());
  }

  std::unique_ptr<AstObject> clone() const override
  {
    assert(m_left && m_right);
    return std::make_unique<Assignment>(
        std::move(m_left->cloneExpression()),
        std::move(m_right->cloneExpression()));
  }

  std::unique_ptr<Expression> eval(SymbolTable& symbolTable) const override
  { 
    assert(m_left);
    return m_left->eval(symbolTable); 
  }

  std::string to_string() const override;

  const Expression& left() const
  { 
    assert(m_left);
    return *m_left.get(); 
  }

  Expression& left() 
  { 
    assert(m_left);
    return *m_left.get(); 
  } 

  const Expression& right() const
  { 
    assert(m_right);
    return *m_right.get(); 
  
  }

  Expression& right() 
  { 
    assert(m_right);
    return *m_right.get(); 
  }  

private:
  std::unique_ptr<Expression> m_left;
  std::unique_ptr<Expression> m_right;
}; 

class ArithmeticExpression : public Expression
{
public:
  enum Operation
  {
    Add,
    Subtract,
    Multiply,
    Divide,
    Power, 
    Modulo
  };

  ArithmeticExpression(Operation operation,
      std::unique_ptr<Expression> left, 
      std::unique_ptr<Expression> right)
    : m_operation{operation}, m_left{std::move(left)}, 
    m_right{std::move(right)}
  { }

  ObjectKind kind() const override
  { return ObjectKind::ArithmeticExpression; }

  Operation operation() const
  { return m_operation; }

  void operation(Operation operation)
  { m_operation = operation; }

  void invertOperation();

  bool containsVariables() const override 
  { 
    assert(m_left && m_right);
    return m_left->containsVariables() &&
      m_right->containsVariables();
  } 

  void accept(Visitor& visitor) override
  { 
    assert(m_left && m_right);
    visitor.accept<Expression>(*this, *m_left.get(), 
      *m_right.get()); 
  }  
  
  bool equals(const AstObject& other) const override
  { 
    if (this == &other)
      return true;
    else if (other.kind() != 
        ObjectKind::ArithmeticExpression)
      return false;
    const ArithmeticExpression& oArith =
      static_cast<const ArithmeticExpression&>(other);
    return m_operation == oArith.m_operation &&
      left().equals(oArith.left()) &&
      right().equals(oArith.right());
  }

  std::unique_ptr<Expression> cloneExpression() const override
  {
    assert(m_left && m_right);
    return std::make_unique<ArithmeticExpression>(
        m_operation,
        std::move(m_left->cloneExpression()),
        std::move(m_right->cloneExpression()));
  } 

  std::string to_string() const override; 

  const Expression& left() const
  { 
    assert(m_left);
    return *m_left.get(); 
  }

  Expression& left() 
  { 
    assert(m_left);
    return *m_left.get(); 
  } 

  void replaceLeft(std::unique_ptr<Expression> left) 
  { m_left.swap(left); }

  const Expression& right() const
  { 
    assert(m_right);
    return *m_right.get(); 
  }

  Expression& right() 
  { 
    assert(m_right);
    return *m_right.get(); 
  }  

  void replaceRight(std::unique_ptr<Expression> right) 
  { m_right.swap(right); } 

  std::unique_ptr<Expression> eval(SymbolTable&) const; 

private:
  Operation   m_operation;
  std::unique_ptr<Expression> m_left;
  std::unique_ptr<Expression> m_right;
};

class UnaryMinusExpression : public Expression
{
public:
  UnaryMinusExpression(std::unique_ptr<Expression> inner) 
    : m_inner{std::move(inner)}
  { }

  ObjectKind kind() const override
  { return ObjectKind::UnaryMinus; }

  bool containsVariables() const override 
  { 
    assert(m_inner);
    return m_inner->containsVariables();
  } 

  void accept(Visitor& visitor) override
  { 
    assert(m_inner);
    visitor.accept<Expression>(*this, *m_inner.get()); 
  } 

  bool equals(const AstObject& other) const override
  { 
    if (this == &other)
      return true;
    else if (other.kind() != 
        ObjectKind::UnaryMinus)
      return false;
    auto& minus =
      static_cast<const UnaryMinusExpression&>(other);
    return inner().equals(minus.inner());
  }

  std::string to_string() const override; 

  std::unique_ptr<Expression> cloneExpression() const override
  {
    assert(m_inner);
    return std::make_unique<UnaryMinusExpression>(
        std::move(m_inner->cloneExpression()));
  } 

  const Expression& inner() const
  { 
    assert(m_inner);
    return *m_inner.get(); 
  }

  Expression& inner() 
  { 
    assert(m_inner);
    return *m_inner.get(); 
  } 

  std::unique_ptr<Expression> eval(SymbolTable&) const;   

private:
  std::unique_ptr<Expression> m_inner;
}; 

class Variable : public Expression
{
public:
  Variable(const std::string_view& name)
    : m_name{name.begin(), name.end()}
  { }

  ObjectKind kind() const override
  { return ObjectKind::Variable; }

  void accept(Visitor& visitor) override
  { visitor.accept<Expression>(*this); } 

  bool isAtomicExpression() const override
  { return true; }  

  std::string to_string() const override  
  { return m_name; }

  bool containsVariables() const override 
  { return false; }  
  
  bool equals(const AstObject& other) const override
  { 
    if (this == &other)
      return true;
    else if (other.kind() != 
        ObjectKind::Variable)
      return false;
    const Variable& var =
      static_cast<const Variable&>(other);
    return m_name == var.m_name;
  }

  std::unique_ptr<Expression> cloneExpression() const override
  { return std::make_unique<Variable>(m_name); } 

  std::unique_ptr<Expression> eval(SymbolTable&) const;

  std::string_view name() const
  { return m_name; }

private:
  std::string m_name;
}; 

class Number : public Expression
{
public:
  Number(const std::string_view& text)
    : m_number{text}
  { }

  Number(const ComplexNumber& number)
    : m_number{number}
  { }

  void accept(Visitor& visitor) override
  { visitor.accept<Expression>(*this); }  

  ObjectKind kind() const override
  { return ObjectKind::Number; }

  bool containsVariables() const override 
  { return true; } 

  bool isAtomicExpression() const override
  { return m_number.isPure(); }
  
  bool equals(const AstObject& other) const override
  { 
    if (this == &other)
      return true;
    else if (other.kind() != 
        ObjectKind::Number)
      return false;
    const Number& num =
      static_cast<const Number&>(other);
    return m_number == num.m_number;
  }

  std::unique_ptr<Expression> cloneExpression() const override
  { return std::make_unique<Number>(m_number); } 

  std::string to_string() const override
  { return m_number.to_string(); }

  std::unique_ptr<Expression> eval(SymbolTable&) const     
  { return cloneExpression(); }

  const ComplexNumber& number() const
  { return m_number; }

  ComplexNumber& number()
  { return m_number; }

private:
  ComplexNumber m_number;
};

} /* namespace kcalc */

#endif // KCALC_AST_H 
