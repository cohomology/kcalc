#include <gtest/gtest.h>

#include "Lexer.h"

TEST(LexerTest, Identifier)
{
  using namespace kcalc;
  const char * test = ("i09234_");
  Lexer lexer(test);
  std::vector<Token> tokens; 
  for (Token token : lexer)
    tokens.push_back(token);
  /* ASSERT_EQ(token1.kind(), TokenKind::Identifier); */
}
