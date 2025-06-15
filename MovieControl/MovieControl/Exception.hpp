#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>

class Exception : public std::exception {
public:
    explicit Exception(const char* msg) noexcept
        : message_(msg)
    {
    }

    const char* what() const noexcept override {
        return message_;
    }

private:
    const char* message_;
};

#endif // EXCEPTION_HPP