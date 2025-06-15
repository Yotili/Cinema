#ifndef DOCUMENTARYMOVIE_H
#define DOCUMENTARYMOVIE_H

#include "Movie.h"

class DocumentaryMovie : public Movie {
    MyString theme_;         // e.g. "history", "nature"
    bool     isBasedOnTrue_; // true if based on real events
public:
    DocumentaryMovie(int id,
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
        const MyString& theme,
        bool isBasedOnTrueEvents);

    double calculatePrice(double basePrice) const override;
    void   print(std::ostream& os) const override;

    const MyString& getTheme() const { return theme_; }
    bool isBasedOnTrue() const { return isBasedOnTrue_; }

};

#endif 

