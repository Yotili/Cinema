#include "DocumentaryMovie.h"

DocumentaryMovie::DocumentaryMovie(int id,
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
    bool isBasedOnTrueEvents)
    : Movie(id, title, rating, duration, year,
        MyString("Documentary"),
        hallId, date, startHour, startMin, endHour, endMin)
    , theme_(theme)
    , isBasedOnTrue_(isBasedOnTrueEvents)
{
}

double DocumentaryMovie::calculatePrice(double basePrice) const {
    return basePrice + (isBasedOnTrue_ ? 3.0 : 0.0);
}

void DocumentaryMovie::print(std::ostream& os) const {
    Movie::print(os);
    os << "   >> theme: " << theme_ << "\n"
        << "   >> based on real events: "
        << (isBasedOnTrue_ ? "yes" : "no") << "\n";
}

