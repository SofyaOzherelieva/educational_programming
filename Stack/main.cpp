#include "stack.h"

int main() {
  CStack<int> stack;
  int n = 5;
  for (int i = 0; i < n; i++) {
    stack.Push(i);
  }
  for (int i = 0; i < n; i++) {
    std::cout << stack.PeekRemove();
  }
  return 0;
}