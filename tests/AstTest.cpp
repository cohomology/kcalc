#include <gtest/gtest.h>

#include "Ast.h"
#include "Exceptions.h"

TEST(AstTest, SimplePositive)
{
  using namespace kcalc;
  const char * number = "121231";
  kcalc::Number num((std::string_view(number)));
  std::string result = num.to_string();
  ASSERT_STREQ("121231", result.c_str());
} 

TEST(AstTest, SimpleNegative)
{
  using namespace kcalc;
  const char * number = "-121231";
  kcalc::Number num((std::string_view(number)));
  std::string result = num.to_string();
  ASSERT_STREQ("-121231", result.c_str());
} 

TEST(AstTest, Decimal)
{
  using namespace kcalc;
  const char * number = "-12.125";
  kcalc::Number num((std::string_view(number)));
  std::string result = num.to_string();
  ASSERT_STREQ("-97/8", result.c_str());
} 

TEST(AstTest, PositiveExponential)
{
  using namespace kcalc;
  const char * number = "-12.125e+12";
  kcalc::Number num((std::string_view(number)));
  std::string result = num.to_string();
  ASSERT_STREQ("-12125000000000", result.c_str());
} 

TEST(AstTest, PositiveExponential2)
{
  using namespace kcalc;
  const char * number = "-12.125e12";
  kcalc::Number num((std::string_view(number)));
  std::string result = num.to_string();
  ASSERT_STREQ("-12125000000000", result.c_str());
} 

TEST(AstTest, ZeroExponential)
{
  using namespace kcalc;
  const char * number = "-12.125e0";
  kcalc::Number num((std::string_view(number)));
  std::string result = num.to_string();
  ASSERT_STREQ("-97/8", result.c_str());
} 

TEST(AstTest, NegativeExponential)
{
  using namespace kcalc;
  const char * number = "-12.125e-1";
  kcalc::Number num((std::string_view(number)));
  std::string result = num.to_string();
  ASSERT_STREQ("-97/80", result.c_str());
} 

TEST(AstTest, NegativeExponential2)
{
  using namespace kcalc;
  const char * number = "-12.125e-12";
  kcalc::Number num((std::string_view(number)));
  std::string result = num.to_string();
  ASSERT_STREQ("-97/8000000000000", result.c_str());
} 

TEST(AstTest, TestException)
{
  using namespace kcalc;
  const char * number = "-12.125e-123456789012345678901";
  try
  {
    kcalc::Number num((std::string_view(number)));
  }
  catch(const kcalc::ExponentiationOverflow& e)
  {
    EXPECT_STREQ("-123456789012345678901", e.exponent().c_str());
    return;
  }
  catch(...)
  { }
  ASSERT_TRUE(1 == 0); 
} 

TEST(AstTest, Complex)
{
  using namespace kcalc;
  const char * number = "-12.125e+12i";
  kcalc::Number num((std::string_view(number)));
  std::string result = num.to_string();
  ASSERT_STREQ("-12125000000000i", result.c_str());
} 

TEST(AstTest, Complex2)
{
  using namespace kcalc;
  const char * number = "-12.125i";
  kcalc::Number num((std::string_view(number)));
  std::string result = num.to_string();
  ASSERT_STREQ("-97i/8", result.c_str());
} 

TEST(AstTest, Complex3)
{
  using namespace kcalc;
  const char * number = "121231i";
  kcalc::Number num((std::string_view(number)));
  std::string result = num.to_string();
  ASSERT_STREQ("121231i", result.c_str());
} 

TEST(AstTest, Complex4)
{
  using namespace kcalc;
  const char * number = "i";
  kcalc::Number num((std::string_view(number)));
  std::string result = num.to_string();
  ASSERT_STREQ("i", result.c_str());
} 
