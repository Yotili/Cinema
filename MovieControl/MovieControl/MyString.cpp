#define _CRT_SECURE_NO_WARNINGS

#include "MyString.h"
#include <cstring>

// Copies data from another MyString object
void MyString::copyFrom(const MyString& other) {
    data = new char[other.length + 1];
    std::strcpy(data, other.data);
    length = other.length;
}

// Frees the allocated memory for the string
void MyString::free() {
    delete[] data;
}

// Default constructor: creates an empty string
MyString::MyString() : data(new char[1]), length(0) {
    data[0] = '\0';
}

// Constructor from C-string
MyString::MyString(const char* str) {
    length = std::strlen(str);
    data = new char[length + 1];
    std::strcpy(data, str);
}

// Copy constructor
MyString::MyString(const MyString& other) {
    copyFrom(other);
}

// Assignment operator
MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }
    return *this;
}

// Destructor: frees allocated memory
MyString::~MyString() {
    free();
}

// Returns the length of the string
size_t MyString::size() const {
    return length;
}

// Returns the C-string representation
const char* MyString::c_str() const {
    return data;
}

// Equality operator: compares two MyString objects
bool MyString::operator==(const MyString& other) const {
    return std::strcmp(data, other.data) == 0;
}

// Inequality operator
bool MyString::operator!=(const MyString& other) const {
    return !(*this == other);
}

// Concatenation operator: returns a new MyString that is the concatenation of two MyStrings
MyString MyString::operator+(const MyString& other) const {
    size_t newLen = length + other.length;
    char* newData = new char[newLen + 1];
    std::strcpy(newData, data);
    std::strcat(newData, other.data);

    MyString result(newData);
    delete[] newData;
    return result;
}

// Indexing operator (non-const): returns reference to character at index, clamps if out of range
char& MyString::operator[](size_t index) {
    // if out of range, clamp to last valid index (or 0 if empty)
    if (index >= length) {
        return data[length > 0 ? length - 1 : 0];
    }
    return data[index];
}

// Indexing operator (const): returns const reference to character at index, clamps if out of range
const char& MyString::operator[](size_t index) const {
    // same clamping behavior
    if (index >= length) {
        return data[length > 0 ? length - 1 : 0];
    }
    return data[index];
}

// Output stream operator: prints the string to the output stream
std::ostream& operator<<(std::ostream& os, const MyString& str) {
    os << str.data;
    return os;
}

// Input stream operator: reads a word from the input stream into the MyString
std::istream& operator>>(std::istream& is, MyString& str) {
    char buffer[1024];
    is >> buffer;
    str = MyString(buffer);
    return is;
}