#import "quadratic_solver.h"
#import "test.h"


int main() {

  vector<vector<double>> test = {{1, 2, 1},
                                  {1, 2, 3},
                                  {0, 3, 1},
                                  {4, 3, 2},
                                  {0, 0, 2}};
  /* you completely got away with input parsing, I wish you had done it, but okay */
  TestQuadraticSolver testQuadraticSolver(test);
  testQuadraticSolver.test_quadratic_solver();
  return 0;
}
