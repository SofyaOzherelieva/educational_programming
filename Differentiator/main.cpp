#include "Differentiator.h"

// Дифференцирование происходит по переменой x!

int main() {
  std::string input_expr = "";
  std::cout << "Введите выражение: ";
  std::cin >> input_expr;

  std::unique_ptr<ExprNode> expr;

  // Строим граф по выражению
  expr = Differentiator::ExpressionToGraph::expression_to_graph(input_expr.begin(), input_expr.end());
  Differentiator::Visualizer::visualize_tree(*expr, "expressionGraph");

  // Строим граф производной по графу выражения
  auto der_expr = Differentiator::diff(*expr);
  Differentiator::Visualizer::visualize_tree(*der_expr, "diffGraph");

  // Упрощаем граф производной
  Differentiator::SimplifyGraph::simplify(*der_expr);
  Differentiator::Visualizer::visualize_tree(*der_expr, "diffSimplGraph");

  // Выводим производную в строку
  auto der_expr_str = Differentiator::GraphToExpression::serialize_string(*der_expr);
  std::cout << "Производная: " << der_expr_str << "\n";

  return 0;

}
