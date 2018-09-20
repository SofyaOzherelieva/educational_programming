//
// Created by sofya on 16.09.18.
//

#include "Book.h"

void Line::skip_punctuation_marks(int *index, bool is_reverse) const {
  while (!isalpha(buffer_[start_index_ + *index]) && buffer_[start_index_ + *index] != '\0') {
    if (!is_reverse) {
      (*index)++;
    } else {
      (*index)--;
    }
  }
}

int Line::length_of_line() const {
  int len = 0;
  while (buffer_[start_index_ + len] != '\0') {
    len++;
  }
  return len;
}

bool Line::is_empty() const {
  int index = 0;
  skip_punctuation_marks(&index, false);
  return buffer_[start_index_ + index] == '\0';
}

Book::Book(const char *filename) {
  book_ = fopen(filename, "rb");

  if (!book_) {
    perror("fopen");
  };

  fseek(book_, 0, SEEK_END);
  book_size_ = ftell(book_);
  rewind(book_);

  buffer_ = (char *) malloc(sizeof(char) * (book_size_ + 2));

  assert("Memory allocation error" and buffer_ != nullptr);

  // добавили '\0' в начало, чтобы не прописывать случаи в сравнении с конца
  buffer_[0] = '\0';
  if (fread(buffer_ + sizeof(char), sizeof(char), static_cast<size_t>(book_size_), book_) != book_size_) {
    perror("fread");
  };
  // добавили '\0' в начало
  book_size_++;
}

Book::~Book() {
  fclose(book_);
  free(buffer_);
  free(array_of_lines_);
};

void Book::sort() {
  if (!array_of_lines_) {
    book_indexing();
  }

  std::sort(array_of_lines_, array_of_lines_ + num_of_lines_, std::bind(&Book::comparator, this, _1, _2));
}


void Book::reverse_sort() {
  if (!array_of_lines_) {
    book_indexing();
  }

  std::sort(array_of_lines_, array_of_lines_ + num_of_lines_, std::bind(&Book::reverse_comparator, this, _1, _2));
}


void Book::output(const char *filename) const {
  FILE *answer;

  answer = fopen(filename, "wb");
  if (!answer) {
    perror("fopen");
  };

  for (int i = 0; i < num_of_lines_; i++) {

    int curr_line_length = array_of_lines_[i].length_of_line();
    if (!array_of_lines_[i].is_empty()) {
      fwrite(buffer_ + array_of_lines_[i].start_index_, sizeof(char), static_cast<size_t>(curr_line_length), answer);
      fwrite("\n", sizeof(char), 1, answer);
    }
  }
  fclose(answer);
}

void Book::change_n_to_o() {
  for (int i = 0; i < book_size_; i++) {
    if (buffer_[i] == '\n') {
      num_of_lines_++;
      buffer_[i] = '\0';
    }
  }
}

void Book::book_indexing() {
  int index = 0;
  change_n_to_o();

  array_of_lines_ = (Line *) malloc(sizeof(Line) * num_of_lines_);

  assert("Memory allocation error" and array_of_lines_ != nullptr);

  // не учитываем первый символ '\0'
  array_of_lines_[index] = {1, buffer_};

  // не учитываем первый и последний символ '\0'
  for (int i = 1; i < book_size_ - 1; i++) {
    if (buffer_[i] == '\0') {
      index++;
      array_of_lines_[index] = {i + 1, buffer_};
    }
  }
}


// < true
bool Book::comparator(const Line &line1, const Line &line2) {
  int line1_index = 0;
  int line2_index = 0;

  line1.skip_punctuation_marks(&line1_index, false);
  line2.skip_punctuation_marks(&line2_index, false);

  while (tolower(buffer_[line1.start_index_ + line1_index]) == tolower(buffer_[line2.start_index_ + line2_index])) {
    line1_index++;
    line2_index++;
    line1.skip_punctuation_marks(&line1_index, false);
    line2.skip_punctuation_marks(&line2_index, false);
  }
  return buffer_[line1.start_index_ + line1_index] < buffer_[line2.start_index_ + line2_index];
}


// < true
bool Book::reverse_comparator(const Line &line1, const Line &line2) {
  int line1_index = line1.length_of_line();
  int line2_index = line2.length_of_line();

  line1.skip_punctuation_marks(&line1_index, true);
  line2.skip_punctuation_marks(&line2_index, true);

  while (tolower(buffer_[line1.start_index_ + line1_index]) == tolower(buffer_[line2.start_index_ + line2_index])) {
    line1_index--;
    line2_index--;
    line1.skip_punctuation_marks(&line1_index, true);
    line2.skip_punctuation_marks(&line2_index, true);
  }
  return buffer_[line1.start_index_ + line1_index] < buffer_[line2.start_index_ + line2_index];
}