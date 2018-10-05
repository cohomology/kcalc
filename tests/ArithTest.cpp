#include <gtest/gtest.h>

#include "Ast.h"
#include "Exceptions.h"

TEST(ArithTest, TestFloor)
{
  using namespace kcalc;
  const char * real = "1.5";
  const char * imag = "1.5i";
  kcalc::ComplexNumber num1(real);
  kcalc::ComplexNumber num2(imag); 
  num1 += num2;
  ASSERT_STREQ("3/2 + 3i/2", num1.to_string().c_str()); 
  num1.floor();
  ASSERT_STREQ("1 + i", num1.to_string().c_str());
} 

TEST(ArithTest, TestFloorNegative)
{
  using namespace kcalc;
  const char * real = "-1.5";
  const char * imag = "-1.5i";
  kcalc::ComplexNumber num1(real);
  kcalc::ComplexNumber num2(imag); 
  num1 += num2;
  ASSERT_STREQ("-3/2 - 3i/2", num1.to_string().c_str()); 
  num1.floor();
  ASSERT_STREQ("-2 - 2i", num1.to_string().c_str());
} 

TEST(ArithTest, TestMod)
{
  using namespace kcalc;
  const char * real = "-1.5";
  const char * imag = "-1.5i";
  kcalc::ComplexNumber num1(real);
  kcalc::ComplexNumber num2(imag); 
  kcalc::ComplexNumber num3("1");
  num1 += num2;
  ASSERT_STREQ("-3/2 - 3i/2", num1.to_string().c_str()); 
  num1 %= num3;
  ASSERT_STREQ("1/2 + i/2", num1.to_string().c_str());
} 

TEST(ArithTest, TestMod2)
{
  using namespace kcalc;
  const char * real = "-2";
  const char * imag = "-2i";
  kcalc::ComplexNumber num1(real);
  kcalc::ComplexNumber num2(imag); 
  kcalc::ComplexNumber num3("1.5");
  num1 += num2;
  ASSERT_STREQ("-2 - 2i", num1.to_string().c_str()); 
  num1 %= num3;
  ASSERT_STREQ("1/2 + i/2", num1.to_string().c_str());
} 
