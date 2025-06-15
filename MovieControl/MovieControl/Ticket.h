#ifndef TICKET_H
#define TICKET_H

#include <iostream>

class Ticket {
    int    id_;
    int    movieId_;
    int    row_, col_;
    double price_;

public:
    // only this constructor is declared:
    Ticket(int id, int movieId, int row, int col, double price);

    int    getId()      const { return id_; }
    int    getMovieId() const { return movieId_; }
    int    getRow()     const { return row_; }
    int    getCol()     const { return col_; }
    double getPrice()   const { return price_; }

    friend std::ostream& operator<<(std::ostream& os, const Ticket& t) {
        os << "[Ticket " << t.id_ << "] movie=" << t.movieId_
            << " seat=(" << t.row_ << ',' << t.col_ << ")"
            << " price=" << t.price_;
        return os;
    }
};

#endif 