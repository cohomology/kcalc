#include <gtest/gtest.h>

#include "Lexer.h"

TEST(LexerTest, Identifier)
{
  using namespace kcalc;
  const char * test = ("i09234_+=1.+00.01*1  (1e+5){1.2E-5}");
  Lexer lexer(test);
  std::vector<Token> expected = {
    Token(TokenKind::Identifier, SourcePosition(1,0),
        std::string_view(test, 7)),
    Token(TokenKind::Plus, SourcePosition(1,7),
        std::string_view(test+7, 1)),
    Token(TokenKind::Equals, SourcePosition(1,8),
        std::string_view(test+8, 1)),
    Token(TokenKind::Number, SourcePosition(1,9),
        std::string_view(test+9, 2)),
    Token(TokenKind::Plus, SourcePosition(1,11),
        std::string_view(test+11, 1)),
    Token(TokenKind::Number, SourcePosition(1,12),
        std::string_view(test+12, 1)),
    Token(TokenKind::Number, SourcePosition(1,13),
        std::string_view(test+13, 4)), 
    Token(TokenKind::Asterisk, SourcePosition(1,17),
        std::string_view(test+17, 1)), 
    Token(TokenKind::Number, SourcePosition(1,18),
        std::string_view(test+18, 1)),
    Token(TokenKind::Whitespace, SourcePosition(1,19),
        std::string_view(test+19, 2)),
    Token(TokenKind::LeftParen, SourcePosition(1,21),
        std::string_view(test+21, 1)),    
    Token(TokenKind::Number, SourcePosition(1,22),
        std::string_view(test+22, 4)),     
    Token(TokenKind::RightParen, SourcePosition(1,26),
        std::string_view(test+26, 1)),      
    Token(TokenKind::LeftBracket, SourcePosition(1,27),
        std::string_view(test+27, 1)),       
    Token(TokenKind::Number, SourcePosition(1,28),
        std::string_view(test+28, 6)),        
    Token(TokenKind::RightBracket, SourcePosition(1,34),
        std::string_view(test+34, 1))        
  }; 
  unsigned int count = 0;
  for (auto it = lexer.begin(); it != lexer.end();
       ++it, ++count)
  {
    ASSERT_EQ(expected[count], *it);
  }
  ASSERT_EQ(16, count);
}

TEST(LexerTest, NewLine)
{
  using namespace kcalc;
  const char * test = ("a\r\nTest\n123");
  Lexer lexer(test);
  TokenIterator it = lexer.begin();

  Token tok1 = *it;
  EXPECT_EQ(TokenKind::Identifier, tok1.kind());
  EXPECT_EQ(SourcePosition(1,0), tok1.position());
  EXPECT_EQ("a", tok1.text());

  ++it;

  Token tok2 = *it;
  EXPECT_EQ(TokenKind::Newline, tok2.kind());
  EXPECT_EQ(SourcePosition(1,1), tok2.position());
  EXPECT_EQ("\r\n", tok2.text()); 

  ++it;

  Token tok3 = *it;
  EXPECT_EQ(TokenKind::Identifier, tok3.kind());
  EXPECT_EQ(SourcePosition(2,0), tok3.position());
  EXPECT_EQ("Test", tok3.text());  

  ++it;

  Token tok4 = *it;
  EXPECT_EQ(TokenKind::Newline, tok4.kind());
  EXPECT_EQ(SourcePosition(2,4), tok4.position());
  EXPECT_EQ("\n", tok4.text());  

  ++it;

  Token tok5 = *it;
  EXPECT_EQ(TokenKind::Number, tok5.kind());
  EXPECT_EQ(SourcePosition(3,0), tok5.position());
  EXPECT_EQ("123", tok5.text());   

} 
