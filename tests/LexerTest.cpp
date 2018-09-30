#include <gtest/gtest.h>

#include "Lexer.h"

TEST(LexerTest, Identifier)
{
  using namespace kcalc;
  const char * test = ("i09234_+=1.");
  Lexer lexer(test);
  std::vector<Token> expected = {
    Token(TokenKind::Identifier, SourcePosition(0,0),
        std::string_view(test, 7)),
    Token(TokenKind::Plus, SourcePosition(0,7),
        std::string_view(test+7, 1)),
    Token(TokenKind::Equals, SourcePosition(0,8),
        std::string_view(test+8, 1)),
    Token(TokenKind::Number, SourcePosition(0,9),
        std::string_view(test+9, 2))
  }; 
  unsigned int count = 0;
  for (auto it = lexer.begin(); it != lexer.end();
       ++it, ++count)
  {
    ASSERT_EQ(expected[count], *it);
  }
}

TEST(LexerTest, NewLine)
{
  using namespace kcalc;
  const char * test = ("a\r\nTest123");
  Lexer lexer(test);
  TokenIterator it = lexer.begin();

  Token tok1 = *it;
  EXPECT_EQ(TokenKind::Identifier, tok1.kind());
  EXPECT_EQ(SourcePosition(0,0), tok1.position());
  EXPECT_EQ("a", tok1.text());

  ++it;

  Token tok2 = *it;
  EXPECT_EQ(TokenKind::Whitespace, tok2.kind());
  EXPECT_EQ(SourcePosition(0,1), tok2.position());
  EXPECT_EQ("\r\n", tok2.text()); 

  ++it;

  Token tok3 = *it;
  EXPECT_EQ(TokenKind::Identifier, tok3.kind());
  EXPECT_EQ(SourcePosition(1,0), tok3.position());
  EXPECT_EQ("Test123", tok3.text());  
} 
