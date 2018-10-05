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

#define TEST_INVERSE(re,im,result) \
  do { EXPECT_STREQ(result, (testUnary<kcalc::ComplexNumber, \
        &kcalc::ComplexNumber::inverse>(re,im)).c_str()); } while(0) 

TEST(ArithTest, TestInverse)
{
  TEST_INVERSE("1", "0", "1");
  TEST_INVERSE("-1", "0", "-1");
  TEST_INVERSE("0", "i", "-i");
  TEST_INVERSE("0", "-1i", "i"); 
  TEST_INVERSE("1", "i", "1/2 - i/2"); 
  TEST_INVERSE("1", "-1i", "1/2 + i/2");  
  TEST_INVERSE("-1", "i", "-1/2 - i/2");  
  TEST_INVERSE("-1", "-1i", "-1/2 + i/2");   
  TEST_INVERSE("2", "0", "1/2");
  TEST_INVERSE("-2", "0", "-1/2"); 
  TEST_INVERSE("-5", "2i", "-5/29 - 2i/29");
}

TEST(ArithTest, TestInverseException)
{
  bool exceptionThrown = false;
  try
  {
    TEST_INVERSE("0", "0", "0");
  }
  catch(const kcalc::DivisionByZeroException&)
  {
    exceptionThrown = true;
  }
  ASSERT_TRUE(exceptionThrown); 
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

#define TEST_ADD(re,im,re2,im2,result) \
  do { EXPECT_STREQ(result, (testBinary<kcalc::ComplexNumber, \
        &kcalc::ComplexNumber::operator+=>(re,im,re2,im2).c_str())); } while(0)

TEST(ArithTest, TestAdd)
{
  TEST_ADD("0", "0i", "0", "0i", "0");
  TEST_ADD("1", "0i", "0", "0i", "1"); 
  TEST_ADD("0", "0i", "1", "0i", "1");  
  TEST_ADD("1.5", "1.5i", "0.5", "0.5i", "2 + 2i");   
  TEST_ADD("-1.5", "1.5i", "0.5", "0.5i", "-1 + 2i");    
  TEST_ADD("-1.5", "-1.5i", "-0.5", "0.5i", "-2 - i");     
  TEST_ADD("1.5", "1.5i", "-0.5", "-0.5i", "1 + i");      
} 

#define TEST_SUB(re,im,re2,im2,result) \
  do { EXPECT_STREQ(result, (testBinary<kcalc::ComplexNumber, \
        &kcalc::ComplexNumber::operator-=>(re,im,re2,im2).c_str())); } while(0)

TEST(ArithTest, TestSub)
{
  TEST_SUB("0", "0i", "0", "0i", "0");
  TEST_SUB("1", "0i", "0", "0i", "1"); 
  TEST_SUB("0", "0i", "1", "0i", "-1");  
  TEST_SUB("1.5", "1.5i", "0.5", "0.5i", "1 + i");   
  TEST_SUB("-1.5", "1.5i", "0.5", "0.5i", "-2 + i");    
  TEST_SUB("-1.5", "-1.5i", "-0.5", "0.5i", "-1 - 2i");     
  TEST_SUB("1.5", "1.5i", "-0.5", "-0.5i", "2 + 2i");      
} 

#define TEST_MUL(re,im,re2,im2,result) \
  do { EXPECT_STREQ(result, (testBinary<kcalc::ComplexNumber, \
        &kcalc::ComplexNumber::operator*=>(re,im,re2,im2).c_str())); } while(0)

TEST(ArithTest, TestMul)
{
  TEST_MUL("0", "0i", "0", "0i", "0");
  TEST_MUL("1", "0i", "0", "0i", "0"); 
  TEST_MUL("0", "0i", "1", "0i", "0");  
  TEST_MUL("-1", "0i", "1", "0i", "-1");   
  TEST_MUL("-1", "0i", "-1", "0i", "1");    
  TEST_MUL("1", "0i", "-1", "0i", "-1");    
  TEST_MUL("1", "0i", "0", "1i", "i");     
  TEST_MUL("-1", "0i", "0", "1i", "-i");      
  TEST_MUL("0", "i", "1", "0", "i");     
  TEST_MUL("0", "i", "-1", "0", "-i");       
  TEST_MUL("0", "-1i", "1", "0", "-i");     
  TEST_MUL("0", "-1i", "-1", "0", "i");        

  TEST_MUL("0", "1i", "0", "1i", "-1");   
  TEST_MUL("0", "-1i", "0", "-1i", "-1");    
  TEST_MUL("0", "i", "0", "-1i", "1");     
  TEST_MUL("0", "-1i", "0", "i", "1");      

  TEST_MUL("1", "i", "1", "-1i", "2");       
  TEST_MUL("1", "-1i", "1", "-1i", "-2i");        
  TEST_MUL("1", "i", "1", "i", "2i");         

  TEST_MUL("1.02", "-4.03i", "-2.7", "8.6i", "3988/125 + 19653i/1000");          
} 

#define TEST_DIV(re,im,re2,im2,result) \
  do { EXPECT_STREQ(result, (testBinary<kcalc::ComplexNumber, \
        &kcalc::ComplexNumber::operator/=>(re,im,re2,im2).c_str())); } while(0)

TEST(ArithTest, TestDiv)
{
  TEST_DIV("1", "0i", "0", "1i", "-i");
  TEST_DIV("1", "0i", "0", "-1i", "i"); 
  TEST_DIV("-1", "0i", "0", "1i", "i");
  TEST_DIV("-1", "0i", "0", "-1i", "-i");  

  TEST_DIV("-5", "0i", "8", "0i", "-5/8");   
  TEST_DIV("-5", "0i", "8", "-1i", "-8/13 - i/13");    
  TEST_DIV("-5", "1i", "8", "-1i", "-41/65 + 3i/65");     
  TEST_DIV("-5", "1i", "8", "i", "-3/5 + i/5");      
}

TEST(ArithTest, TestDivException)
{
  bool exceptionThrown = false;
  try
  {
    TEST_DIV("-2.5", "-1.5i", "0", "0i", "0");
  }
  catch(const kcalc::DivisionByZeroException& e)
  {
    exceptionThrown = true;
  }
  ASSERT_TRUE(exceptionThrown); 
} 

#define TEST_POW(re,im,re2,im2,result) \
  do { EXPECT_STREQ(result, (testBinary<kcalc::ComplexNumber, \
        &kcalc::ComplexNumber::operator^=>(re,im,re2,im2).c_str())); } while(0) 

TEST(ArithTest, TestPow)
{
  TEST_POW("1", "0i", "1", "0i", "1");
  TEST_POW("1", "0i", "0", "0i", "1"); 
  TEST_POW("0", "0i", "1", "0i", "0");
  TEST_POW("0", "0i", "0", "0i", "1");  

  TEST_POW("1", "1i", "2", "0i", "2i"); 
  TEST_POW("1", "-1i", "2", "0i", "-2i");  
  TEST_POW("1", "1i", "-2", "0i", "-i/2"); 
  TEST_POW("1", "-1i", "-2", "0i", "i/2");   

  TEST_POW("1", "-1i", "-100", "0i", "-1/1125899906842624");    
  TEST_POW("1", "i", "-100", "0i", "-1/1125899906842624");     

  TEST_POW("1", "i", "100", "0i", "-1125899906842624");      
} 

TEST(ArithTest, TestPowDivisionByZero)
{
  bool exceptionThrown = false;
  try
  {
    TEST_POW("0", "0i", "-1", "0i", "0");  
  }
  catch(const kcalc::DivisionByZeroException& e)
  {
    exceptionThrown = true;
  }
  ASSERT_TRUE(exceptionThrown); 
} 

TEST(ArithTest, TestPowLargeExponent)
{
  bool exceptionThrown = false;
  try
  {
    TEST_POW("1", "0i", "1e30", "0i", "0");  
  }
  catch(const kcalc::ExponentiationOverflow& e)
  {
    ASSERT_STREQ("1000000000000000000000000000000", 
        e.exponent().c_str());
    exceptionThrown = true;
  }
  ASSERT_TRUE(exceptionThrown); 
} 

TEST(ArithTest, TestPowComplexExponent)
{
  bool exceptionThrown = false;
  try
  {
    TEST_POW("1", "0i", "0", "1i", "0");  
  }
  catch(const kcalc::PowerIllegalExponentException& e)
  {
    ASSERT_STREQ("i", e.exponent().c_str());
    ASSERT_EQ(kcalc::PowerIllegalExponentException::ComplexExponent,
              e.kind());
    exceptionThrown = true;
  }
  ASSERT_TRUE(exceptionThrown); 
} 

TEST(ArithTest, TestPowRationalExponent)
{
  bool exceptionThrown = false;
  try
  {
    TEST_POW("1", "0i", "0.5", "0i", "0");  
  }
  catch(const kcalc::PowerIllegalExponentException& e)
  {
    ASSERT_STREQ("1/2", e.exponent().c_str());
    ASSERT_EQ(kcalc::PowerIllegalExponentException::RationalExponent,
              e.kind());
    exceptionThrown = true;
  }
  ASSERT_TRUE(exceptionThrown); 
} 
