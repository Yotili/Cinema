#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP

#include <cstddef>   // for size_t
#include <utility>   // for std::move

template <typename T>
class MyVector {
private:
    T* data_;
    size_t  size_;
    size_t  capacity_;

    void grow() {
        size_t newCap = capacity_ ? capacity_ * 2 : 1;
        T* newData = static_cast<T*>(::operator new[](newCap * sizeof(T)));

        // Move-construct existing elements
        for (size_t i = 0; i < size_; ++i) {
            new (newData + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        // Free old raw storage
        ::operator delete[](data_);
        data_ = newData;
        capacity_ = newCap;
    }

public:
    // Constructors / Destructor
    MyVector()
        : data_(nullptr), size_(0), capacity_(0) {
    }

    ~MyVector() {
        // Destroy contained objects
        for (size_t i = 0; i < size_; ++i)
            data_[i].~T();
        // Free raw storage
        ::operator delete[](data_);
    }

    MyVector(const MyVector& other)
        : data_(nullptr), size_(other.size_), capacity_(other.capacity_)
    {
        if (capacity_) {
            data_ = static_cast<T*>(::operator new[](capacity_ * sizeof(T)));
            for (size_t i = 0; i < size_; ++i)
                new (data_ + i) T(other.data_[i]);
        }
    }

    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            // Destroy current
            for (size_t i = 0; i < size_; ++i)
                data_[i].~T();
            ::operator delete[](data_);

            // Copy from other
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = nullptr;
            if (capacity_) {
                data_ = static_cast<T*>(::operator new[](capacity_ * sizeof(T)));
                for (size_t i = 0; i < size_; ++i)
                    new (data_ + i) T(other.data_[i]);
            }
        }
        return *this;
    }

    MyVector(MyVector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.size_ = other.capacity_ = 0;
    }

    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            // Destroy current
            for (size_t i = 0; i < size_; ++i)
                data_[i].~T();
            ::operator delete[](data_);

            // Steal data
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = other.capacity_ = 0;
        }
        return *this;
    }

    // Element access
    T& operator[](size_t idx) { return data_[idx]; }
    const T& operator[](size_t idx) const { return data_[idx]; }

    // Iterators
    T* begin() { return data_; }
    T* end() { return data_ + size_; }
    const T* begin() const { return data_; }
    const T* end()   const { return data_ + size_; }

    // Capacity
    size_t size()  const { return size_; }
    bool   empty() const { return size_ == 0; }

    // Modifiers
    void push_back(const T& value) {
        if (size_ == capacity_) grow();
        new (data_ + size_) T(value);
        ++size_;
    }

    void push_back(T&& value) {
        if (size_ == capacity_) grow();
        new (data_ + size_) T(std::move(value));
        ++size_;
    }

    void removeAt(size_t idx) {
        if (idx >= size_) return;
        // Destroy the idx element
        data_[idx].~T();
        // Move-shift remainder
        for (size_t i = idx; i + 1 < size_; ++i) {
            new (data_ + i) T(std::move(data_[i + 1]));
            data_[i + 1].~T();
        }
        --size_;
    }

    void clear() {
        // Destroy all elements
        for (size_t i = 0; i < size_; ++i)
            data_[i].~T();
        size_ = 0;
        // (We keep capacity_ and data_ for reuse.)
    }
};

#endif
