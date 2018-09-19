#ifndef QUADRATIC_SOLVER_TEST_H
#define QUADRATIC_SOLVER_TEST_H


#include <cassert>
#include <complex>
#include <iostream>
#include <vector>

#define EPSILON 0.001


using std::vector;

/*!
    \defgroup Test Тестирование
    \brief Данный модуль предназначен для тестирования Quadratic Solver.
    \author Sofia
	  \version 1.0
	  \date 10.09.18

    Тестирование проходит путем подставление результата в исходное квадратное уравнение.
*/

///@{
class TestQuadraticSolver {
  public:

    /*! \brief Конструктор класса TestQuadraticSolver
        \param test_data массив тестов, которые представляют собой действительные коэффициенты квадратного уравнения
     */
    explicit TestQuadraticSolver(vector <vector<double>> &test_data) : test_data_(test_data), test_num_(0) {};

    ~TestQuadraticSolver() = default;

    /*! \brief Последовательно выполняет тесты.
        \throws assert - если ответ осноной программы не проходит тестирование.

        Если тест пройден успешно выводит информацию об этом в stderr.
     */
    void test_quadratic_solver();

  private:
    int test_num_;
    vector <vector<double>> test_data_;

    void better_assert(vector<double> &coefficients);
};
///@}


#endif //QUADRATIC_SOLVER_TEST_H
