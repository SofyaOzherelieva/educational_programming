#ifndef ONEGIN_SORT_BOOK_H
#define ONEGIN_SORT_BOOK_H

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <functional>

using namespace std::placeholders;

/*!
    \defgroup BookSort
	  \author Sofia
	  \version 1.0
	  \date 16.09.18
    \brief Данный модуль предназначен для прямой и обратной сортировки строк в книге.

    Сортировка проходит без учета порядка и знаков препинания.
*/

///@{

/*!
 \struct Line
 \brief Структура строки в текте.
 */
struct Line {

  /// Позиция строки в тексте относительно начала текста.
  int start_index_{0};
  /// Указатель на начало текта в памяти.
  char *buffer_{nullptr};

  /*!
   \brief Пропускает знаки препинания.
   \param index текущая позиция в строке
   \param is_reverse флаг направления чтения строки

   Перемещает указатель на позицию в строке со знака препинания до ближашей буквы в направлении,
   заданном параметром is_reverse.
   */
  void skip_punctuation_marks(int *index, bool is_reverse) const;

  /*!
   * @return длину данной строки
   *
   * Считает символы с начала строки до первого встретившегося символа '\0'.
   */
  int length_of_line() const;

  /*!
   * @return является ли строка пустой
   *
   * Пустая строка - не содержит букв.
   */
  bool is_empty() const;
};
///@}

///@{

/*!
 * \class Book
 * \brief Класс для хранения текста и работы с ним.
 */

class Book {
  public:
    /*!
     *
     * @param filename дескриптор файла для чтения
     */

    explicit Book(const char *filename);

    ~Book();

    /// Сортирует строки без учета знаком препинания и регистра слева направо.
    void sort();

    /// Сравнивает строки без учета знаком препинания и регистра справа налево.
    void reverse_sort();

    /*!
     *
     * @param filename дескриптор файла для записи
     * @warning не выводит пустые строки
     *
     * Выводит текст с порядком строк, заданным сортировкой в указанный файл.
     */
    void output(const char *filename) const;

  private:
    FILE *book_{nullptr};

    char *buffer_{nullptr};
    size_t book_size_{0};

    Line *array_of_lines_{nullptr};
    size_t num_of_lines_{0};

    /*!
     * Меняет '\n' на '\0' в буфере. Для упрощенного вывода конкретной строки.
     */
    void change_n_to_o();

    /*!
     * Заполняет array_of_lines_.
     */
    void book_indexing();

    bool comparator(const Line &line1, const Line &line2);

    bool reverse_comparator(const Line &line1, const Line &line2);
};
///@}

#endif //ONEGIN_SORT_BOOK_H
