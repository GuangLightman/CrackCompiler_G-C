#include "NFAState.hpp"
#include <set>

class NFASimulator {
public:
  NFASimulator();
  void reset();
  state_t add_state(bool start, bool accept);
  void del_state(state_t target);
  void add_edge(char input, state_t from, state_t to);
  const std::string& matched_str();
  void move(char input);
  bool accept();
  std::set<state_t> get_start_state();
  std::set<state_t> get_curr_state();
  std::set<state_t> get_accept_state();
  friend void merge_nfa_and(NFASimulator* nfa1, NFASimulator* nfa2);
  friend void merge_nfa_or(NFASimulator* nfa1, NFASimulator* nfa2);
  void print_nfa();
  void print_state();
//  ~NFASimulator();

private:
//  bool merged;
  int next_state_no;
  std::string matched;
  std::set<state_t> start_state;
  std::set<state_t> curr_state;
  std::set<state_t> accept_state;
  std::set<state_t> all_state;
};
