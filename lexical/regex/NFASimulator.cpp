#include "NFASimulator.hpp"

#include <iostream>
#include <algorithm>

#define _DEBUG_ON_

NFASimulator::NFASimulator() {
  /* Add one start states: 0 */
  next_state_no = 0;
}

void NFASimulator::reset() {
  matched = "";
  curr_state.clear();
  curr_state.insert(start_state.begin(), start_state.end());
#ifdef _DEBUG_ON_
  std::cout << "# NFA Reset" << std::endl;
  print_nfa();
  print_state();
#endif
}

state_t NFASimulator::add_state(bool start, bool accept) {
  state_t new_state = new NFAState(accept, next_state_no++);
  all_state.insert(new_state);
  if (start) start_state.insert(new_state);
  if (accept) accept_state.insert(new_state);
  return new_state;
}

void NFASimulator::del_state(state_t target) {
  std::set<state_t>::iterator it;
  if ((it=accept_state.find(target)) != accept_state.end()) {
    accept_state.erase(it);
  }
  if ((it=start_state.find(target)) != start_state.end()) {
    start_state.erase(it);
  }
  if ((it=all_state.find(target)) != all_state.end()) {
    all_state.erase(it);
  }
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

std::set<state_t> NFASimulator::get_start_state() {
  return start_state;
}

std::set<state_t> NFASimulator::get_curr_state() {
  return curr_state;
}

std::set<state_t> NFASimulator::get_accept_state() {
  return accept_state;
}

void merge_nfa_and(NFASimulator* nfa1, NFASimulator* nfa2) {
  /* Merge nfa2 to nfa1 under "AND" relationship */
  /* Step 1: merge nfa1's accept_state to nfa2's start_state */
  for (auto ait=nfa1->accept_state.begin(); ait!=nfa1->accept_state.end(); ++ait) {
    for (auto sit=nfa2->start_state.begin(); sit!=nfa2->start_state.end(); ++sit) {
      merge_state(*sit, *ait);
      if (nfa1->start_state.find(*ait) != nfa1->start_state.end()) {
        /* If an accepting_state is also a start_state, then need to add
         * the merged state into the start_state set. */
        nfa1->start_state.insert(*sit);
      }
    }
  }
  // delete merged states (they are useless forever)
  std::set<state_t> tmp = nfa1->accept_state;
  for (auto ait=tmp.begin(); ait!=tmp.end(); ++ait) {
    (*ait)->clean_edges();
    nfa2->del_state(*ait);
    nfa1->del_state(*ait);
    free(*ait);
  }
  /* Step 2: let nfa1's accept_state equal to nfa2's */
  for (auto it= nfa1->accept_state.begin(); it!=nfa1->accept_state.end(); ++it) {
    (*it)->SetAccepting(false);
  }
  nfa1->accept_state.clear();
  nfa1->accept_state.insert(nfa2->accept_state.begin(), nfa2->accept_state.end());
  /* Step 3: add nfa2.all_state to nfa1 and Chang state_no*/
  for (auto it=nfa2->all_state.begin(); it!=nfa2->all_state.end(); ++it) {
    (*it)->SetStateNum((*it)->GetStateNum() + nfa1->next_state_no);
    nfa1->all_state.insert(*it);
  }
  nfa1->next_state_no += nfa2->next_state_no;
  nfa2->next_state_no = nfa1->next_state_no;
}

void merge_nfa_or(NFASimulator* nfa1, NFASimulator* nfa2) {
  /* Merge nfa2 to nfa1 under "OR" relationship */
  /* Step 1: merge start_state */
  nfa1->start_state.insert(nfa2->start_state.begin(), nfa2->start_state.end());
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
  std::cout << "Start State: ";
  for (auto it=start_state.begin(); it!=start_state.end(); ++it) {
    std::cout << (*it)->GetStateNum() << " ";
  }
  std::cout << std::endl;
  std::cout << "Accept State: ";
  for (auto it=accept_state.begin(); it!=accept_state.end(); ++it) {
    std::cout << (*it)->GetStateNum() << " ";
  }
  std::cout << std::endl;
}
