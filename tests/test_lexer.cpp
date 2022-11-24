#include "Lexer.hpp"
#include "Token.hpp"

#include "gtest/gtest.h"
#include <string_view>

TEST(Lexer, SingleCharTokens) {
  Lexer lexer = Lexer("+ = / * > < ! () ,");
  auto tokens = lexer.lex();

  ASSERT_EQ(tokens.size(), 11);

  std::array<TokenType, 11> correctTokenTypes = {
      TokenType::Plus,  TokenType::Equal,     TokenType::Slash,
      TokenType::Star,  TokenType::Greater,   TokenType::Lesser,
      TokenType::Bang,  TokenType::LeftParen, TokenType::RightParen,
      TokenType::Comma, TokenType::Eof,
  };

  for (unsigned int i = 0; i < tokens.size(); i++) {
    EXPECT_EQ(tokens[i].type, correctTokenTypes[i]) << i;
  }
}

TEST(Lexer, DoubleCharTokens) {
  Lexer lexer = Lexer(">= <= != ==");
  auto tokens = lexer.lex();

  ASSERT_EQ(tokens.size(), 5);

  std::array<TokenType, 5> correctTokenTypes = {
      TokenType::GreaterEqual, TokenType::LesserEqual, TokenType::BangEqual,
      TokenType::EqualEqual,   TokenType::Eof,
  };

  for (unsigned int i = 0; i < tokens.size(); i++) {
    EXPECT_EQ(tokens[i].type, correctTokenTypes[i]);
  }
}

TEST(Lexer, SingleLineComment) {
  Lexer lexer = Lexer("# this should be ignored");
  auto tokens = lexer.lex();

  ASSERT_EQ(tokens.size(), 1);
  EXPECT_EQ(tokens.back().type, TokenType::Eof);
}

TEST(Lexer, MultiLineComment) {
  Lexer lexer = Lexer("# this should be ignored \n however this should not");
  auto tokens = lexer.lex();

  ASSERT_EQ(tokens.size(), 5);
  EXPECT_EQ(tokens.back().type, TokenType::Eof);
}

TEST(Lexer, String) {
  Lexer lexer = Lexer(" \"This is a string!\" \"This is another string!\" ");
  auto tokens = lexer.lex();

  ASSERT_EQ(tokens.size(), 3);
  EXPECT_EQ(tokens[0].type, TokenType::String);
  EXPECT_EQ(tokens[1].type, TokenType::String);

  EXPECT_EQ(tokens[0].get<std::string_view>(), "This is a string!");
  EXPECT_EQ(tokens[1].get<std::string_view>(), "This is another string!");
}

TEST(Lexer, Keywords) {
  Lexer lexer = Lexer("let and or def if else then end elif return");
  auto tokens = lexer.lex();

  ASSERT_EQ(tokens.size(), 11);

  std::array<TokenType, 11> correctTokenTypes = {
      TokenType::Let,    TokenType::And,    TokenType::Or,   TokenType::Def,
      TokenType::If,     TokenType::Else,   TokenType::Then, TokenType::End,
      TokenType::ElseIf, TokenType::Return, TokenType::Eof,
  };

  for (unsigned int i = 0; i < tokens.size(); i++) {
    EXPECT_EQ(tokens[i].type, correctTokenTypes[i]) << i;
  }
}

TEST(Lexer, Identifiers) {
  Lexer lexer = Lexer("_test varName var123 var_123");
  auto tokens = lexer.lex();

  std::array<std::string, 4> correctNames = {"_test", "varName", "var123",
                                             "var_123"};

  ASSERT_EQ(tokens.size(), 5);

  for (unsigned int i = 0; i < tokens.size() - 1; i++) {
    EXPECT_EQ(tokens[i].type, TokenType::Identifier);
    EXPECT_EQ(tokens[i].get<std::string_view>(), correctNames[i]);
  }

  ASSERT_EQ(tokens.back().type, TokenType::Eof);
}

TEST(Lexer, Number) {
  Lexer lexer = Lexer("3.141519 1.618 2.71828 137 42");
  auto tokens = lexer.lex();

  std::array<double, 5> correctNumbers = {3.141519, 1.618, 2.71828, 137, 42};

  ASSERT_EQ(tokens.size(), 6);

  for (unsigned int i = 0; i < tokens.size() - 1; i++) {
    EXPECT_EQ(tokens[i].type, TokenType::Number);
    EXPECT_EQ(tokens[i].get<double>(), correctNumbers[i]);
  }

  ASSERT_EQ(tokens.back().type, TokenType::Eof);
}
