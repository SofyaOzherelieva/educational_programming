#pragma once

#include "buffer.h"

static const bool debug = true;

template<typename T>
class CStack {
  private:

    static constexpr size_t INITIAL_BUFFER_SIZE     = 1;
    static constexpr size_t REALLOCATE_COEFFICIENT  = 2;
    static constexpr size_t CONTRACTION_COEFFICIENT = 4;

    enum class ErrorTypes {
        OK                       = 0,
        TAINTED_FRONT_CANARY     = 1,
        TAINTED_BACK_CANARY      = 2,
        MISMATCH_BUFFER_CHECKSUM = 3,
        MISMATCH_STACK_CHECKSUM  = 4
    };

    const char *ErrorNames[5] =
                 {
                   "OK",
                   "TAINTED_FRONT_CANARY",
                   "TAINTED_BACK_CANARY",
                   "MISMATCH_BUFFER_CHECKSUM",
                   "MISMATCH_STACK_CHECKSUM"
                 };

  public:

    CStack() : amount_of_elements_(0),
               buffer_checksum_(0),
               buffer_(new CBuffer<T>(INITIAL_BUFFER_SIZE)) {

      stack_checksum_ = CalculateStackChecksum();
    };

    ~CStack() noexcept {
      delete buffer_;
    };

    bool Pop();

    void Push(const T &element);

    inline bool IsEmpty() const noexcept;

    inline bool IsFull() const noexcept;

    inline bool IsFilled() const noexcept;

  private:

    void Dump(ErrorTypes error_type) const;

    void IsError() const;

    ErrorTypes IsStackChecksumOk() const noexcept;

    ErrorTypes IsBufferChecksumOk() const noexcept;

    ErrorTypes IsFrontCanaryOk() const noexcept;

    ErrorTypes IsBackCanaryOk() const noexcept;

    void Reallocate(TypeOfReallocation type);

    size_t CalculateBufferChecksum() const noexcept;

    size_t CalculateStackChecksum() const noexcept;

    void AddToChecksum(const T &element, size_t *temp_checksum) noexcept;

    size_t       buffer_checksum_;
    size_t       stack_checksum_;
    CBuffer<T>   *buffer_;
    size_t       amount_of_elements_;
    std::hash<T> hash_func;
};


//////////////////////////////////////////////////////////////////////////////////////////


template<typename T>
bool CStack<T>::Pop() {
  IsError();
  if (IsEmpty()) {
    return false;
  }

  amount_of_elements_--;
  AddToChecksum((*buffer_)[amount_of_elements_], &buffer_checksum_);

  if (!IsFilled()) {
    Reallocate(TypeOfReallocation::CONTRACTION);
  }
  IsError();
  return true;
}

template<typename T>
void CStack<T>::Push(const T &element) {
  IsError();
  if (IsFull()) {
    Reallocate(TypeOfReallocation::EXTENSION);
  }
  (*buffer_)[amount_of_elements_] = element;
  amount_of_elements_++;
  AddToChecksum(element, &buffer_checksum_);
  IsError();
}

template<typename T>
inline bool CStack<T>::IsEmpty() const noexcept {
  return amount_of_elements_ == 0;
}

template<typename T>
inline bool CStack<T>::IsFull() const noexcept {
  return buffer_->GetBufferSize() <= amount_of_elements_;
}

template<typename T>
inline bool CStack<T>::IsFilled() const noexcept {
  return amount_of_elements_ >= buffer_->GetBufferSize() / CONTRACTION_COEFFICIENT;
}

template<typename T>
void CStack<T>::Dump(ErrorTypes error_type) const {
  if (error_type != ErrorTypes::OK) {
    std::cout << ErrorNames[static_cast<int>(error_type)] << '\n';

    std::cout << "--------------------------------------------------------\n";

    std::cout << "CStack: " << this;
    std::cout << "Buffer: " << buffer_;

    std::cout << "Buffer size: " << buffer_->GetBufferSize() << '\n';
    std::cout << "Number of elements in buffer: " << amount_of_elements_ << '\n';

    auto temp_front_canary = buffer_->GetFrontCanary();

    std::cout << "Front buffer canary: " << temp_front_canary
              << " Expected: " << 0xBAADF00D
              << " Is equal? - " << (temp_front_canary == 0xBAADF00D) << '\n';

    auto temp_stack_checksum = CalculateStackChecksum();

    std::cout << "CStack checksum: " << stack_checksum_
              << " Expected: " << stack_checksum_
              << " Is equal? - " << (temp_stack_checksum == stack_checksum_) << '\n';

    auto temp_buffer_checksum = CalculateBufferChecksum();

    std::cout << "Buffer checksum: " << temp_buffer_checksum
              << " Expected: " << buffer_checksum_
              << " Is equal? - " << (temp_buffer_checksum == buffer_checksum_) << '\n';

    std::cout << "Buffer elements: ";
    for (int i = 0; i < amount_of_elements_; i++) {
      std::cout << (*buffer_)[i] << ' ';
    }
    std::cout << '\n';

    auto temp_back_canary = buffer_->GetBackCanary();

    std::cout << "Front buffer canary: " << temp_back_canary
              << " Expected: " << 0xBAADF00D
              << " Is equal? - " << (temp_back_canary == 0xBAADF00D) << '\n';

    std::cout << "--------------------------------------------------------\n";
    exit(static_cast<int>(error_type));
  }
}

template<typename T>
void CStack<T>::IsError() const {
  if (debug) {
    Dump(IsStackChecksumOk());
    Dump(IsBufferChecksumOk());
    Dump(IsFrontCanaryOk());
    Dump(IsBackCanaryOk());
  }
}

template<typename T>
typename CStack<T>::ErrorTypes CStack<T>::IsStackChecksumOk() const noexcept {
  if (stack_checksum_ == CalculateStackChecksum()) {
    return ErrorTypes::OK;
  } else {
    return ErrorTypes::MISMATCH_STACK_CHECKSUM;
  }
}

template<typename T>
typename CStack<T>::ErrorTypes CStack<T>::IsBufferChecksumOk() const noexcept {
  if (buffer_checksum_ == CalculateBufferChecksum()) {
    return ErrorTypes::OK;
  } else {
    return ErrorTypes::MISMATCH_BUFFER_CHECKSUM;
  }
}

template<typename T>
typename CStack<T>::ErrorTypes CStack<T>::IsFrontCanaryOk() const noexcept {
  if (buffer_->CheckFrontCanary()) {
    return ErrorTypes::OK;
  } else {
    return ErrorTypes::TAINTED_BACK_CANARY;
  }
}

template<typename T>
typename CStack<T>::ErrorTypes CStack<T>::IsBackCanaryOk() const noexcept {
  if (buffer_->CheckBackCanary()) {
    return ErrorTypes::OK;
  } else {
    return ErrorTypes::TAINTED_FRONT_CANARY;
  }
}

template<typename T>
void CStack<T>::Reallocate(TypeOfReallocation type) {
  (*buffer_).Reallocate(type, REALLOCATE_COEFFICIENT, amount_of_elements_);
}

template<typename T>
size_t CStack<T>::CalculateBufferChecksum() const noexcept {
  size_t   temp_checksum = 0;
  for (int i             = 0; i < amount_of_elements_; i++) {
    temp_checksum ^= hash_func((*buffer_)[i]);
  }
  return temp_checksum;
}

template<typename T>
size_t CStack<T>::CalculateStackChecksum() const noexcept {
  size_t result = 0;
  result ^= reinterpret_cast<intptr_t>(buffer_);
  result ^= reinterpret_cast<intptr_t>(this);
  return result;
}

template<typename T>
void CStack<T>::AddToChecksum(const T &element, size_t *temp_checksum) noexcept {
  *temp_checksum ^= hash_func(element);
}
