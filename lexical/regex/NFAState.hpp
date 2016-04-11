#include <map>
#include <vector>

#define state_t NFAState*

class NFAState {
public:
  NFAState(bool accept, int state_num);
  bool IsAccepting();
  void SetAccepting(bool accept);
  const int GetStateNum();
  void SetStateNum(int num);
  void AddOutwardEdge(char input, state_t dst);
  void DelOutwardEdge(char input, state_t target);
  void AddInwardEdge(char input, state_t src);
  void DelInwardEdge(char input, state_t target);
  const std::vector<state_t>& GetOutwardEdge(char input);
  const std::vector<state_t> GetAllOutwardEdge();
  const std::vector<state_t>& GetInwardEdge(char input);
  const std::vector<state_t> GetAllInwardEdge();
  friend void merge_state(state_t st1, state_t st2);
  void clean_edges();
  void PrintStateInfo();

private:
  bool is_accepting;
  int state_no;   // may not be unique, just a mark
  std::map<char, std::vector<state_t>> outward_map;
  std::map<char, std::vector<state_t>> inward_map;
};
