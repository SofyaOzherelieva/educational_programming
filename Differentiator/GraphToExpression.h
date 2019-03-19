#pragma once

#include <iostream>
#include <string>
#include "ExpressionNode.h"

namespace Differentiator::GraphToExpression {

std::string serialize(const ExprNode &node, std::string &str);

std::string constant_to_expression(const ExprNode &node, std::string &str) {
  auto constant_str = std::to_string(node.constant_);
  str.append(constant_str);
  return str;
}

std::string variable_to_expression(const ExprNode &node, std::string &str) {
  str.append(1, node.variable_);
  return str;
}

std::string operation_name_to_expression(const ExprNode &node, std::string &str) {
#define OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)  \
  if ((NUM) == node.operation_type_){   \
    str.append((NAME)); \
  } \


#include "Operations.h"

#undef OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)
  return str;
}

std::string unary_operation_to_expression(const ExprNode &node, std::string &str) {
  str = operation_name_to_expression(node, str);
  str.append(1, '(');
  str = serialize(*L(node), str);
  str.append(1, ')');
  return str;
}

std::string binary_operation_to_expression(const ExprNode &node, std::string str) {
  str.append(1, '(');
  str = serialize(*L(node), str);
  str.append(1, ')');

  str = operation_name_to_expression(node, str);

  str.append(1, '(');
  str = serialize(*R(node), str);
  str.append(1, ')');

  return str;
}

std::string serialize(const ExprNode &node, std::string &str) {
  switch (node.node_type_) {
    case CONSTANT_NODE:
      return constant_to_expression(node, str);
    case VARIABLE_NODE:
      return variable_to_expression(node, str);
    case OPERATION_NODE: {

#define OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)  \
  if ((NUM) == node.operation_type_){   \
    if((ARGC) == 1){\
      return unary_operation_to_expression(node, str); \
    }\
    if((ARGC) == 2){\
      return binary_operation_to_expression(node, str); \
    }\
  } \


#include "Operations.h"

#undef OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)
    }
    default:
      assert(false);
  }
  return str;
}

std::string serialize_string(const ExprNode &node) {
  std::string str;
  str = serialize(node, str);
  return str;
}
}