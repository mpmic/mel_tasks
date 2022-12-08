#include "hw05.h"
#include <iostream>

std::vector<sql::Token> valid_token_stream() {
  std::vector<sql::Token> tokens;

  tokens.emplace_back(sql::token::Select{});
  tokens.emplace_back(sql::token::Asterisks{});
  tokens.emplace_back(sql::token::From{});
  tokens.emplace_back(sql::token::Identifier{"MY_TABLE"});
  tokens.emplace_back(sql::token::Semicolon{});

  return tokens;
}

std::vector<sql::Token> invalid_token_stream() {
  std::vector<sql::Token> tokens;

  tokens.emplace_back(sql::token::From{});
  tokens.emplace_back(sql::token::Asterisks{});
  tokens.emplace_back(sql::token::Select{});
  tokens.emplace_back(sql::token::Identifier{"MY_TABLE"});
  tokens.emplace_back(sql::token::Semicolon{});

  return tokens;
}

int main() {
  // Change to get an invalid token stream

  auto valid_tokens = valid_token_stream();
  auto invalid_tokens = invalid_token_stream();

  std::cout << "Query should be valid: " << sql::is_valid_sql_query(valid_tokens) << std::endl;
  std::cout << "Query should be invalid: " << sql::is_valid_sql_query(invalid_tokens) << std::endl;

  // Your other tests go here
}
