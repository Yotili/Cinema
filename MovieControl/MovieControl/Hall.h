// Hall.h
#ifndef HALL_H
#define HALL_H

#include "MyVector.hpp"
#include <iostream>

class Hall {
    int id_;
    int rows_;
    int cols_;
    MyVector<MyVector<bool>> seats_;  // true = free, false = taken
    bool isOpen_;

public:
    // Construct a hall with given id and dimensions.
    // Invalid dims → 1×1 hall, closed.
    Hall(int id, int rows, int cols);

    // Mark this hall closed (no longer usable)
    void close() { isOpen_ = false; }
    bool isOpen()  const { return isOpen_; }

    int  getId()    const { return id_; }
    int  getRows()  const { return rows_; }
    int  getCols()  const { return cols_; }

    // Reserve a seat; returns false if:
    //  - hall is closed
    //  - out of bounds
    //  - already taken
    bool reserveSeat(int row, int col);

    // Free a seat; returns false if out of bounds
    bool freeSeat(int row, int col);

    // Query if seat is free; false if out of bounds
    bool isSeatFree(int row, int col) const;

    // Print ASCII layout ('.'=free, 'X'=taken).
    // If hall is closed, prints nothing.
    void printLayout(std::ostream& os = std::cout) const;
};

#endif 
