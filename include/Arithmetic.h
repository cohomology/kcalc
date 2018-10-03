#ifndef KCALC_ARITHMETIC_H
#define KCALC_ARITHMETIC_H 

#include <gmpxx.h>

namespace kcalc 
{

class ComplexNumber
{
public:
  ComplexNumber(
      const std::string_view& text);

  bool isPure() const
  { return m_real == 0 || m_imaginary == 0; }   

  bool operator==(const ComplexNumber& number) const
  { 
    return m_real == number.m_real && 
        m_imaginary == number.m_imaginary; 
  }

  std::string to_string() const;
  
private:
  mpq_class m_real;
  mpq_class m_imaginary; 
};

} /* namespace kcalc */

#endif // KCALC_ARITHMETIC_H  
