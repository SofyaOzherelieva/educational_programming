//
// Created by sofya on 20.09.18.
//

#ifndef ONEGIN_SORT_TEST_H
#define ONEGIN_SORT_TEST_H

/*!
    \defgroup TestBookSort
	  \author Sofia
	  \version 1.0
	  \date 19.09.18
    \brief Данный модуль предназначен для тестирования сортировки строк в книге.
*/

///@{

/*!
 * \class Book
 * \brief Класс для хранения тестирования сортировки строк в книге.
 */

class TestBookSort{
  public:
    //TestBookSort(Book& book_for_test): book_for_test_(book_for_test){};
    ~TestBookSort() = default;

    /*!
     * \brief Порядок строк после сортировки.
     * \throw
     *
     * Проверяем результат сравнения всех пар строк в тексте.
     */
    void test_comparator();

    /*!
     * \brief Порядок строк после сортировки с конца.
     * \throw
     *
     * Проверяем результат сравнения с концов всех пар строк в тексте.
     */
    void test_reverse_comparator();

    /*!
     * \brief Проверяем результат сортировки всех строк в тексте.
     * \throw
     *
     * Наличие непустых строк из исходного текста.
     */
    void test_sort();

    /*!
     * \brief Проверяем результат сортировки с концов всех строк в тексте.
     * \throw
     *
     * Наличие непустых строк из исходного текста.
     */
    void test_reverse_sort();
  private:
    char* source_book;
    //Book& book_for_test_;

};
///@}

#endif //ONEGIN_SORT_TEST_H
