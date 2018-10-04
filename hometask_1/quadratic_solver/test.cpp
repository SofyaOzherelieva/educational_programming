#import "test.h"
#import "quadratic_solver.h"


void TestQuadraticSolver::better_assert(const vector<double> &coefficients) {

  test_num_++;

  QuadraticSolver quadraticSolver(coefficients);

  quadraticSolver.quadratic_solver();

  for (auto answer: quadraticSolver.answer_) {
    assert("The test failed" &&
           (!is_equal(answer.real(), 0) or !is_equal(answer.imag(), 0)));
  }
}


void TestQuadraticSolver::test_quadratic_solver() {
  for (auto coefficients: test_data_) {
    better_assert(coefficients);
    std::cerr << "The test №" << test_num_ << " OK" << '\n';
  }
};