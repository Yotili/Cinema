#include "Hall.h"

Hall::Hall(int id, int rows, int cols)
    : id_(id), rows_(rows), cols_(cols), isOpen_(true)
{
    if (rows_ <= 0 || cols_ <= 0) {
        rows_ = 1;
        cols_ = 1;
        isOpen_ = false;
    }

    seats_.clear();
    for (int r = 0; r < rows_; ++r) {
        MyVector<bool> rowVec;
        for (int c = 0; c < cols_; ++c) {
            rowVec.push_back(true);
        }
        seats_.push_back(std::move(rowVec));
    }
}

bool Hall::reserveSeat(int row, int col) {
    // Bounds + open check
    if (!isOpen_ || row < 0 || row >= rows_ || col < 0 || col >= cols_)
        return false;

    if (!seats_[row][col])
        return false;

    seats_[row][col] = false;
    return true;
}

bool Hall::freeSeat(int row, int col) {
    // Bounds check
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
        return false;

    seats_[row][col] = true;
    return true;
}

bool Hall::isSeatFree(int row, int col) const {
    // Bounds check
    if (row < 0 || row >= rows_ || col < 0 || col >= cols_)
        return false;

    return seats_[row][col];
}

void Hall::printLayout(std::ostream& os) const {
    if (!isOpen_) {
        // closed halls are skipped
        return;
    }
    os << "Hall " << id_
        << " (" << rows_ << "×" << cols_ << ") seating:\n";
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            os << (seats_[r][c] ? '.' : 'X') << ' ';
        }
        os << '\n';
    }
}
