#pragma once

#include <fstream>
#include "ExpressionNode.h"

namespace Differentiator::Visualizer {

// Делает обход дерева слева-направо и печатает в консоль дерево, положенное на бок.
void printAsTree(const ExprNode &node, std::ofstream &outfile, int currentLevel) {
  if (&node == NULL) {
    return;
  }
  printAsTree(*(node.left_), outfile, currentLevel + 1);
  outfile << std::string(currentLevel, '\t') << "|";
  switch (node.node_type_) {
    case VARIABLE_NODE:
      outfile << node.variable_ << "|" << std::endl;
      break;
    case CONSTANT_NODE:
      outfile << node.constant_ << "|" << std::endl;
      break;
    case OPERATION_NODE: {
#define OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)  \
  if ((NUM) == node.operation_type_){   \
    outfile << (NAME) << "|" << std::endl; \
  } \


#include "Operations.h"

#undef OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)
    }
  }

  if (is_argc(node, 2)) {
    printAsTree(*(node.right_), outfile, currentLevel + 1);
  }
}

void visualize_tree(const ExprNode &node, const char *file_name) {
  FILE *file = fopen(file_name, "w");

  std::ofstream outfile;
  outfile.open(file_name);

  printAsTree(node, outfile, 1);
  outfile.close();
}

}