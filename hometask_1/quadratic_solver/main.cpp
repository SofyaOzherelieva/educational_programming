#import "quadratic_solver.h"
#import "test.h"


enum {
  INF_ROOTS = -1, NO_ROOTS = 0, ONE_ROOT = 1, TWO_ROOTS = 2
};

/*!
 *
 * Решает линейное уравнение.
 *
 * @param coefficients - коэффициенты линейного уравнения.
 * @param roots - корни линейного уравнения
 * @return число корней; -1 - корней бесконечно много
 */

int simple_linear_solver(const vector<double> &coefficients, vector<double> *roots) {

  if (is_equal(coefficients[1], 0)) {

    if (is_equal(coefficients[2], 0)) {
      return INF_ROOTS;
    } else {
      return NO_ROOTS;
    }

  } else {
    (*roots).push_back(-coefficients[2] / coefficients[1]);
    return ONE_ROOT;
  }
}

/*!
 *
 * Решает квадратное уравнение.
 *
 * @param coefficients - коэффициенты квадратного уравнения.
 * @param roots - корни квадратного уравнения.
 * @return число корней; -1 - корней бесконечно много
 */

int simple_quadratic_solver(const vector<double> &coefficients, vector<double> *roots) {
  assert("Number of coefficients is not 3" && coefficients.size() == 3);

  if (is_equal(coefficients[0], 0)) {
    return simple_linear_solver(coefficients, roots);
  } else {
    double discriminant = coefficients[1] * coefficients[1] - 4 * coefficients[2] * coefficients[0];

    if (discriminant < 0) {
      return NO_ROOTS;
    }

    (*roots).push_back((-coefficients[1] + sqrt(discriminant)) / (2 * coefficients[0]));
    (*roots).push_back((-coefficients[1] - sqrt(discriminant)) / (2 * coefficients[0]));

    if (is_equal(discriminant, 0)) {
      return ONE_ROOT;
    }
    return TWO_ROOTS;
  }
}

void output(int number_of_roots, const vector<double> &roots) {
  assert("Number of roots is bigger than size of root vector." && number_of_roots <= roots.size());

  if (number_of_roots == INF_ROOTS) {
    std::cout << "Бесконечное число решений\n";
  }

  if (number_of_roots == NO_ROOTS) {
    std::cout << "Нет решений\n";
  }

  for (int i = 0; i < number_of_roots; i++) {
    std::cout << roots[i] << "\n";
  }
}


int main() {

  vector <vector<double>> test = {{1,      2,      1},
                                  {1,      2,      3},
                                  {0,      3,      1},
                                  {4,      3,      2},
                                  {0.1,    0.2,    0.1},
                                  {0.0001, 0.0001, 0.1},
                                  {0,      0,      2},
                                  {0,      0,      0}};
  //TestQuadraticSolver testQuadraticSolver(test);
  //testQuadraticSolver.test_quadratic_solver();

  std::cout << "Enter the coefficients of the quadratic equation.";

  vector<double> coefficients(3);

  for (int i = 0; i < 3; i++) {
    std::cin >> coefficients[i];
    if (!std::cin) {
      assert("Incorrect input" && 0);
    }
    assert(std::isfinite(coefficients[i]));
  }

  // Простое решение

  std::cout << "Решение простым способом: \n";
  vector<double> roots;
  int number_of_roots = simple_quadratic_solver(coefficients, &roots);

  output(number_of_roots, roots);

  // Сложное решение с комплексными корнями.

  std::cout << "Решение сложным способом: \n";

  QuadraticSolver solver(coefficients);
  solver.quadratic_solver();
  for (auto answer: solver.answer_) {
    std::cout << answer << "\n";
  }
  return 0;
}
