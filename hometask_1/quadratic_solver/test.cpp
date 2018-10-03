#import "test.h"
#import "quadratic_solver.h"


void TestQuadraticSolver::better_assert(vector<double> &coefficients) {

  test_num_++;

  QuadraticSolver quadraticSolver(coefficients);

  quadraticSolver.quadratic_solver();

  for (auto answer: quadraticSolver.answer_) {
    assert("The test failed" and
           is_equal(std::norm(answer * answer * coefficients[0] + answer * coefficients[1] + coefficients[2]), 0));
  }
}


void TestQuadraticSolver::test_quadratic_solver() {
  for (auto coefficients: test_data_) {
    better_assert(coefficients);
    std::cerr << "The test №" << test_num_ << " OK" << '\n';
  }
};