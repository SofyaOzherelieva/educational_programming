#import "test.h"
#import "quadratic_solver.h"


void TestQuadraticSolver::better_assert(vector<double>& coefficients) {

  test_num_++;

  QuadraticSolver quadraticSolver(coefficients);

  quadraticSolver.quadratic_solver();

  for (int i = 0; i < quadraticSolver.answer_.size(); i++) {
    assert(
      std::norm(quadraticSolver.answer_[i] * quadraticSolver.answer_[i] * coefficients[0] +
                quadraticSolver.answer_[i] * coefficients[1] + coefficients[2]) < EPSILON and "The test failed");
  }
}


void TestQuadraticSolver::test_quadratic_solver(){
  vector<double> coefficients;

  for(auto& coefficients: test_data_){
    better_assert(coefficients);
    std::cerr << "The test №" << test_num_ << " OK" << '\n';
  }
};