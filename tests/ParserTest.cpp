#include <gtest/gtest.h>

#include "Parser.h"
#include "Exceptions.h"

std::unique_ptr<kcalc::AstObject> testParse(const char * input)
{
  kcalc::Lexer lexer(input);
  kcalc::Parser parser(lexer);
  return parser.parse(); 
}

template<typename T>
void checkParseError(const char * input,
                     const kcalc::Token& token)
{
  bool exceptionThrown = false;
  try
  {
    testParse(input);
  }
  catch(const T& exception)
  {
    exceptionThrown = true;
    std::string expected(token.text());
    std::string actual(exception.token()->text());
    ASSERT_STREQ(expected.c_str(), actual.c_str());
    ASSERT_EQ(token.line(), exception.token()->line());
    ASSERT_EQ(token.offset(), exception.token()->offset()); 
  }
  ASSERT_TRUE(exceptionThrown);
}

TEST(ParserTest, SimpleErrorTest)
{
  checkParseError<kcalc::IllegalEndOfInput>(
      "+", kcalc::Token(kcalc::TokenKind::Number, 
        kcalc::SourcePosition(1,0), "+"));
  checkParseError<kcalc::UnexpectedToken>(
      "1 1", kcalc::Token(kcalc::TokenKind::Number, 
        kcalc::SourcePosition(1,2), "1")); 
  checkParseError<kcalc::IllegalCharacter>(
      "1+?", kcalc::Token(kcalc::TokenKind::Unknown, 
        kcalc::SourcePosition(1,2), "?"));  
}

TEST(ParserTest, PointBeforeLine)
{
  std::unique_ptr<kcalc::AstObject> object = testParse("1+2*-3");
  std::unique_ptr<kcalc::Expression> one = std::make_unique<kcalc::Number>("1");
  std::unique_ptr<kcalc::Expression> two = std::make_unique<kcalc::Number>("2"); 
  std::unique_ptr<kcalc::Expression> three = std::make_unique<kcalc::Number>("3");  

  std::unique_ptr<kcalc::Expression> unary = std::make_unique<kcalc::UnaryMinusExpression>(
      std::move(three));
  std::unique_ptr<kcalc::Expression> times = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Multiply, std::move(two), std::move(unary));
  std::unique_ptr<kcalc::Expression> plus = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Add, std::move(one), std::move(times)); 
  ASSERT_STREQ("1 + ( 2 * ( - 3 ) )", object->to_string().c_str());  
  ASSERT_TRUE(object->equals(*plus.get()));
} 

TEST(ParserTest, RightAssociativity)
{
  std::unique_ptr<kcalc::AstObject> object = testParse("1^2^-3");
  std::unique_ptr<kcalc::Expression> one = std::make_unique<kcalc::Number>("1");
  std::unique_ptr<kcalc::Expression> two = std::make_unique<kcalc::Number>("2"); 
  std::unique_ptr<kcalc::Expression> three = std::make_unique<kcalc::Number>("3");  

  std::unique_ptr<kcalc::Expression> unary = std::make_unique<kcalc::UnaryMinusExpression>(
      std::move(three));
  std::unique_ptr<kcalc::Expression> inner = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Power, std::move(two), std::move(unary));
  std::unique_ptr<kcalc::Expression> outer = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Power, std::move(one), std::move(inner)); 
  ASSERT_STREQ("1 ^ ( 2 ^ ( - 3 ) )", object->to_string().c_str());  
  ASSERT_TRUE(object->equals(*outer.get()));
} 

TEST(ParserTest, LeftAssociativity1)
{
  std::unique_ptr<kcalc::AstObject> object = testParse("1 + 2 + 3");
  std::unique_ptr<kcalc::Expression> one = std::make_unique<kcalc::Number>("1");
  std::unique_ptr<kcalc::Expression> two = std::make_unique<kcalc::Number>("2"); 
  std::unique_ptr<kcalc::Expression> three = std::make_unique<kcalc::Number>("3");  

  std::unique_ptr<kcalc::Expression> left = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Add, std::move(one), std::move(two));
  std::unique_ptr<kcalc::Expression> outer = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Add, std::move(left), std::move(three)); 
  ASSERT_STREQ("( 1 + 2 ) + 3", object->to_string().c_str());  
  ASSERT_TRUE(object->equals(*outer.get()));
} 

TEST(ParserTest, LeftAssociativity2)
{
  std::unique_ptr<kcalc::AstObject> object = testParse("1 - 2 - 3");
  std::unique_ptr<kcalc::Expression> one = std::make_unique<kcalc::Number>("1");
  std::unique_ptr<kcalc::Expression> two = std::make_unique<kcalc::Number>("2"); 
  std::unique_ptr<kcalc::Expression> three = std::make_unique<kcalc::Number>("3");  

  std::unique_ptr<kcalc::Expression> left = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Subtract, std::move(one), std::move(two));
  std::unique_ptr<kcalc::Expression> outer = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Subtract, std::move(left), std::move(three)); 
  ASSERT_STREQ("( 1 - 2 ) - 3", object->to_string().c_str());  
  ASSERT_TRUE(object->equals(*outer.get()));
} 

TEST(ParserTest, LeftAssociativity3)
{
  std::unique_ptr<kcalc::AstObject> object = testParse("1 * 2 / 3");
  std::unique_ptr<kcalc::Expression> one = std::make_unique<kcalc::Number>("1");
  std::unique_ptr<kcalc::Expression> two = std::make_unique<kcalc::Number>("2"); 
  std::unique_ptr<kcalc::Expression> three = std::make_unique<kcalc::Number>("3");  

  std::unique_ptr<kcalc::Expression> left = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Multiply, std::move(one), std::move(two));
  std::unique_ptr<kcalc::Expression> outer = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Divide, std::move(left), std::move(three)); 
  ASSERT_STREQ("( 1 * 2 ) / 3", object->to_string().c_str());  
  ASSERT_TRUE(object->equals(*outer.get()));
} 

TEST(ParserTest, LeftAssociativity4)
{
  std::unique_ptr<kcalc::AstObject> object = testParse("1 / 2 * 3");
  std::unique_ptr<kcalc::Expression> one = std::make_unique<kcalc::Number>("1");
  std::unique_ptr<kcalc::Expression> two = std::make_unique<kcalc::Number>("2"); 
  std::unique_ptr<kcalc::Expression> three = std::make_unique<kcalc::Number>("3");  

  std::unique_ptr<kcalc::Expression> left = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Divide, std::move(one), std::move(two));
  std::unique_ptr<kcalc::Expression> outer = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Multiply, std::move(left), std::move(three)); 
  ASSERT_STREQ("( 1 / 2 ) * 3", object->to_string().c_str());  
  ASSERT_TRUE(object->equals(*outer.get()));
} 

TEST(ParserTest, Brackets)
{
  std::unique_ptr<kcalc::AstObject> object = testParse("(1+2)*3");
  std::unique_ptr<kcalc::Expression> one = std::make_unique<kcalc::Number>("1");
  std::unique_ptr<kcalc::Expression> two = std::make_unique<kcalc::Number>("2"); 
  std::unique_ptr<kcalc::Expression> three = std::make_unique<kcalc::Number>("3");  

  std::unique_ptr<kcalc::Expression> left = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Add, std::move(one), std::move(two));
  std::unique_ptr<kcalc::Expression> outer = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Multiply, std::move(left), std::move(three)); 
  ASSERT_STREQ("( 1 + 2 ) * 3", object->to_string().c_str());  
  ASSERT_TRUE(object->equals(*outer.get()));
} 

TEST(ParserTest, Brackets2)
{
  std::unique_ptr<kcalc::AstObject> object = testParse("1 * ( 2+3)");
  std::unique_ptr<kcalc::Expression> one = std::make_unique<kcalc::Number>("1");
  std::unique_ptr<kcalc::Expression> two = std::make_unique<kcalc::Number>("2"); 
  std::unique_ptr<kcalc::Expression> three = std::make_unique<kcalc::Number>("3");  

  std::unique_ptr<kcalc::Expression> right = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Add, std::move(two), std::move(three));
  std::unique_ptr<kcalc::Expression> outer = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Multiply, std::move(one), std::move(right)); 
  ASSERT_STREQ("1 * ( 2 + 3 )", object->to_string().c_str());  
  ASSERT_TRUE(object->equals(*outer.get()));
} 

TEST(ParserTest, Modulo1)
{
  std::unique_ptr<kcalc::AstObject> object = testParse("1 + 2 % 3");
  std::unique_ptr<kcalc::Expression> one = std::make_unique<kcalc::Number>("1");
  std::unique_ptr<kcalc::Expression> two = std::make_unique<kcalc::Number>("2"); 
  std::unique_ptr<kcalc::Expression> three = std::make_unique<kcalc::Number>("3");  

  std::unique_ptr<kcalc::Expression> right = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Modulo, std::move(two), std::move(three));
  std::unique_ptr<kcalc::Expression> outer = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Add, std::move(one), std::move(right)); 
  ASSERT_STREQ("1 + ( 2 % 3 )", object->to_string().c_str());  
  ASSERT_TRUE(object->equals(*outer.get()));
} 

TEST(ParserTest, Modulo2)
{
  std::unique_ptr<kcalc::AstObject> object = testParse("1 / 2 % 3");
  std::unique_ptr<kcalc::Expression> one = std::make_unique<kcalc::Number>("1");
  std::unique_ptr<kcalc::Expression> two = std::make_unique<kcalc::Number>("2"); 
  std::unique_ptr<kcalc::Expression> three = std::make_unique<kcalc::Number>("3");  

  std::unique_ptr<kcalc::Expression> left = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Divide, std::move(one), std::move(two));
  std::unique_ptr<kcalc::Expression> outer = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Modulo, std::move(left), std::move(three)); 
  ASSERT_STREQ("( 1 / 2 ) % 3", object->to_string().c_str());  
  ASSERT_TRUE(object->equals(*outer.get()));
} 

TEST(ParserTest, Modulo3)
{
  std::unique_ptr<kcalc::AstObject> object = testParse("1 % 2 / 3");
  std::unique_ptr<kcalc::Expression> one = std::make_unique<kcalc::Number>("1");
  std::unique_ptr<kcalc::Expression> two = std::make_unique<kcalc::Number>("2"); 
  std::unique_ptr<kcalc::Expression> three = std::make_unique<kcalc::Number>("3");  

  std::unique_ptr<kcalc::Expression> left = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Modulo, std::move(one), std::move(two));
  std::unique_ptr<kcalc::Expression> outer = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Divide, std::move(left), std::move(three)); 
  ASSERT_STREQ("( 1 % 2 ) / 3", object->to_string().c_str());  
  ASSERT_TRUE(object->equals(*outer.get()));
} 

TEST(ParserTest, Modulo4)
{
  std::unique_ptr<kcalc::AstObject> object = testParse("1 % 2 ^ 3");
  std::unique_ptr<kcalc::Expression> one = std::make_unique<kcalc::Number>("1");
  std::unique_ptr<kcalc::Expression> two = std::make_unique<kcalc::Number>("2"); 
  std::unique_ptr<kcalc::Expression> three = std::make_unique<kcalc::Number>("3");  

  std::unique_ptr<kcalc::Expression> right = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Power, std::move(two), std::move(three));
  std::unique_ptr<kcalc::Expression> outer = std::make_unique<kcalc::ArithmeticExpression>(
      kcalc::ArithmeticExpression::Modulo, std::move(one), std::move(right)); 
  ASSERT_STREQ("1 % ( 2 ^ 3 )", object->to_string().c_str());  
  ASSERT_TRUE(object->equals(*outer.get()));
} 

