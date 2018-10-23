#include <cstdio>
#include <iostream>
// если функция static она видна только из этого модуля

enum TypeOfReallocation {
  EXTENSION = 0,
  CONTRACTION = 1
};


bool debug = true;


template<typename T>
class Stack {
  public:
    const size_t INITIAL_BUFFER_SIZE = 1;
    const size_t REALLOCATE_COEFFICIENT = 2;
    const size_t CONTRACTION_COEFFICIENT = 4;

    const uint8_t STACK_CANARY = 0xBAADF00D;

  public:
    Stack() : buffer_size(INITIAL_BUFFER_SIZE), size(0) {
      buffer = new T[INITIAL_BUFFER_SIZE];
    };

    ~Stack() {
      delete buffer;
    };

    bool Pop() {
      if (IsEmpty()) {
        return false;
      }

      size--;
      if (!IsFilled()) {
        Reallocate(CONTRACTION);
      }
      Visualize();
      return true;
    }

    void Push(const T &element) {
      if (IsFull()) {
        Reallocate(EXTENSION);
      }
      buffer[size] = element;
      size++;
      Visualize();
    }

    bool IsEmpty() {
      return size == 0;
    }

    bool IsFull() {
      return buffer_size <= size;
    }

    bool IsFilled() {
      return size >= buffer_size / CONTRACTION_COEFFICIENT;
    }

    void Visualize() {
      std::cout << buffer_size << ' ' << size << ' ';
      for (int i = 0; i < size; i++) {
        std::cout << buffer[i] << ' ';
      }
      std::cout << '\n';
    }

  private:

    void Reallocate(TypeOfReallocation type) {
      size_t new_size = NULL;
      switch (type) {
        case EXTENSION:
          new_size = buffer_size * REALLOCATE_COEFFICIENT;
          break;
        case CONTRACTION:
          new_size = buffer_size / REALLOCATE_COEFFICIENT;
          break;
      }

      T *new_buffer = new T[new_size];
      for (int i = 0; i < size; i++) {
        new_buffer[i] = buffer[i];
      }
      delete buffer;
      buffer = new_buffer;
      buffer_size = new_size;
    }

    T *buffer = nullptr;
    size_t buffer_size = NULL;
    size_t size = NULL;

};

template <typename T>
struct Buffer{
  Buffer():{};

  T *buffer = nullptr;
  size_t buffer_size = NULL;
};