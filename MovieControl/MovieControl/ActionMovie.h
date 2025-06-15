// movie/ActionMovie.h
#ifndef ACTIONMOVIE_H
#define ACTIONMOVIE_H

#include "Movie.h"

class ActionMovie : public Movie {
    int actionIntensity_;  // 0..20
public:
    ActionMovie(int id,
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
        int actionIntensity);

    double calculatePrice(double basePrice) const override;
    void   print(std::ostream& os) const override;

    int getIntensity() const { return actionIntensity_; }
};

#endif 
