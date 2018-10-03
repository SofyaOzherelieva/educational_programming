#import "quadratic_solver.h"
#import "test.h"

/*!
 *
 * Решает линейное уравнение.
 *
 * @param coefficients - коэффициенты линейного уравнения.
 * @param roots - корни линейного уравнения
 * @return число корней; -1 - корней бесконечно много
 */

int simple_linear_solver(vector<double>& coefficients, vector<double>& roots){

  if(is_equal(coefficients[1], 0)){

    if(is_equal(coefficients[2], 0)){
      return -1;
    } else{
      return 0;
    }

  } else{
    roots[0] = -coefficients[2]/coefficients[1];
    return 1;
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

int simple_quadratic_solver(vector<double>& coefficients, vector<double>& roots){
  if(is_equal(coefficients[0], 0)){
    return simple_linear_solver(coefficients, roots);
  }else{
    double discriminant = coefficients[1] * coefficients[1] - 4 * coefficients[2] * coefficients[0];

    if(discriminant < 0){
      return 0;
    }

    double root1 = (-coefficients[1] + sqrt(discriminant)) / (2 * coefficients[0]);
    double root2 = (-coefficients[1] - sqrt(discriminant)) / (2 * coefficients[0]);

    roots[0] = root1;
    roots[1] = root2;

    if(is_equal(discriminant, 0)){
      return 1;
    }
    return 2;
  }
}


int main() {
  vector<vector<double>> test = {{1,      2,      1},
                                 {1,      2,      3},
                                 {0,      3,      1},
                                 {4,      3,      2},
                                 {0.1,    0.2,    0.1},
                                 {0.0001, 0.0001, 0.1},
                                 {0,      0,      2},
                                 {0,      0,      0}
  };
  //TestQuadraticSolver testQuadraticSolver(test);
  //testQuadraticSolver.test_quadratic_solver();

  std::cout << "Enter the coefficients of the quadratic equation.";

  vector<double> coefficients(3);

  for (int i = 0; i < 3; i++) {
    std::cin >> coefficients[i];
    assert(std::isfinite(coefficients[i]));
  }

  // Простое решение
  std::cout << "Решение простым способом: \n";
  vector<double> roots(2);
  int number_of_roots = simple_quadratic_solver(coefficients, roots);

  if(number_of_roots < 0){
    std::cout << "Бесконечное число решений\n";
  }

  if(number_of_roots == 0){
    std::cout << "Нет решений\n";
  }

  for(int i = 0; i < number_of_roots; i++){
    std::cout << roots[i] << "\n";
  }

  // Сложное решение с комплексными корнями.

  std::cout << "Решение сложным способом: \n";

  QuadraticSolver solver(coefficients);
  solver.quadratic_solver();
  for (auto answer: solver.answer_)
    std::cout << answer << "\n";
  return 0;
}
