#pragma once

#include <variant>
#include <vector>

#include "token.h"

namespace sql {

namespace state {
/// Initial state of the FSM
struct Start {};
/// State if any transition was wrong, no matter the token, the FSM will stay in the invalid state
struct Invalid {};
/// If the sequence of tokens is correct the FSM are in the valid state, wrong tokens put the FSM
/// into the invalid state, only the semicolon is allowed.
struct Valid {};

struct SelectStmt {};

struct AllColumns {};

struct NamedColumn {};

struct MoreColumns {};

struct FromClause {};

struct TableName {};
} // namespace state

/// variant of all possible states of our finite machine
using State =
	std::variant<state::Start, state::Invalid, state::Valid, state::SelectStmt, state::AllColumns,
				 state::NamedColumn, state::MoreColumns, state::FromClause, state::TableName>;

/// Transition from the `Start` state to the next state depending on the given
/// token
[[nodiscard]]
State transition(state::Start, Token token);

/// Transition from the `Valid` state to the next state depending on the given
/// token
[[nodiscard]]
State transition(state::Valid, Token token);

/// Transition from the `Invalid` state to the next state depending on the given
/// token
[[nodiscard]]
State transition(state::Invalid, Token token);

[[nodiscard]]
State transition(state::TableName, Token token);

[[nodiscard]]
State transition(state::FromClause, Token token);

[[nodiscard]]
State transition(state::MoreColumns, Token token);

[[nodiscard]]
State transition(state::NamedColumn, Token token);

[[nodiscard]]
State transition(state::AllColumns, Token token);

[[nodiscard]]
State transition(state::SelectStmt, Token token);

/// Our finite state machine.
/// The initial state is `Start` and based on the given tokens it will move to
/// the next state. You can query if the given sequence of tokens was valid with
/// the `is_valid` member function.
class SqlValidator {
 public:
  SqlValidator() = default;

  /// Returns `true` iff the variant is in the `Valid` state
  [[nodiscard]]
  bool is_valid() const;

  /// Moves from one state to the next given the token.
  void handle(Token token);

 private:
  State state_ = state::Start{};
};

///
/// Given a sequence of tokens, this functions returns true, if it is a valid
/// (simplified) select clause of SQL
///
/// - Example of a valid token sequence:
///   - "SELECT * FROM table;"
/// - Example of a invalid token sequence:
///   - "SELECT Col1 Col2 FROM table;" (Missing comma)
///   - "SELECT Col1, * FROM table;" (Either select specific columns or all)
///   - "SELECT Col1 FROM table" (Missing Semicolon)
///   - "FROM * SELECT Col1;" (Select should be first)
///
/// These sequences must be given in a `std::vector<Token>`
[[nodiscard]]
bool is_valid_sql_query(std::vector<Token> tokens);
} // namespace sql
