#import "quadratic_solver.h"
#import "test.h"


int main() {
  vector <vector<double>> test = {{1,      2,      1},
                                  {1,      2,      3},
                                  {0,      3,      1},
                                  {4,      3,      2},
                                  {0.1,    0.2,    0.1},
                                  {0.0001, 0.0001, 0.1},
                                  {0,      0,      2}
  };
  /* you completely got away with input parsing, I wish you had done it, but okay */
  TestQuadraticSolver testQuadraticSolver(test);
  testQuadraticSolver.test_quadratic_solver();

  std::cout << "Enter the coefficients of the quadratic equation.";

  vector<double> coefficients(3);

  for (int i = 0; i < 3; i++) {
    std::cin >> coefficients[i];
  }

  QuadraticSolver solver(coefficients);
  solver.quadratic_solver();
  for (auto answer: solver.answer_)
    std::cout << answer;

  return 0;
}
