#include "NFASimulator.hpp"

#include <iostream>

#define _DEBUG_ON_

NFASimulator::NFASimulator(bool accept) {
  /* Add one start states: 0 */
  next_state_no = 0;
  start_state = new NFAState(accept, next_state_no++);
  if (accept) accept_state.insert(start_state);
  all_state.insert(start_state);
}

void NFASimulator::reset() {
  matched = "";
  curr_state.clear();
  curr_state.insert(start_state);
  curr_state.insert(start_state->GetOutwardEdge('\0').begin(),
                    start_state->GetOutwardEdge('\0').end());
#ifdef _DEBUG_ON_
  std::cout << "# NFA Reset" << std::endl;
  print_nfa();
  print_state();
#endif
}

state_t NFASimulator::add_state(bool accept) {
  state_t new_state = new NFAState(accept, next_state_no++);
  all_state.insert(new_state);
  if (accept) accept_state.insert(new_state);
  return new_state;
}

void NFASimulator::add_edge(char input, state_t from, state_t to) {
  if (!(all_state.count(from) && all_state.count(to))) {
    std::cerr << "Add edge error: state " << from->GetStateNum() << " or "
              << to->GetStateNum() << "not exists." << std::endl;
    return;
  }
  from->AddOutwardEdge(input, to);
  to->AddInwardEdge(input, from);
}

void NFASimulator::print_nfa() {
  std::cout << "-----NFA Structure-----";
  for (auto it=all_state.begin(); it!=all_state.end(); ++it) {
    (*it)->PrintStateInfo();
  }
  std::cout << "-----------------------" << std::endl;
}

void NFASimulator::print_state() {
  std::cout << "Current State: ";
  for (auto it=curr_state.begin(); it!=curr_state.end(); ++it) {
    std::cout << (*it)->GetStateNum() << " ";
  }
  std::cout << std::endl;
}

const std::string& NFASimulator::matched_str() {
  return matched;
}

void NFASimulator::move(char input) {
  std::set<state_t> tmp;
  for (auto it=curr_state.begin(); it!=curr_state.end(); ++it) {
    const std::vector<state_t>& dst = (*it)->GetOutwardEdge(input);
    tmp.insert(dst.begin(), dst.end());
  }
  curr_state = tmp;
  matched += input;

#ifdef _DEBUG_ON_
    std::cout << "# Move (" << input << ")  ";
    if (accept()) {
      std::cout << "Accept: TRUE" << std::endl;
    } else {
      std::cout << "Accept: FALSE" << std::endl;
    }
    print_state();
#endif
}

bool NFASimulator::accept() {
  for (auto it=curr_state.begin(); it!=curr_state.end(); ++it) {
    if ((*it)->IsAccepting()) {
      return true;
    }
  }
  return false;
}

state_t NFASimulator::get_start_state() {
  return start_state;
}

std::set<state_t> NFASimulator::get_accept_state() {
  return accept_state;
}

void merge_nfa_and(NFASimulator* nfa1, NFASimulator* nfa2) {

}

void merge_nfa_or(NFASimulator* nfa1, NFASimulator* nfa2) {
  /* Merge nfa2 to nfa1 */
  /* Step 1: merge start_state */
  merge_state(nfa1->start_state, nfa2->start_state, MERGE_OR);
  /* Step 2: add nfa2.accept_state to nfa1 */
  nfa1->accept_state.insert(nfa2->accept_state.begin(), nfa2->accept_state.end());
  /* Step 3: add nfa2.all_state to nfa1 and Chang state_no*/
  for (auto it=nfa2->all_state.begin(); it!=nfa2->all_state.end(); ++it) {
    (*it)->SetStateNum((*it)->GetStateNum() + nfa1->next_state_no);
    nfa1->all_state.insert(*it);
  }
  nfa1->next_state_no += nfa2->next_state_no;
  nfa2->next_state_no = nfa1->next_state_no;
}
