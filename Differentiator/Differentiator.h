#pragma once

#include "GraphVisualizer.h"
#include "ExpressionToGraph.h"
#include "GraphToExpression.h"
#include "SimplifyGraph.h"

namespace Differentiator {

#define diffL diff(*node.left_)
#define diffR diff(*node.right_)
#define copyL node.left_->copy()
#define copyR node.right_->copy()

std::unique_ptr<ExprNode> diff(const ExprNode &node) {

#define OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)  \
  if ((NUM) == node.operation_type_){   \
    DIFF_CODE; \
  } \

#include "Operations.h"

#undef OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE)

  assert(false && "Incorrect node.");
}
}