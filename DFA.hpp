#include <bits/stdc++.h>

using namespace std;

template <> struct std::hash<pair<string, string>> {
  size_t operator()(const pair<string, string> &p) const {
    size_t h1 = hash<string>()(p.first);
    size_t h2 = hash<string>()(p.second);
    return h1 ^ (h2 << 1);
  }
};
class SIMPLE_DFA {
private:
  unordered_set<string> states;
  unordered_set<string> final_states;
  string start_state;
  string trap_state;
  unordered_map<pair<string, string>, string> transitions;
  void initializeStatesFromFile(const char *filepath) {
    FILE *fptr = fopen(filepath, "r");
    if (!fptr) {
      cerr << "Error: can't open the file!\n";
      exit(1);
    }
    char ch;
    string temp;
    while ((ch = fgetc(fptr)) != EOF) {
      if (ch == ',' || ch == '\n') {
        states.emplace(temp);
        temp.clear();
        if (ch == '\n')
          break;
      } else {
        if (!isspace(ch) && ch != '\n')
          temp.push_back(ch);
      }
    }
    while ((ch = fgetc(fptr)) != EOF) {
      if (ch == '\n')
        break;
      else if (!isspace(ch))
        temp.push_back(ch);
    }
    start_state = temp;
    temp.clear();
    while ((ch = fgetc(fptr)) != EOF) {
      if (ch == ',' || ch == '\n') {
        final_states.emplace(temp);
        temp.clear();
        if (ch == '\n')
          break;
      } else if (!isspace(ch))
        temp.push_back(ch);
    }
    while ((ch = fgetc(fptr)) != EOF) {
      if (ch == '\n')
        break;
      temp.push_back(ch);
    }
    trap_state = temp;
    temp.clear();
    fclose(fptr);
  }
  pair<pair<string, string>, string> getTransition(char transition_line[]) {
    int length = static_cast<int>(strlen(transition_line));
    pair<pair<string, string>, string> ans;
    int pointer = 0;
    string temp;
    for (int i = 0; i < length; ++i) {
      if (transition_line[i] == ',') {
        ++pointer;
        switch (pointer) {
        case 1:
          ans.first.first = temp;
          break;
        case 2:
          ans.first.second = temp;
          break;
        default:
          break;
        }
        temp.clear();
      } else if (isalnum(transition_line[i]) || transition_line[i] == '_')
        temp.push_back(transition_line[i]);
    }
    ans.second = temp;
    if (ans.first.first.empty() || ans.first.second.empty() ||
        ans.second.empty()) {
      cerr << "Error: Invalid transition format in file!\n";
      exit(3);
    }
    return ans;
  }
  void initializeTransitionTableFromFile(const char *filepath) {
    FILE *fptr = fopen(filepath, "r");
    if (!fptr) {
      cerr << "Error: can't open the file!\n";
      exit(2);
    }
    char buffer[256];
    while ((fgets(buffer, sizeof(buffer), fptr)) != NULL) {
      if (strlen(buffer) > 0) {
        pair<pair<string, string>, string> transition = getTransition(buffer);
        transitions[transition.first] = transition.second;
      }
    }
    fclose(fptr);
  }
  string makeTransition(const string &current_state, const string &input) {
    unordered_map<pair<string, string>, string>::iterator it =
        transitions.find({current_state, input});
    if (it != transitions.end())
      return it->second;
    return trap_state;
  }

public:
  SIMPLE_DFA(const char *statesFilePath, const char *transitionTableFilePath) {
    initializeStatesFromFile(statesFilePath);
    initializeTransitionTableFromFile(transitionTableFilePath);
  }
  bool recognizeString(const string &str) {
    string current_state = start_state;
    for (const char &i : str) {
      current_state = makeTransition(current_state, string(1, i));
      if (current_state == trap_state)
        return false;
    }
    return final_states.find(current_state) != final_states.end();
  }
  void displayTransitionTable() const {
    for (const auto &t : transitions)
      cout << "[" << t.first.first << ", " << t.first.second << "] => "
           << t.second << '\n';
  }
  void displayStates() const {
    cout << "States: ";
    for (const string &i : states)
      cout << i << ' ';
    cout << '\n';
    cout << "Start state: " << start_state << '\n';
    cout << "Trap state: " << trap_state << '\n';
    cout << "Final states: ";
    for (const string &i : final_states)
      cout << i << ' ';
    cout << '\n';
  }
};