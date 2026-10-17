#include "DFA.hpp"
#include <iostream>
#include <stack>
#include <string.h>
using namespace std;
bool accept(char a[], int l, int r) {
  if (l >= r || a[l] == a[r])
    return 0;
  if (r == l + 1)
    return a[r] == a[l] ? 0 : 1;
  return accept(a, l + 1, r - 1);
}
bool recognizeCFL(const string &str) {
  if (str.size() % 2 != 0)
    return false;
  stack<char> s;
  size_t size = str.size();
  int i = 0;
  for (; i < size; ++i) {
    if (str.at(i) == 'a')
      s.push(str.at(i));
    else if (str.at(i) == 'b')
      break;
    else if (str.at(i) == ' ')
      continue;
    else
      return false;
  }
  for (; i < size; ++i) {
    if (str.at(i) == 'b') {
      if (s.empty())
        return false;
      s.pop();
    } else if (str.at(i) == ' ')
      continue;
    else
      return false;
  }
  return s.empty();
}
int main() {
  SIMPLE_DFA d("states.txt", "transition table.txt");
  cout << d.recognizeString("0111110") << '\n';
  return 0;
}