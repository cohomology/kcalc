#include "Ast.h"
#include "Exceptions.h"

#include <cassert>
#include <iterator>

namespace kcalc
{

std::string Assignment::to_string() const
{
  std::string result;
  result.append(left().to_string());
  result.append(" = ");
  result.append(right().to_string());
  return result;
}

std::string ArithmeticExpression::to_string() const 
{
  std::string result;
  bool leftBracket = !left().isAtomicExpression();
  bool rightBracket = !right().isAtomicExpression();
  if (leftBracket)
    result.append(" ( ");
  result.append(left().to_string());
  if (leftBracket)
    result.append(" ) ");
  switch(m_operation)
  {
    case Add:
      result.append(" + ");
      break;
    case Subtract:
      result.append(" - ");
      break; 
    case Multiply:
      result.append(" * ");
      break; 
    case Divide:
      result.append(" / ");
      break; 
    case Power:
      result.append(" ^ ");
      break; 
    default:
      assert(1 == 0);
      break;
  }
  if (rightBracket)
    result.append(" ( ");
  result.append(right().to_string());
  if (rightBracket)
    result.append(" ) ");
  return result; 
}

std::string UnaryMinusExpression::to_string() const 
{
  std::string result;
  result.append(" - ");
  result.append(inner().to_string());
  return result; 
} 

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

Number::Number(const std::string_view& text)
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

std::string Number::to_string() const 
{
  std::string result;
  bool printReal = m_real != 0;
  bool printImaginary = m_imaginary != 0;
  if (printReal)
    result.append(m_real.get_str());
  if (printReal && printImaginary)
    result.append(" + ");
  if (printImaginary)
  {
    result.append(m_imaginary.get_str());
    result.append("i");
  }
  if (!printReal && !printImaginary)
    result.append("0");
  return result;
}

} // namespace kcalc 
