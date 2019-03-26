#pragma once

#include "ExpressionNode.h"

#define iterStrType std::string::iterator


namespace Differentiator::ExpressionToGraph {

std::unique_ptr<ExprNode> double_to_graph(iterStrType begin, iterStrType end);

std::unique_ptr<ExprNode> variable_to_graph(iterStrType begin, iterStrType end);

std::unique_ptr<ExprNode> binary_operation_to_graph(iterStrType begin, iterStrType end);

std::unique_ptr<ExprNode> unary_operation_to_graph(iterStrType begin, iterStrType end);

std::unique_ptr<ExprNode> expression_to_graph(const iterStrType begin, const iterStrType end);
}

std::unique_ptr<ExprNode> Differentiator::ExpressionToGraph::double_to_graph(iterStrType begin, iterStrType end) {
  std::string str_value(begin, end);
  double      value = std::stod(str_value);
  return std::make_unique<ExprNode>(value);
}

std::unique_ptr<ExprNode> Differentiator::ExpressionToGraph::variable_to_graph(iterStrType begin, iterStrType end) {
  return std::make_unique<ExprNode>(*begin);
}

std::unique_ptr<ExprNode>
Differentiator::ExpressionToGraph::binary_operation_to_graph(iterStrType begin, iterStrType end) {
  iterStrType current_pos = begin;
  assert(*current_pos == '(');

  ++current_pos;
  int balance = 1;
  // доходим до середины выражения
  while(balance != 0 && current_pos != end) {
    if (*current_pos == '(') ++balance;
    if (*current_pos == ')') --balance;
    current_pos++;
  }

  assert(current_pos != end && "Incorrect format of binary operation: the first arg");

  iterStrType operation_pointer = current_pos;
  std::string operation = "";
  operation += *current_pos;

  ++current_pos;
  assert(current_pos != end && *current_pos == '(' && "Incorrect format of binary operation: the second arg is not found");

  balance = 1;
  ++current_pos;
  // доходим до конца выражения
  while(balance != 0 && current_pos != end) {
    if (*current_pos == '(') ++balance;
    if (*current_pos == ')') --balance;
    current_pos++;
  }

  assert(balance == 0 && "Incorrect format of binary operation: the second arg");
  assert(current_pos == end && "Incorrect format of binary operation: symbols after the end");

#define OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)  \
  if ((NAME) == operation){   \
    auto arg1 = expression_to_graph(begin + 1, operation_pointer - 1); \
    auto arg2 = expression_to_graph(operation_pointer + 2, end - 1); \
    assert(arg1 && arg2 && "Incorrect format of binary operation: missing arg"); \
    return std::make_unique<ExprNode>((NUM), std::move(arg1), std::move(arg2)); \
  } \


#include "Operations.h"

#undef OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)
  assert(false && "Incorrect operator");

  //return std::make_unique<ExprNode>(operation_type, std::move(arg1), std::move(arg2));
}

std::unique_ptr<ExprNode>
Differentiator::ExpressionToGraph::unary_operation_to_graph(iterStrType begin, iterStrType end) {

  std::string current_name(begin, begin + 3);

#define OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)  \
  if ((NAME) == current_name){   \
    auto arg = expression_to_graph(begin + 4, end - 1); \
    return std::make_unique<ExprNode>((NUM), std::move(arg)); \
  } \


#include "Operations.h"

#undef OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)
  assert(false && "Incorrect operator");
}

std::unique_ptr<ExprNode> Differentiator::ExpressionToGraph::expression_to_graph(const iterStrType begin,
                                                                                 const iterStrType end) {
  if (end == begin) {
    return nullptr;
  }

  // number case
  char ch = *begin;
  if (std::isdigit(ch)) {
    return double_to_graph(begin, end);
  }

  // variable case
  if (std::distance(begin, end) == 1 && std::isalpha(ch)) {
    return variable_to_graph(begin, end);
  }

  // binary operation case
  if (ch == '(') {
    return binary_operation_to_graph(begin, end);
  }

  // unary operation case
  return unary_operation_to_graph(begin, end);
}