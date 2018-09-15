#import "quadratic_solver.h"

std::complex<double> QuadraticSolver::linear_solver() {
  assert(coefficients_[0] == 0 and coefficients_[1] != 0 and "The first two coefficients must be nonzero");
  return -coefficients_[2] / coefficients_[1];
}

void QuadraticSolver::quadratic_solver() {
  /*c - coefficients of the quadratic equation*/
  if (coefficients_[0] == 0) {
    answer_.push_back(linear_solver());

  } else {
    std::complex<double> discriminant = coefficients_[1] * coefficients_[1] - 4 * coefficients_[2] * coefficients_[0];

    std::complex<double> root1 = (-coefficients_[1] + sqrt(discriminant)) / (2 * coefficients_[0]);
    std::complex<double> root2 = (-coefficients_[1] - sqrt(discriminant)) / (2 * coefficients_[0]);

    answer_.push_back(root1);
    answer_.push_back(root2);
  }
}