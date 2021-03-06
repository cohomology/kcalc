#ifndef KCALC_ARITHMETIC_H
#define KCALC_ARITHMETIC_H 

#include <gmpxx.h>

#include "Exceptions.h"

namespace kcalc 
{

class ComplexNumber
{
public:
  ComplexNumber(
      const std::string_view& text);
  ComplexNumber(
      const long real,
      const long imag = 0) :
    m_real{real}, m_imaginary{imag}
  { }
  ComplexNumber(
      const ComplexNumber&) = default;
  ComplexNumber(
      ComplexNumber&&) = default; 

  bool isPure() const
  { return m_real == 0 || m_imaginary == 0; }   

  bool operator==(const ComplexNumber& number) const
  { 
    return m_real == number.m_real && 
        m_imaginary == number.m_imaginary; 
  }

  ComplexNumber& operator+=(
      const ComplexNumber& other)
  {
    m_real += other.m_real;
    m_imaginary += other.m_imaginary;
    return *this;
  }

  ComplexNumber operator+(
      const ComplexNumber& other) const
  {
    ComplexNumber copy(*this);
    copy += other;
    return copy;
  }

  ComplexNumber& operator-=(
      const ComplexNumber& other)
  {
    m_real -= other.m_real;
    m_imaginary -= other.m_imaginary;
    return *this;
  } 

  ComplexNumber operator-(
      const ComplexNumber& other) const
  {
    ComplexNumber copy(*this);
    copy -= other;
    return copy;
  } 

  ComplexNumber& operator*=(
      const ComplexNumber& other)
  {
    mpq_class real = m_real * other.m_real
      - m_imaginary * other.m_imaginary;
    mpq_class imag = m_real * other.m_imaginary 
      + m_imaginary * other.m_real;
    m_real.swap(real);
    m_imaginary.swap(imag);
    return *this;
  }

  ComplexNumber operator*(
      const ComplexNumber& other) const
  {
    ComplexNumber copy(*this);
    copy *= other;
    return copy;
  } 

  ComplexNumber& operator/=(
      const ComplexNumber& other)
  {
    ComplexNumber copy(other);
    *this *= copy.inverse();
    return *this;
  }

  ComplexNumber operator/(
      const ComplexNumber& other) const
  {
    ComplexNumber copy(*this);
    copy /= other;
    return copy;
  } 

  ComplexNumber& operator%=(
      const ComplexNumber& other)
  {
    if (other.m_imaginary != 0)
      throw ModuloComplexNumberException(__FILE__, __LINE__,
          to_string(), other.to_string());
    ComplexNumber copy(*this);
    copy /= other;
    copy.floor();
    copy *= other;
    *this -= copy;
    return *this;
  }  

  ComplexNumber operator%(
      const ComplexNumber& other) const
  {
    ComplexNumber copy(*this);
    copy %= other;
    return copy;
  } 

  ComplexNumber& operator^=(
      const ComplexNumber& other);

  ComplexNumber operator^(
      const ComplexNumber& other) const
  {
    ComplexNumber copy(*this);
    copy ^= other;
    return copy;
  } 

  ComplexNumber& inverse() 
  {
    mpq_class divisor = 
      (m_real * m_real) + (m_imaginary * m_imaginary);
    if (divisor == 0)
      throw DivisionByZeroException(__FILE__, __LINE__);
    m_real /= divisor;
    m_imaginary /= - divisor;
    return *this;
  }

  ComplexNumber& floor(); 

  std::string to_string() const;

private:
  void binExp(unsigned long exponent); 

  mpq_class m_real;
  mpq_class m_imaginary; 
};

} /* namespace kcalc */

#endif // KCALC_ARITHMETIC_H  
