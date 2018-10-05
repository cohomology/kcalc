#include <gtest/gtest.h>

#include "Ast.h"
#include "Exceptions.h"
#include "Arithmetic.h"

kcalc::ComplexNumber construct(const char *re, const char *im)
{
  kcalc::ComplexNumber num(re != nullptr ? re : "0");
  if (im != nullptr)
  {
    kcalc::ComplexNumber imag(im);  
    num += imag; 
  } 
  return num;
}

template<typename T, T& (T::*method)()> 
std::string testUnary(const char * re, const char *im)
{
  kcalc::ComplexNumber num = construct(re, im);
  (num.*method)();
  return num.to_string().c_str();
}

template<typename T, T& (T::*method)(const T&)> 
std::string testBinary(const char * re, const char *im,
                       const char * re2, const char *im2)
{
  kcalc::ComplexNumber a = construct(re, im);
  kcalc::ComplexNumber b = construct(re2, im2);
  (a.*method)(b);
  return a.to_string().c_str();
} 

#define TEST_FLOOR(re,im,result) \
  do { EXPECT_STREQ(result, (testUnary<kcalc::ComplexNumber, \
        &kcalc::ComplexNumber::floor>(re,im)).c_str()); } while(0)

TEST(ArithTest, TestFloor)
{
  TEST_FLOOR("1.5", "1.5i", "1 + i");
  TEST_FLOOR("-1.5", "-1.5i", "-2 - 2i");
  TEST_FLOOR("0", "0i", "0");
  TEST_FLOOR("0", "-0.5i", "-i");
  TEST_FLOOR("-0.5", "0", "-1");
  TEST_FLOOR("0.00001", "0", "0");
  TEST_FLOOR("-2", "0", "-2");
  TEST_FLOOR("2", "0", "2");   
} 

#define TEST_MOD(re,im,re2,im2,result) \
  do { EXPECT_STREQ(result, (testBinary<kcalc::ComplexNumber, \
        &kcalc::ComplexNumber::operator%=>(re,im,re2,im2).c_str())); } while(0)

TEST(ArithTest, TestMod)
{
  TEST_MOD("-2", "-2i", "1", "0", "0");
  TEST_MOD("-2", "-2i", "-1", "0", "0"); 
  TEST_MOD("2", "2i", "1", "0", "0");  
  TEST_MOD("2", "2i", "-1", "0", "0");   

  TEST_MOD("-2", "0", "1", "0", "0");
  TEST_MOD("-2", "0", "-1", "0", "0"); 
  TEST_MOD("2", "0", "1", "0", "0");  
  TEST_MOD("2", "0", "-1", "0", "0");    

  TEST_MOD("-2.5", "-2i", "-1.5", "0", "-1 - i/2");
  TEST_MOD("-3", "-2i", "-2", "0", "-1");
  TEST_MOD("3", "3i", "-2", "0", "-1 - i");  
  TEST_MOD("3", "3i", "2", "0", "1 + i");   
  TEST_MOD("-3", "-3i", "2", "0", "1 + i");  
}

TEST(ArithTest, TestModException)
{
  bool exceptionThrown = false;
  try
  {
    TEST_MOD("-2.5", "-1.5i", "0", "0", "0");
  }
  catch(const kcalc::DivisionByZeroException&)
  {
    exceptionThrown = true;
  }
  ASSERT_TRUE(exceptionThrown);
}

TEST(ArithTest, TestModImaginary)
{
  bool exceptionThrown = false;
  try
  {
    TEST_MOD("-2.5", "-1.5i", "0", "i", "0");
  }
  catch(const kcalc::ModuloComplexNumberException& e)
  {
    ASSERT_STREQ("i", e.divisor().c_str());
    exceptionThrown = true;
  }
  ASSERT_TRUE(exceptionThrown);
} 

