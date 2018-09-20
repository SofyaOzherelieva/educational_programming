#import "quadratic_solver.h"

std::complex<double> QuadraticSolver::linear_solver() {
  assert("The equation has an infinite number of solutions." and std::abs(coefficients_[0]) < EPSILON and
         (std::abs(coefficients_[1]) > EPSILON or std::abs(coefficients_[2]) > EPSILON));

  assert(
    "The equation has no solution." and std::abs(coefficients_[0]) < EPSILON and std::abs(coefficients_[1]) > EPSILON);

  return -coefficients_[2] / coefficients_[1];
}

void QuadraticSolver::quadratic_solver() {
  /*c - coefficients of the quadratic equation*/

  /* Okay, do not compare double-like numbers with 0, it's inaccurate, plz think of eps */
  if (std::abs(coefficients_[0]) < EPSILON) {
    answer_.push_back(linear_solver());

  } else {
    std::complex<double> discriminant = coefficients_[1] * coefficients_[1] - 4 * coefficients_[2] * coefficients_[0];

    std::complex<double> root1 = (-coefficients_[1] + sqrt(discriminant)) / (2 * coefficients_[0]);
    std::complex<double> root2 = (-coefficients_[1] - sqrt(discriminant)) / (2 * coefficients_[0]);

    answer_.push_back(root1);
    answer_.push_back(root2);
  }
}
