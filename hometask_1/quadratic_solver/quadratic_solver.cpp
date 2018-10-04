#import "quadratic_solver.h"

bool is_equal(double num1, double num2) {
  return std::fabs(num1 - num2) < EPSILON;
}


std::complex<double> QuadraticSolver::linear_solver() {
  assert("The equation has an infinite number of solutions." && is_equal(coefficients_[0], 0) &&
         (!is_equal(coefficients_[1], 0) || !is_equal(coefficients_[2], 0)));

  assert(
    "The equation has no solution." && is_equal(coefficients_[0], 0) && !is_equal(coefficients_[1], 0));

  return -coefficients_[2] / coefficients_[1];
}

void QuadraticSolver::quadratic_solver() {

  if (is_equal(coefficients_[0], 0)) {
    answer_.push_back(linear_solver());
  } else {
    std::complex<double> discriminant = coefficients_[1] * coefficients_[1] - 4 * coefficients_[2] * coefficients_[0];

    std::complex<double> root1 = (-coefficients_[1] + sqrt(discriminant)) / (2 * coefficients_[0]);
    std::complex<double> root2 = (-coefficients_[1] - sqrt(discriminant)) / (2 * coefficients_[0]);

    answer_.push_back(root1);
    answer_.push_back(root2);
  }
}
