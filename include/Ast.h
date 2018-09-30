#ifndef KCALC_AST_H
#define KCALC_AST_H 

#include <gmpxx.h>

#include <memory>
#include <cassert>

namespace kcalc 
{

enum class ObjectKind : unsigned short
{
  ArithmeticExpression = 0u,
  Assignment = 1u,
  Variable = 2u,
  Number = 3u
};

class AstObject
{
public:
  virtual ObjectKind kind() const = 0;
  virtual bool equals(const AstObject&) const = 0;
};

class Expression : public AstObject
{ };

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
    Divide
  };

  ArithmeticExpression(Operation operation,
      std::unique_ptr<Expression> left, 
      std::unique_ptr<Expression> right)
    : m_operation{operation}, m_left{std::move(left)}, 
    m_right{std::move(right)}
  { }

  ObjectKind kind() const override
  { return ObjectKind::ArithmeticExpression; }
  
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

class Variable : public Expression
{
public:
  Variable(const std::string_view& name)
    : m_name{name.begin(), name.end()}
  { }

  ObjectKind kind() const override
  { return ObjectKind::Variable; }
  
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
  
  bool equals(const AstObject& other) const override
  { 
    if (this == &other)
      return true;
    else if (other.kind() != 
        ObjectKind::Variable)
      return false;
    const Number& num =
      static_cast<const Number&>(other);
    return m_number == num.m_number;
  }

  std::string get_string() const
  { return mpq_class(m_number).get_str(); }

private:
  mpq_t m_number;
};

} /* namespace kcalc */

#endif // KCALC_AST_H 
