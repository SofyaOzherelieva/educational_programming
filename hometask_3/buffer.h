#pragma once

#include <iostream>

enum class TypeOfReallocation {
    EXTENSION   = 0,
    CONTRACTION = 1
};

template<typename T>
struct CBuffer {

  public:

    explicit CBuffer(size_t buffer_size) : buffer_size_(buffer_size),
                                           buffer_(new T[buffer_size]) {}

    ~CBuffer() noexcept {
      delete[] buffer_;
    }

    T &operator[](int i) const;

    void Reallocate(TypeOfReallocation type, size_t realloc_coeff, size_t size);

    inline size_t GetBufferSize() const noexcept;

    inline uint32_t GetFrontCanary() const noexcept;

    inline uint32_t GetBackCanary() const noexcept;

    inline bool CheckFrontCanary() const noexcept;

    inline bool CheckBackCanary() const noexcept;

  private:
    const uint32_t front_canary = 0xBAADF00D;

    T      *buffer_     = nullptr;
    size_t buffer_size_ = NULL;

    const uint32_t back_canary = 0xBAADF00D;
};


//////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
T & CBuffer<T>::operator[](int i) const {
  if (i > buffer_size_) {
    std::cout << "ERROR: INDEX OUT OF RANGE";
  }
  return buffer_[i];
}

template<typename T>
void CBuffer<T>::Reallocate(TypeOfReallocation type, size_t realloc_coeff, size_t size) {
  size_t new_size = NULL;
  switch (type) {
    case TypeOfReallocation::EXTENSION:
      new_size = buffer_size_ * realloc_coeff;
      break;
    case TypeOfReallocation::CONTRACTION:
      new_size = buffer_size_ / realloc_coeff;
      size     = new_size;
      break;
    default:
      std::cout << "ERROR: INCORRECT TYPE OF REALLOCATION";
      break;
  }

  auto new_buffer = new T[new_size]{};

  for (int i = 0; i < size; i++) {
    new_buffer[i] = buffer_[i];
  }
  delete[] buffer_;
  buffer_      = new_buffer;
  buffer_size_ = new_size;
}

template<typename T>
inline size_t CBuffer<T>::GetBufferSize() const noexcept {
  return buffer_size_;
}

template<typename T>
inline uint32_t CBuffer<T>::GetFrontCanary() const noexcept {
  return front_canary;
}

template<typename T>
inline uint32_t CBuffer<T>::GetBackCanary() const noexcept {
  return back_canary;
}

template<typename T>
inline bool CBuffer<T>::CheckFrontCanary() const noexcept {
  return GetFrontCanary() == 0xBAADF00D;
}

template<typename T>
inline bool CBuffer<T>::CheckBackCanary() const noexcept {
  return GetBackCanary() == 0xBAADF00D;
}