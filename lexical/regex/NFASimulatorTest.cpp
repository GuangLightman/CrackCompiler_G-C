#include "NFASimulator.hpp"
#include <iostream>

int main(void) {
  NFASimulator nfa;
  state_t st0 = nfa.get_start_state();
  state_t st1 = nfa.add_state(false);
  state_t st2 = nfa.add_state(true);
  nfa.add_edge('a', st0, st1);
  nfa.add_edge('a', st1, st0);
  nfa.add_edge('a', st1, st1);
  nfa.add_edge('a', st1, st2);
  nfa.reset();

  std::string input = "aaaaaacccc";
  bool accept = nfa.accept();
  for (auto it=input.begin(); it!=input.end(); ++it) {
    nfa.move(*it);
    bool next_accept = nfa.accept();
    if (accept && !next_accept) {
      std::cout << "Matched:" << input.substr(0, it-input.begin()-1) << std::endl;
      break;
    } else {
      accept = next_accept;
    }
  }
}
