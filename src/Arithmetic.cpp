#include "Arithmetic.h"
#include "Exceptions.h"

#include <cassert>
#include <iterator> 

namespace kcalc 
{ 

static long parseExponent(const std::string_view& view)
{
  assert(!view.empty());
  errno = 0;
  char * temp;
  const char * start = &*view.begin();
  long val = strtol(start, &temp, 10);

  if (std::distance<const char*>(start, temp) != std::distance(view.begin(), view.end()) ||
      ((val == LONG_MIN || val == LONG_MAX) && errno == ERANGE))
    throw ExponentiationOverflow(__FILE__, __LINE__, view);

  return val;
} 

ComplexNumber::ComplexNumber(const std::string_view& text)
  : m_real(), m_imaginary()
{
  auto end = text.end();
  bool complexI = false;
  if (text.back() == 'i') 
  {
    end = text.begin() + text.size() - 1;
    complexI = true;
  } 
  auto decimalPoint = std::find_if(text.begin(), end, 
      [](char c) { return c == '.'; });
  auto exponential = std::find_if(text.begin(), end, 
      [](char c) { return c == 'E' || c == 'e'; });
  std::string number(text.begin(), 
      decimalPoint != end ? decimalPoint : 
      ( exponential != end ? exponential :
        end ));
  if (decimalPoint != end && 
      decimalPoint + 1 != exponential)
  {
    number.append(decimalPoint+1, exponential);
    int numberOfDecimals = std::distance(decimalPoint + 1,
      exponential); 
    number.append("/1");
    for(int i = 0; i < numberOfDecimals; ++i)
      number.append("0");
  }
  if (number.empty())
    number = "1"; 
  mpq_t num;
  mpq_init(num);
  mpq_set_str(num, number.c_str(), 10);
  mpq_canonicalize(num);

  if (exponential != end)
  {
    ++exponential;
    assert(exponential != end);
    long exp = parseExponent(
        std::string_view(exponential, 
          std::distance(exponential, end)));
    unsigned long posExp = std::abs(exp);
    mpz_t tenPow;
    mpz_init(tenPow);
    mpz_set_ui(tenPow, 10);
    mpz_ui_pow_ui(tenPow, 10, posExp);
    mpq_t tenPowQ;
    mpq_init(tenPowQ);
    mpq_set_z(tenPowQ, tenPow);
    mpz_clear(tenPow); 
    if (exp > 0)
      mpq_mul(num, num, tenPowQ);
    else
      mpq_div(num, num, tenPowQ);
    mpq_clear(tenPowQ);
  }
  mpq_class temp(num); 
  if (complexI)
    m_imaginary.swap(temp);
  else
    m_real.swap(temp);
  mpq_clear(num);
} 

std::string ComplexNumber::to_string() const 
{
  std::string result;
  bool printReal = m_real != 0;
  bool printImaginary = m_imaginary != 0;
  if (printReal)
    result.append(m_real.get_str());
  if (printReal && printImaginary)
  {
    if (m_imaginary > 0)
      result.append(" + ");
    else
      result.append(" - ");
  }
  if (printImaginary)
  {
    mpq_class num = printReal ?
      abs(m_imaginary.get_num()) :
      m_imaginary.get_num();
    mpz_class den = m_imaginary.get_den();
    if (num != 1 && num != -1)
      result.append(num.get_str());
    if (num == -1)
      result.append("-");
    result.append("i"); 
    if (den != 1)
    {
      result.append("/");
      result.append(den.get_str());
    }
  }
  if (!printReal && !printImaginary)
    result.append("0");
  return result;
} 

static void do_floor(mpq_class& number)
{
  mpq_ptr num = number.get_mpq_t();
  assert(num != nullptr);
  mpz_t floor;
  mpz_init(floor);
  mpz_set_q(floor, num);
  mpq_t floorQ;
  mpq_init(floorQ);
  mpq_set_z(floorQ, floor);
  mpq_class tmp(floorQ); 
  if (number < 1)
    tmp -= 1;
  number.swap(tmp);
  mpq_clear(floorQ);
  mpz_clear(floor);
}

ComplexNumber& ComplexNumber::floor() 
{
  do_floor(m_real);
  do_floor(m_imaginary);
  return *this;
}

void ComplexNumber::binExp(unsigned long exponent)
{
  ComplexNumber copy(*this);
  m_real = 1;
  m_imaginary = 0;
  while (exponent > 0)
  {
    if (exponent & 1) 
      *this *= copy;
    copy *= copy;
    exponent >>= 1;
  }
}

ComplexNumber& ComplexNumber::operator^=(
    const ComplexNumber& other)
{
  if (other.m_imaginary != 0)
    throw PowerIllegalExponentException(__FILE__, __LINE__,
        PowerIllegalExponentException::ComplexExponent, 
        other.to_string());
  mpz_class den = other.m_real.get_den();
  if (den != 1)
    throw PowerIllegalExponentException(__FILE__, __LINE__,
        PowerIllegalExponentException::RationalExponent, 
        other.to_string()); 
  mpz_class exp = other.m_real.get_num();
  if (!exp.fits_slong_p())
    throw ExponentiationOverflow(__FILE__, __LINE__,
        other.to_string());  
  long lexp = exp.get_si();
  if (lexp == 0)
  {
    m_real = 1;
    m_imaginary = 0;
  }
  else
  {
    if (lexp < 0)
      inverse();
    binExp(lexp);
  }
  return *this;
} 

} /* namespace kcalc */ 
