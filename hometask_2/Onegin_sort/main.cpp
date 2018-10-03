#include "Book.h"

int main(int argv, char *argc[]) {
  assert("Too many arguments" and argv <= 2);
  assert("Too few arguments" and argv >= 2);
  char *filename = argc[1];

  Book book(filename);

  sort(book);
  output(book, "sort.txt");

  reverse_sort(book);
  output(book, "rev_sort.txt");
  return 0;
}