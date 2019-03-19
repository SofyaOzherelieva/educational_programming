#pragma once

#include "../Stack/stack.h"
#include "../Onegin_sort/Book.h"
#include <string>
#include <cassert>
#include <iostream>
#include <cstring>
#include "string"
#include <sstream>
#include <map>
#include <cmath>
#include <vector>

#include "CCommand.h"

#define EPS 0.0001

namespace Parser {
const size_t MAX_ARGC = 1;

void parse_line(const std::string &line, std::string *parts, size_t *size) {
  for (int i = 0; i < line.length(); i++) {
    assert("Incorrect command" && *size < MAX_ARGC + 1);
    if (line[i] != ' ') {
      parts[*size].push_back(line[i]);
    } else {
      (*size)++;
    }
  }
}

void command_from_str(const std::string &command_line, CCommand *command) {
  std::string parts[MAX_ARGC + 1]; // + 1 for command word
  size_t      size = 0;
  parse_line(command_line, parts, &size);

  CCommand new_command(parts, size);

  *command = new_command;

#define COMMAND(NUM, NAME, ARGC, CODE)\
        if (command->name_ == (NAME) && command->argc_ == (ARGC)) {\
          command->num_=StrToCommand[(NAME)]; \
          return; \
        }

#include "commands.h"

#undef COMMAND

  assert("No such command in command list" && false);
}

void command_from_bin(const char *program, CCommand *command, size_t *position) {
  (*command).num_ = Command(program[*position]);

  bool is_command = false;
#define COMMAND(NUM, NAME, ARGC, CODE) \
  if((*command).num_ == (NUM)){ \
    (*command).name_ = (NAME); \
    (*command).argc_ = (ARGC); \
    is_command = true; \
  }

#include "commands.h"

#undef COMMAND

  if (!is_command) {
    assert("No such command in command list" && false);
  }

  (*position)++;
  size_t argc = command->argc_;
  if (argc) {
    command->argv_ = new double[argc];
    for (int i     = 0; i < argc; i++) {
      command->argv_[i] = reinterpret_cast<const double *>(program + *position)[i];
    }
  }
  *position += argc * sizeof(double);
}


std::pair<char *, size_t> read_bin_file(const char *input_filename = "myBinFile") {
  FILE *book_ = fopen(input_filename, "rb");

  fseek(book_, 0, SEEK_END);
  size_t book_size_ = static_cast<size_t>(ftell(book_));
  rewind(book_);

  char *program = (char *) malloc(sizeof(char) * (book_size_));

  if (fread(program, sizeof(char), static_cast<size_t>(book_size_), book_) != book_size_) {
    perror("fread");
  };

  return {program, book_size_};
}
}