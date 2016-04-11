#include <iostream>
#include <algorithm>
#include "NFAState.hpp"

NFAState::NFAState(bool accept, int num) {
  is_accepting = accept;
  state_no = num;
}

bool NFAState::IsAccepting() {
  return is_accepting;
}

void NFAState::SetAccepting(bool accept) {
  is_accepting = accept;
}

const int NFAState::GetStateNum() {
  return state_no;
}

void NFAState::SetStateNum(int num) {
  state_no = num;
}

void NFAState::AddOutwardEdge(char input, NFAState* dst) {
  outward_map[input].push_back(dst);
}

void NFAState::AddInwardEdge(char input, NFAState* src) {
  inward_map[input].push_back(src);
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
  for (auto it=outward_map.begin(); it!=outward_map.end(); ++it) {
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
  for (auto it=inward_map.begin(); it!=inward_map.end(); ++it) {
    all_inwards.insert(all_inwards.end(), (it->second).begin(), (it->second).end());
  }
  return all_inwards;
}

void NFAState::PrintStateInfo() {
  std::cout << std::endl;
  std::cout << "<state-" << GetStateNum() << "> ";
  if (is_accepting) {
    std::cout << "Accept: TRUE" << std::endl;
  } else {
    std::cout << "Accept: FALSE" << std::endl;
  }
  std::cout << "Outward:" << std::endl;
  for (auto it=outward_map.begin(); it!=outward_map.end(); ++it) {
    std::cout << "  " << it->first << ": ";
    for (auto vit=it->second.begin(); vit!=it->second.end(); ++vit) {
      std::cout << "<" << (*vit)->GetStateNum() << "> ";
    }
    std::cout << '\n';
  }
  std::cout << "Inward:" << std::endl;
  for (auto it=inward_map.begin(); it!=inward_map.end(); ++it) {
    std::cout << "  " << it->first << ": ";
    for (auto vit=it->second.begin(); vit!=it->second.end(); ++vit) {
      std::cout << "<" << (*vit)->GetStateNum() << "> ";
    }
    std::cout << '\n';
  }
}

void merge_state(state_t st1, state_t st2) {
  /* mrege st2 to st1, state_no and IsAccept is equal to st1
   * In normal case, there shouldn't be any edge between st1 and st2.
   * If there is any, then a self loop of st1 will be generated.
   * After the merge, the caller should delete st2 manully.
   */
  if (!st1 || !st2) return;
  // outward_map
  for (auto it=st2->outward_map.begin(); it!=st2->outward_map.end(); ++it) {
    char input = it->first;
    std::vector<state_t> edges = it->second;  // Cannot use reference here
    for (auto eit=edges.begin(); eit!=edges.end(); ++eit) {
      state_t dst = *eit;
      if (dst == st2) {
        // Self-loop need to be handled specially
        st1->AddOutwardEdge(input, st1);
        st1->AddInwardEdge(input, st1);
      } else {
        dst->AddInwardEdge(input, st1);   // Add the new edge
        st1->AddOutwardEdge(input, dst);
      }
    }
  }
  // inward_map
  for (auto it=st2->inward_map.begin(); it!=st2->inward_map.end(); ++it) {
    char input = it->first;
    std::vector<state_t> edges = it->second;
    for (auto eit=edges.begin(); eit!=edges.end(); ++eit) {
      state_t dst = *eit;
      if (dst != st2) {
        // Self-loop has been handled above, so just omit that case
        dst->AddOutwardEdge(input, st1);   // Add the new edge
        st1->AddInwardEdge(input, dst);
      }
    }
  }
}

void NFAState::clean_edges() {
  // clean outward_map
  for (auto it=outward_map.begin(); it!=outward_map.end(); ++it) {
    char input = it->first;
    std::vector<state_t> edges = it->second;  // Cannot use reference here
    for (auto eit=edges.begin(); eit!=edges.end(); ++eit) {
      state_t dst = *eit;
      DelOutwardEdge(input, dst);
      dst->DelInwardEdge(input, this);
    }
  }
  // clean inward_map
  for (auto it=inward_map.begin(); it!=inward_map.end(); ++it) {
    char input = it->first;
    std::vector<state_t> edges = it->second;  // Cannot use reference here
    for (auto eit=edges.begin(); eit!=edges.end(); ++eit) {
      state_t dst = *eit;
      DelInwardEdge(input, dst);
      dst->DelOutwardEdge(input, this);
    }
  }
}
