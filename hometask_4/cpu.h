#pragma once

#include "parser.h"
#include "CCommand.h"

namespace Cpu {
void compile(const char *in_file, const char *out_file = "myBinFile") {
  CCommand    command;
  std::string program = "";
  Book        program_text(in_file);

  if (!program_text.book_indexed) {
    program_text.book_index();
  }

  std::vector<std::pair<double, size_t> > from;
  std::map<double, size_t>                to;

  for (int i = 0; i < program_text.lines_count(); i++) {
    const std::string command_line = program_text[i]->start_index_;

    if (command_line.empty()) {
      continue;
    }

    Parser::command_from_str(command_line, &command);

    if (command.name_[0] == '<' && command.name_[1] == '-') {
      from.emplace_back(command.argv_[0], program.size() + 1); // +1 имя команды
    }

    program.append(reinterpret_cast<const char *>(&command.num_), sizeof(command.num_));
    program.append(reinterpret_cast<const char *>(command.argv_), sizeof(double) * command.argc_);

    if (command.name_[0] == '-' && command.name_[1] == '>') {
      to.emplace(command.argv_[0], program.size());
    }
  }

  for (const auto &from_ : from) {
    auto to_ = to.find(from_.first);
    assert(to_ != to.end());
    *reinterpret_cast<double *>(&program[from_.second]) = to_->second;
  }

  std::ofstream out(out_file);
  out << program;
  out.close();
}


void decompile(std::ostream &out, const char *input_filename = "myBinFile") {
  auto text = Parser::read_bin_file();
  char *program = text.first;
  size_t size = text.second;

  size_t   position = 0;
  CCommand command;

  do {
    Parser::command_from_bin(program, &command, &position);

    out << command.name_ << " ";
    for (int i = 0; i < command.argc_; ++i) {
      out << command.argv_[i] << " ";
    }
    out << "\n";
  } while (position < size);

  free(program);
}

void run(const char *input_filename = "myBinFile") {

  std::vector<double> registers(4, NAN);
  CStack<double>      stack;
  CStack<double> return_pos;

  char *program = Parser::read_bin_file().first;

  size_t   position = 0;
  CCommand command;
  do {
    Parser::command_from_bin(program, &command, &position);
    switch (command.num_) {

#define COMMAND(NUM, NAME, ARGC, CODE) \
                case (NUM): \
                    CODE \
                    break;

#include "commands.h"

#undef COMMAND
    }
  } while (command.num_ != END);

  free(program);
}
}