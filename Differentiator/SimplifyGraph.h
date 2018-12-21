#pragma once

#include "ExpressionNode.h"

namespace Differentiator::SimplifyGraph{

double calculate_operation(OperationType operation_type, double arg1, double arg2 = NULL);

bool simplify_operations_with_constants(ExprNode &node);
bool simplify_multiply_one(ExprNode &node);
bool simplify_divide_one(ExprNode &node);
bool simplify_multiply_zero(ExprNode &node);
bool simplify_add_zero(ExprNode &node);

void simplify(ExprNode &node);
}

double Differentiator::SimplifyGraph::calculate_operation(OperationType operation_type,
                                                          double arg1, double arg2) {
  double result;
#define OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)  \
  if ((NUM) == operation_type){   \
    CODE; \
  } \

#include "Operations.h"

#undef OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)
  return result;
}


bool Differentiator::SimplifyGraph::simplify_operations_with_constants(ExprNode &node) {
  bool remained = true;

  if (node.left_) {
    remained &= simplify_operations_with_constants(*node.left_);
  }
  if (node.right_) {
    remained &= simplify_operations_with_constants(*node.right_);
  }

  if (!is_type(node, OPERATION_NODE) || !is_type(*node.left_, CONSTANT_NODE)) {
    return remained;
  }

  if (is_argc(node, 1)) {

    double arg      = node.left_->constant_;
    double constant = calculate_operation(node.operation_type_, arg);
    node.convert_to_constant(constant);

    remained = false;
  } else if (is_argc(node, 2)) {

    if (!is_type(*node.right_, CONSTANT_NODE)) {
      return remained;
    }

    double arg1     = node.left_->constant_;
    double arg2     = node.right_->constant_;
    double constant = calculate_operation(node.operation_type_, arg1, arg2);
    node.convert_to_constant(constant);
    remained = false;

  }

  return remained;
}

bool Differentiator::SimplifyGraph::simplify_multiply_one(ExprNode &node) {
  bool remained = true;

  if (node.left_) {
    remained &= simplify_multiply_one(*node.left_);
  }

  if (node.right_) {
    remained &= simplify_multiply_one(*node.right_);
  }

  if (!is_op(node, MULTIPLY_OP)) {
    return remained;
  }

  if (is_type(*node.left_, CONSTANT_NODE) && are_equal(node.left_->constant_, 1)) {
    node    = std::move(*node.right_);
    remained = false;
  } else if (is_type(*node.right_, CONSTANT_NODE) && are_equal(node.right_->constant_, 1)) {
    node    = std::move(*node.left_);
    remained = false;
  }

  return remained;
}

bool Differentiator::SimplifyGraph::simplify_divide_one(ExprNode &node) {
  bool remained = true;

  if (node.left_) {
    remained &= simplify_divide_one(*node.left_);
  }
  if (node.right_) {
    remained &= simplify_divide_one(*node.right_);
  }

  if (!is_op(node, DIVIDE_OP)) {
    return remained;
  }

  if (is_type(*node.right_, CONSTANT_NODE) && are_equal(node.right_->constant_, 1)) {
    node    = std::move(*node.left_);
    remained = false;
  }

  return remained;
}

bool Differentiator::SimplifyGraph::simplify_multiply_zero(ExprNode &node) {
  bool remained = true;

  if (node.left_) {
    remained &= simplify_multiply_zero(*node.left_);
  }
  if (node.right_) {
    remained &= simplify_multiply_zero(*node.right_);
  }

  if (!is_op(node, MULTIPLY_OP)) {
    return remained;
  }

  if (is_type(*node.left_, CONSTANT_NODE) && are_equal(node.left_->constant_, 0) ||
      is_type(*node.right_, CONSTANT_NODE) && are_equal(node.right_->constant_, 0)) {
    node    = std::move(ExprNode(0.));
    remained = false;
  }

  return remained;
}

bool Differentiator::SimplifyGraph::simplify_add_zero(ExprNode &node) {
  bool remained = true;

  if (node.left_) {
    remained &= simplify_add_zero(*node.left_);
  }
  if (node.right_) {
    remained &= simplify_add_zero(*node.right_);
  }

  if (!is_op(node, ADD_OP)) {
    return remained;
  }

  if (is_type(*node.left_, CONSTANT_NODE) && are_equal(node.left_->constant_, 0)) {
    node    = std::move(*node.right_);
    remained = false;
  } else if (is_type(*node.right_, CONSTANT_NODE) && are_equal(node.right_->constant_, 0)) {
    node    = std::move(*(node.left_));
    remained = false;
  }

  return remained;
}

void Differentiator::SimplifyGraph::simplify(ExprNode &node) {
  bool remaind = false;
  while (!remaind) {
    remaind = true;
    remaind &= simplify_operations_with_constants(node);
    remaind &= simplify_multiply_one(node);
    remaind &= simplify_divide_one(node);
    remaind &= simplify_multiply_zero(node);
    remaind &= simplify_add_zero(node);
  }
}