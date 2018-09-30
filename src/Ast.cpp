#include "Ast.h"
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

Number::Number(const std::string_view& text)
  : m_number()
{
  auto decimalPoint = std::find_if(text.begin(),
      text.end(), [](char c) { return c == '.'; });
  auto exponential = std::find_if(text.begin(),
      text.end(), [](char c) { 
      return c == 'E' || c == 'e'; });
  std::string number(text.begin(), 
      decimalPoint != text.end() ? decimalPoint : 
      ( exponential != text.end() ? exponential :
        text.end() ));
  if (decimalPoint != text.end() && 
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

  if (exponential != text.end())
  {
    ++exponential;
    assert(exponential != text.end());
    long exp = parseExponent(
        std::string_view(exponential, 
          std::distance(exponential, text.end())));
    unsigned long posExp = std::abs(exp);
    mpz_t tenPow;
    mpz_init(tenPow);
    mpz_set_ui(tenPow, 10);
    mpz_ui_pow_ui(tenPow, 10, posExp);
    mpq_t tenPowQ;
    mpq_init(tenPowQ);
    mpq_set_z(tenPowQ, tenPow);
    if (exp > 0)
    {
      mpq_t result;
      mpq_init(result);
      mpq_mul(result, num, tenPowQ);
      mpq_class rc(result);
      m_number.swap(rc);
    }
    else
    {
      mpq_t result;
      mpq_init(result);
      mpq_div(result, num, tenPowQ);
      mpq_class rc(result); 
      m_number.swap(rc);
    }
  }
  else
  {
    mpq_class rc(num);  
    m_number.swap(rc);
  }
}

} // namespace kcalc 
