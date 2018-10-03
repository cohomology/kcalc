#ifndef KCALC_AST_H
#define KCALC_AST_H 

#include <gmpxx.h>

#include <memory>
#include <cassert>
#include <ostream>

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

class AstObject
{
public:
  virtual std::string to_string() const = 0; 
  virtual ObjectKind kind() const = 0;
  virtual bool equals(const AstObject&) const = 0;
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
  virtual bool isAtomicExpression() const
  { return false; } 
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

  bool isAtomicExpression() const override
  { return true; }  

  std::string to_string() const override  
  { return m_name; }
  
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

  std::string_view name() const
  { return m_name; }

private:
  std::string m_name;
}; 

class Number : public Expression
{
public:
  Number(const std::string_view& text);

  ObjectKind kind() const override
  { return ObjectKind::Number; }

  bool isAtomicExpression() const override
  { return m_real == 0 || m_imaginary == 0; } 
  
  bool equals(const AstObject& other) const override
  { 
    if (this == &other)
      return true;
    else if (other.kind() != 
        ObjectKind::Number)
      return false;
    const Number& num =
      static_cast<const Number&>(other);
    return m_real == num.m_real &&
      m_imaginary == num.m_imaginary;
  }

  std::string to_string() const override;

private:
  mpq_class m_real;
  mpq_class m_imaginary;
};

} /* namespace kcalc */

#endif // KCALC_AST_H 
