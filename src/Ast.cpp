#include "Ast.h"

namespace kcalc
{

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
  unsigned int numberOfDecimals = 0;
  if (decimalPoint != text.end() && 
      decimalPoint + 1 != exponential)
  {
    number.append(decimalPoint+1, exponential);
    numberOfDecimals = std::distance(decimalPoint + 1,
      exponential); 
  }
}

} // namespace kcalc 
