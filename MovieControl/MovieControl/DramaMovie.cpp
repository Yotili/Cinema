#include "DramaMovie.h"

DramaMovie::DramaMovie(int id,
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
    bool hasComedyElements)
    : Movie(id, title, rating, duration, year,
        MyString("Drama"),
        hallId, date, startHour, startMin, endHour, endMin)
    , hasComedyElements_(hasComedyElements)
{
}

double DramaMovie::calculatePrice(double basePrice) const {
    return basePrice + (hasComedyElements_ ? 2.0 : 0.0);
}

void DramaMovie::print(std::ostream& os) const {
    Movie::print(os);
    os << "   >> comedy elements: "
        << (hasComedyElements_ ? "yes" : "no") << "\n";
}

