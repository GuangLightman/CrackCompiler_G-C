#include <iostream>
#include <algorithm>
#include "NFAState.hpp"

NFAState::NFAState(bool accept, int state_no) {
  this->is_accepting = accept;
  this->state_no = state_no;
}

bool NFAState::IsAccepting() {
  return this->is_accepting;
}

const int NFAState::GetStateNum() {
  return state_no;
}

void NFAState::SetStateNum(int num) {
  state_no = num;
}

void NFAState::AddOutwardEdge(char input, NFAState* dst) {
  (this->outward_map[input]).push_back(dst);
}

void NFAState::AddInwardEdge(char input, NFAState* src) {
  (this->inward_map[input].push_back(src));
}

const std::vector<NFAState*>& NFAState::GetOutwardEdge(char input) {
  /* If no outward edge for input found, return an empty vector */
  return outward_map[input];
}

void NFAState::DelOutwardEdge(char input, state_t target) {
  std::vector<state_t>& v = outward_map[input];
  v.erase(std::remove(v.begin(), v.end(), target), v.end());
}

const std::vector<NFAState*> NFAState::GetAllOutwardEdge() {
  std::vector<NFAState*> all_outwards;
  for (auto it=this->outward_map.begin(); it!=outward_map.end(); ++it) {
    all_outwards.insert(all_outwards.end(), (it->second).begin(), (it->second).end());
  }
  return all_outwards;
}

const std::vector<NFAState*>& NFAState::GetInwardEdge(char input) {
  /* If no inward edge for input found, return an empty vector */
  return inward_map[input];
}

void NFAState::DelInwardEdge(char input, state_t target) {
  std::vector<state_t>& v = inward_map[input];
  v.erase(std::remove(v.begin(), v.end(), target), v.end());
}

const std::vector<NFAState*> NFAState::GetAllInwardEdge() {
  std::vector<NFAState*> all_inwards;
  for (auto it=this->inward_map.begin(); it!=inward_map.end(); ++it) {
    all_inwards.insert(all_inwards.end(), (it->second).begin(), (it->second).end());
  }
  return all_inwards;
}

void NFAState::PrintStateInfo() {
  std::cout << std::endl;
  std::cout << "<state-" << this->GetStateNum() << "> ";
  if (is_accepting) {
    std::cout << "Accept: TRUE" << std::endl;
  } else {
    std::cout << "Accept: FALSE" << std::endl;
  }
  std::cout << "Outward:" << std::endl;
  for (auto it=this->outward_map.begin(); it!=outward_map.end(); ++it) {
    std::cout << "  " << it->first << ": ";
    for (auto vit=it->second.begin(); vit!=it->second.end(); ++vit) {
      std::cout << "<" << (*vit)->GetStateNum() << ">  ";
    }
    std::cout << '\n';
  }
  std::cout << "Inward:" << std::endl;
  for (auto it=this->inward_map.begin(); it!=inward_map.end(); ++it) {
    std::cout << "  " << it->first << ": ";
    for (auto vit=it->second.begin(); vit!=it->second.end(); ++vit) {
      std::cout << "<" << (*vit)->GetStateNum() << ">  ";
    }
    std::cout << '\n';
  }
}

void merge_state(state_t st1, state_t st2) {
  /* mrege st2 to st1, state_no is equal to st1 */
  if (!st1 || !st2) return;
  // is_accepting
  st1->is_accepting = st1->is_accepting && st2->is_accepting;
  // outward_map
  for (auto it=st2->outward_map.begin(); it!=st2->outward_map.end(); ++it) {
    char input = it->first;
    std::vector<state_t> edges = it->second;  // Cannot use reference here
    for (auto eit=edges.begin(); eit!=edges.end(); ++eit) {
      state_t dst = *eit;
      dst->DelInwardEdge(input, st2);   // Delete the old edge
      st2->DelOutwardEdge(input, dst);
      dst->AddInwardEdge(input, st1);   // Add the new edge
      st1->AddOutwardEdge(input, dst);
    }
  }
  // inward_map
  for (auto it=st2->inward_map.begin(); it!=st2->inward_map.end(); ++it) {
    char input = it->first;
    std::vector<state_t> edges = it->second;
    for (auto eit=edges.begin(); eit!=edges.end(); ++eit) {
      state_t dst = *eit;
      dst->DelOutwardEdge(input, st2);   // Delete the old edge
      st2->DelInwardEdge(input, dst);
      dst->AddOutwardEdge(input, st1);   // Add the new edge
      st1->AddInwardEdge(input, dst);
    }
  }
}
