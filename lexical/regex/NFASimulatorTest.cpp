#include "NFASimulator.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Please input a string to match." << std::endl;
    return -1;
  }

  NFASimulator nfa1;
  state_t st0 = nfa1.add_state(true, true);
  state_t st1 = nfa1.add_state(false, false);
  nfa1.add_edge('a', st0, st1);
  nfa1.add_edge('a', st1, st0);
  nfa1.reset();

  NFASimulator nfa2;
  state_t st4 = nfa2.add_state(true, false);
  state_t st5 = nfa2.add_state(false, true);
  nfa2.add_edge('b', st4, st5);
  nfa2.add_edge('b', st5, st4);
  nfa2.reset();

  NFASimulator nfa3;
  state_t st7 = nfa3.add_state(true, false);
  state_t st8 = nfa3.add_state(false, true);
  nfa3.add_edge('c', st7, st8);
  nfa3.add_edge('c', st8, st7);
  nfa3.reset();

  merge_nfa_or(&nfa1, &nfa2);
  nfa1.reset();
  merge_nfa_and(&nfa1, &nfa3);
  nfa1.reset();

  std::string input(argv[1]); //"aaacccc;
  int last_accept_pos = 0;
  for (auto it=input.begin(); it!=input.end() && !nfa1.get_curr_state().empty(); ++it) {
    nfa1.move(*it);
    if (nfa1.accept()) {
      last_accept_pos = it - input.begin();
    }
  }
  std::cout << "Matched:" << input.substr(0, last_accept_pos+1) << std::endl;
}
