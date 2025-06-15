#include "ActionMovie.h"

ActionMovie::ActionMovie(int id,
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
    int actionIntensity)
    : Movie(id, title, rating, duration, year,
        MyString("Action"), // genre
        hallId, date, startHour, startMin, endHour, endMin)
    , actionIntensity_(actionIntensity)
{
    if (actionIntensity_ < 0 || actionIntensity_ > 20)
        throw "actionIntensity must be in [0..20]";
}

double ActionMovie::calculatePrice(double basePrice) const {
    return basePrice + actionIntensity_ * 1.5;
}

void ActionMovie::print(std::ostream& os) const {
    Movie::print(os);
    os << "   >> action intensity: " << actionIntensity_ << "\n";
}
