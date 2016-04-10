#include "NFAState.hpp"
#include <set>

class NFASimulator {
public:
  NFASimulator();
  void reset();
  state_t add_state(bool accept);
  void add_edge(char input, state_t from, state_t to);
  const std::string& matched_str();
  void move(char input);
  bool accept();
  state_t get_start_state();
  std::set<state_t> get_accept_state();
  // void merge(NFASimulator* another_nfa);
  void print_nfa();
  void print_state();

private:
  int next_state_no;
  std::string matched;
  state_t start_state;
  std::set<state_t> curr_state;
  std::set<state_t> accept_state;
  std::set<state_t> all_state;
};
