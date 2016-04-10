#include <iostream>
#include "NFAState.hpp"

NFAState::NFAState(bool accept, int state_no) {
  this->is_accepting = accept;
  this->state_no = state_no;
}

bool NFAState::IsAccepting() {
  return this->is_accepting;
}

const int NFAState::GetStateNum() {
  return this->state_no;
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

const std::vector<NFAState*> NFAState::GetAllOutwardEdge() {
  std::vector<NFAState*> all_outwards;
  for (std::map<char, std::vector<NFAState*>>::iterator
       it=this->outward_map.begin(); it!=outward_map.end(); ++it) {
    all_outwards.insert(all_outwards.end(), (it->second).begin(), (it->second).end());
  }
  return all_outwards;
}

const std::vector<NFAState*>& NFAState::GetInwardEdge(char input) {
  /* If no inward edge for input found, return an empty vector */
  return inward_map[input];
}

const std::vector<NFAState*> NFAState::GetAllInwardEdge() {
  std::vector<NFAState*> all_inwards;
  for (std::map<char, std::vector<NFAState*>>::iterator
       it=this->inward_map.begin(); it!=inward_map.end(); ++it) {
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
  for (std::map<char, std::vector<NFAState*>>::iterator
       it=this->outward_map.begin(); it!=outward_map.end(); ++it) {
    std::cout << "  " << it->first << ": ";
    for (std::vector<NFAState*>::iterator vit=it->second.begin();
         vit!=it->second.end(); ++vit) {
      std::cout << "<" << (*vit)->GetStateNum() << ">  ";
    }
    std::cout << '\n';
  }
  std::cout << "Inward:" << std::endl;
  for (std::map<char, std::vector<NFAState*>>::iterator
       it=this->inward_map.begin(); it!=inward_map.end(); ++it) {
    std::cout << "  " << it->first << ": ";
    for (std::vector<NFAState*>::iterator vit=it->second.begin();
         vit!=it->second.end(); ++vit) {
      std::cout << "<" << (*vit)->GetStateNum() << ">  ";
    }
    std::cout << '\n';
  }
}
