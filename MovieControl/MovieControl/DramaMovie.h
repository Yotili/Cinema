#ifndef DRAMAMOVIE_H
#define DRAMAMOVIE_H

#include "Movie.h"

class DramaMovie : public Movie {
    bool hasComedyElements_;
public:
    DramaMovie(int id,
        const MyString& title,
        double rating,
        int duration,
        int year,
        int hallId,
        const MyString& date,
        int startHour,
        int startMin,
        int endHour,
        int endMin,
        bool hasComedyElements);

    double calculatePrice(double basePrice) const override;
    void   print(std::ostream& os) const override;

    bool hasComedyElements() const { return hasComedyElements_; }

};

#endif 

