#ifndef QUADRATIC_SOLVER_QUADRATIC_SOLVER_H
#define QUADRATIC_SOLVER_QUADRATIC_SOLVER_H

#include <cassert>
#include <complex>
#include <vector>

const double EPSILON = 0.001;

using std::vector;

/// Проверка на равенство двух чисел с плавающей точкой.
bool is_equal(double num1, double num2);

/*!
    \defgroup QuadraticSolver Решение квадратных уравнений
	  \author Sofia
	  \version 1.0
	  \date 10.09.18
    \brief Данный модуль предназначен для решения квадратных уравнений с действительными коэффициентами в комплексных числах.

    Уравнение решается при помощи вычисления дискриминанта.
*/


///@{
class QuadraticSolver {
  public:

    /*! \brief Конструктор класса QuadraticSolver
        \param coefficients коэффициенты квадратного уравнения
     */

    explicit QuadraticSolver(vector<double> &coefficients) : coefficients_(coefficients) {};

    ~QuadraticSolver() = default;

    /*!
     * Комплексные корни уравнения.
     */
    vector<std::complex<double>> answer_;

    /*! \brief Решает квадратное уравнение.
        \throws assert - если и коэффициент при x^2 и при x равны нулю

        Записывает ответ в public переменную класса answer_.
     */
    void quadratic_solver();

  private:
    const vector<double>& coefficients_;

    std::complex<double> linear_solver();
};
///@}



#endif //QUADRATIC_SOLVER_QUADRATIC_SOLVER_H