#include "validator.h"

#include <variant>
#include <vector>

#include "token.h"

namespace sql {

bool SqlValidator::is_valid() const {
  return std::holds_alternative<state::Valid>(state_);
}

void SqlValidator::handle(Token token) {
  State next_state = std::visit([&token](auto &state) {
	return transition(state, token);
  }, state_);

  state_ = next_state;
}

struct TransitionFromStartVisitor {
  /// Only the `Select` token advances the FSM from the `Start` state
  State operator()(token::Select) const { return state::SelectStmt{}; }

  /// All the other tokens, put it in the invalid state
  State operator()(token::From) const { return state::Invalid{}; }
  State operator()(token::Comma) const { return state::Invalid{}; }
  State operator()(token::Asterisks) const { return state::Invalid{}; }
  State operator()(token::Semicolon) const { return state::Invalid{}; }
  State operator()(token::Identifier) const { return state::Invalid{}; }
};

State transition(state::Start, Token token) {
  return std::visit(TransitionFromStartVisitor{}, token.value());
}

State transition(state::SelectStmt, Token token) {

  if (std::holds_alternative<token::Asterisks>(token.value())) {
	return state::AllColumns{};
  } else if (std::holds_alternative<token::Identifier>(token.value())) {
	return state::NamedColumn{};
  }

  return state::Invalid{};

}

State transition(state::AllColumns, Token token) {

  if (std::holds_alternative<token::From>(token.value())) {
	return state::FromClause{};
  }
  return state::Invalid{};

}

State transition(state::NamedColumn, Token token) {
  if (std::holds_alternative<token::Comma>(token.value())) {
	return state::MoreColumns{};
  } else if (std::holds_alternative<token::From>(token.value())) {
	return state::FromClause{};
  }
  return state::Invalid{};

}

State transition(state::MoreColumns, Token token) {
  if (std::holds_alternative<token::Identifier>(token.value())) {
	return state::NamedColumn{};
  }

  return state::Invalid{};

}

State transition(state::FromClause, Token token) {
  if (std::holds_alternative<token::Identifier>(token.value())) {
	return state::TableName{};
  }

  return state::Invalid{};

}

State transition(state::TableName, Token token) {
  if (std::holds_alternative<token::Semicolon>(token.value())) {
	return state::Valid{};
  }
  return state::Invalid{};

}

State transition(state::Valid, Token token) {

  return state::Valid{};

}

State transition(state::Invalid, Token token) {

  return state::Invalid{};
}

bool is_valid_sql_query(std::vector<Token> tokens) {

  SqlValidator check_syntax = SqlValidator();

  for (auto token : tokens) {
	check_syntax.handle(Token(token));
  }
  return check_syntax.is_valid();

}
} // namespace sql

