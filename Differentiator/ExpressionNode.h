#pragma once

#include <cmath>
#include <memory>
#include <cassert>


#define L(node) ((node).left_)
#define R(node) ((node).right_)

#define is_type(node, type) ((node).node_type_ == (type))
#define is_op(node, op) ((node).operation_type_ == (op))
#define is_argc(node, argc) (op_argc[(node).operation_type_] == (argc))

constexpr double EPS = 1e-6;

bool are_equal(double x, double y) {
  return fabs(x - y) < EPS;
}

enum NodeType {
  CONSTANT_NODE,
  VARIABLE_NODE,
  OPERATION_NODE,
};

enum OperationType {
#define OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE) NUM,

#include "Operations.h"

#undef OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)
};

short const op_argc[] = {
#define OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE) ARGC,

#include "Operations.h"

#undef OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)
};

struct ExprNode {

  std::unique_ptr<ExprNode> left_;
  std::unique_ptr<ExprNode> right_;

  NodeType node_type_;

  OperationType operation_type_;
  double        constant_;
  char          variable_;

  ExprNode() = delete;

  explicit ExprNode(OperationType operation_type,
                    std::unique_ptr<ExprNode> left,
                    std::unique_ptr<ExprNode> right = nullptr) :
    left_(std::move(left)),
    right_(std::move(right)),
    node_type_(OPERATION_NODE),
    operation_type_(operation_type),
    constant_(NULL),
    variable_(NULL) {}

  explicit ExprNode(double constant) :
    left_(nullptr),
    right_(nullptr),
    node_type_(CONSTANT_NODE),
    operation_type_(CONST_OP),
    constant_(constant),
    variable_(NULL) {}

  explicit ExprNode(char variable) :
    left_(nullptr),
    right_(nullptr),
    node_type_(VARIABLE_NODE),
    operation_type_(VAR_OP),
    constant_(NULL),
    variable_(variable) {}

  ExprNode(ExprNode &&node) noexcept :
    node_type_(node.node_type_),
    operation_type_(node.operation_type_),
    constant_(node.constant_),
    variable_(node.variable_) {

    left_  = std::move(node.left_);
    right_ = std::move(node.right_);
  }

  ExprNode(const ExprNode &node) noexcept :
    left_(nullptr),
    right_(nullptr),
    node_type_(node.node_type_),
    operation_type_(node.operation_type_),
    constant_(node.constant_),
    variable_(node.variable_) {}

  ExprNode &operator=(ExprNode &&node) noexcept {
    node_type_      = node.node_type_;
    operation_type_ = node.operation_type_;
    constant_       = node.constant_;
    variable_       = node.variable_;

    left_  = std::move(node.left_);
    right_ = std::move(node.right_);
    return *this;
  }

  std::unique_ptr<ExprNode> copy() const {
    return copy(*this);
  }

  static std::unique_ptr<ExprNode> copy(const ExprNode &node) {
    auto node_copy = std::make_unique<ExprNode>(node);
    if (node.left_) {
      node_copy->left_ = std::move(copy(*node.left_));
    }
    if (node.right_) {
      node_copy->right_ = std::move(copy(*node.right_));
    }
    return node_copy;
  }

  void convert_to_constant(double constant) {
    node_type_      = CONSTANT_NODE;
    operation_type_ = CONST_OP;
    constant_       = constant;
  }
};


std::unique_ptr<ExprNode> operator+(std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right) {
  return std::make_unique<ExprNode>(ADD_OP, std::move(left), std::move(right));
}

std::unique_ptr<ExprNode> operator-(std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right) {
  return std::make_unique<ExprNode>(SUBTRACT_OP, std::move(left), std::move(right));
}

std::unique_ptr<ExprNode> operator*(std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right) {
  return std::make_unique<ExprNode>(MULTIPLY_OP, std::move(left), std::move(right));
}

std::unique_ptr<ExprNode> operator/(std::unique_ptr<ExprNode> left, std::unique_ptr<ExprNode> right) {
  return std::make_unique<ExprNode>(DIVIDE_OP, std::move(left), std::move(right));
}

std::unique_ptr<ExprNode> sin(std::unique_ptr<ExprNode> left) {
  return std::make_unique<ExprNode>(SIN_OP, std::move(left));
}

std::unique_ptr<ExprNode> cos(std::unique_ptr<ExprNode> left) {
  return std::make_unique<ExprNode>(COS_OP, std::move(left));
}

std::unique_ptr<ExprNode> log(std::unique_ptr<ExprNode> left) {
  return std::make_unique<ExprNode>(LOG_OP, std::move(left));
}