#include "stack.h"

int main() {
  Stack<int> stack;
  int n = 5;
  for(int i = 0; i < n; i++){
    stack.Push(i);
  }
  for(int i = 0; i < n; i++){
    stack.Pop();
  }
  return 0;
}