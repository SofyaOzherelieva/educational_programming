#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>

using std::cout;

const bool debug = true;
#define BUFFER_CANARY 0xBAADF00D

enum TypeOfReallocation {
  EXTENSION = 0,
  CONTRACTION = 1
};

enum ErrorTypes{
  OK = 0,
  FRONT_CANARY_ERROR = 1,
  BACK_CANARY_ERROR = 2,
  BUFFER_CHECKSUM_ERROR = 3,
  STACK_CHECKSUM_ERROR = 4
};

const char* ErrorNames[] =
  {
    "OK",
    "FRONT_CANARY_ERROR",
    "BACK_CANARY_ERROR",
    "BUFFER_CHECKSUM_ERROR",
    "STACK_CHECKSUM_ERROR"
  };

template<typename T>
struct CBuffer {
    explicit CBuffer(size_t buffer_size) : buffer_size_(buffer_size) {
      buffer_ = new T[buffer_size];
    }

    ~CBuffer() {
      delete[] buffer_;
    }

    CBuffer(const CBuffer&) = delete;
    CBuffer & operator=(const CBuffer&) = delete;

    T &operator[](int i) {
      if (i > buffer_size_) {
        std::cout << "ERROR: INDEX OUT OF RANGE";
      }
      return buffer_[i];
    }

    void Reallocate(TypeOfReallocation type, size_t realloc_coeff, size_t size) {
      size_t new_size = NULL;
      switch (type) {
        case EXTENSION:
          new_size = buffer_size_ * realloc_coeff;
          break;
        case CONTRACTION:
          new_size = buffer_size_ / realloc_coeff;
          size = new_size;
          break;
      }

      auto new_buffer = new T[new_size];

      for(int i = 0; i < size; i++){
        new_buffer[i] = buffer_[i];
      }
      delete[] buffer_;
      buffer_ = new_buffer;
      buffer_size_ = new_size;

    }

    size_t GetBufferSize(){
      return buffer_size_;
    }

    uint32_t GetFrontCanary(){
      return front_canary;
    }

    uint32_t GetBackCanary(){
      return back_canary;
    }

    bool CheckFrontCanary(){
      return GetFrontCanary() == BUFFER_CANARY;
    }

    bool CheckBackCanary(){
      return GetBackCanary() == BUFFER_CANARY;
    }

  private:
    uint32_t front_canary = BUFFER_CANARY;

    T *buffer_ = nullptr;
    size_t buffer_size_ = NULL;

    uint32_t back_canary = BUFFER_CANARY;
};

template<typename T>
class Stack {
  public:
    const size_t INITIAL_BUFFER_SIZE = 1;
    const size_t REALLOCATE_COEFFICIENT = 2;
    const size_t CONTRACTION_COEFFICIENT = 4;

  public:
    Stack() : amount_of_elements_(0), buffer_checksum_(0) {
      buffer_ = new CBuffer<T>(INITIAL_BUFFER_SIZE);
      stack_checksum_ = CalculateStackChecksum();
    };

    ~Stack() {
      buffer_->~CBuffer();
    };

    Stack(const Stack&) = delete;
    Stack & operator=(const Stack&) = delete;

    bool Pop() {
      IsError();
      if (IsEmpty()) {
        return false;
      }

      amount_of_elements_--;
      AddToChecksum((*buffer_)[amount_of_elements_], &buffer_checksum_);

      if (!IsFilled()) {
        Reallocate(CONTRACTION);
      }
      IsError();
      return true;
    }

    void Push(const T &element) {
      IsError();
      if (IsFull()) {
        Reallocate(EXTENSION);
      }
      (*buffer_)[amount_of_elements_] = element;
      amount_of_elements_++;
      AddToChecksum(element, &buffer_checksum_);
      IsError();
    }

    bool IsEmpty() {
      return amount_of_elements_ == 0;
    }

    bool IsFull() {
      return buffer_->GetBufferSize() <= amount_of_elements_;
    }

    bool IsFilled() {
      return amount_of_elements_ >= buffer_->GetBufferSize() / CONTRACTION_COEFFICIENT;
    }

  private:

    void Dump(ErrorTypes error_type){
      if(error_type != OK){
        std::cout << ErrorNames[error_type] << '\n';

        std::cout << "--------------------------------------------------------\n";

        std::cout << "Stack: " << this;
        std::cout << "Buffer: " << buffer_;

        std::cout << "Buffer size: " << buffer_->GetBufferSize() << '\n';
        std::cout << "Number of elements in buffer: " << amount_of_elements_<< '\n';

        auto temp_front_canary = buffer_->GetFrontCanary();

        std::cout << "Front buffer canary: " << temp_front_canary
                  << " Expected: " << BUFFER_CANARY
                  << " Is equal? - " << (temp_front_canary == BUFFER_CANARY) << '\n';

        auto temp_stack_checksum = CalculateStackChecksum();

        std::cout << "Stack checksum: " << stack_checksum_
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
                  << " Expected: " << BUFFER_CANARY
                  << " Is equal? - " << (temp_back_canary == BUFFER_CANARY) << '\n';

        std::cout << "--------------------------------------------------------\n";
        exit(error_type);
      }
    }

    void IsError(){
      if(debug) {
        Dump(IsStackChecksumOk());
        Dump(IsBufferChecksumOk());
        Dump(IsFrontCanaryOk());
        Dump(IsBackCanaryOk());
      }
    }

    ErrorTypes IsStackChecksumOk(){
      if(stack_checksum_ == CalculateStackChecksum()){
        return OK;
      }else{
        return STACK_CHECKSUM_ERROR;
      }
    }

    ErrorTypes IsBufferChecksumOk(){
      if(buffer_checksum_ == CalculateBufferChecksum()){
        return OK;
      }else{
        return BUFFER_CHECKSUM_ERROR;
      }
    }

    ErrorTypes IsFrontCanaryOk(){
      if(buffer_->CheckFrontCanary()){
        return OK;
      }else{
        return BACK_CANARY_ERROR;
      }
    }

    ErrorTypes IsBackCanaryOk(){
      if(buffer_->CheckBackCanary()){
        return OK;
      }else{
        return FRONT_CANARY_ERROR;
      }
    }

    void Reallocate(TypeOfReallocation type) {
      (*buffer_).Reallocate(type, REALLOCATE_COEFFICIENT, amount_of_elements_);
    }

    size_t CalculateBufferChecksum(){
      size_t temp_check_sum = 0;
      for(int i = 0; i < amount_of_elements_; i++){
        AddToChecksum((*buffer_)[i], &temp_check_sum);
      }
      return temp_check_sum;
    }

    size_t CalculateStackChecksum(){
      size_t result = 0;
      result ^= reinterpret_cast<intptr_t>(buffer_);
      result ^= reinterpret_cast<intptr_t>(this);
      return result;
    }

    void AddToChecksum(const T& element, size_t* temp_check_sum){
      *temp_check_sum ^= hash_func(element);
    }

    size_t buffer_checksum_ = NULL;
    size_t stack_checksum_ = NULL;
    CBuffer<T> *buffer_ = nullptr;
    size_t amount_of_elements_ = NULL; // кол-во элементов в буффере
    std::hash<T> hash_func;
};