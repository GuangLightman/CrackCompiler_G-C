#include <map>
#include <vector>

#define state_t NFAState*

class NFAState {
public:
  NFAState(bool accept, int state_num);
  bool IsAccepting();
  const int GetStateNum();
  void AddOutwardEdge(char input, state_t dst);
  void AddInwardEdge(char input, state_t src);
  const std::vector<state_t>& GetOutwardEdge(char input);
  const std::vector<state_t> GetAllOutwardEdge();
  const std::vector<state_t>& GetInwardEdge(char input);
  const std::vector<state_t> GetAllInwardEdge();
  void PrintStateInfo();

private:
  bool is_accepting;
  int state_no;
  std::map<char, std::vector<state_t>> outward_map;
  std::map<char, std::vector<state_t>> inward_map;
};
