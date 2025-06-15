#include "Movie.h"

Movie::Movie(int id,
    const MyString& title,
    double rating,
    int duration,
    int year,
    const MyString& genre,
    int hallId,
    const MyString& date,
    int startHour,
    int startMin,
    int endHour,
    int endMin)
    : id_(id)
    , title_(title)
    , rating_(rating)
    , duration_(duration)
    , year_(year)
    , genre_(genre)
    , hallId_(hallId)
    , date_(date)
    , startHour_(startHour)
    , startMin_(startMin)
    , endHour_(endHour)
    , endMin_(endMin)
{
    ratingSum_ = rating_;
    ratingCount_ = 1;
}

void Movie::print(std::ostream& os) const {
    os << "[" << id_ << "] "
        << title_ << " (" << year_ << "), "
        << "rating: " << rating_ << ", "
        << duration_ << "min, "
        << "genre: " << genre_ << "\n"
        << "  hall: ";
    if (hallId_ == -1)
        os << "closed";
    else
        os << hallId_;
    os << "  date/time: " << date_
        << " " << (startHour_ < 10 ? "0" : "") << startHour_
        << ":" << (startMin_ < 10 ? "0" : "") << startMin_
        << "-" << (endHour_ < 10 ? "0" : "") << endHour_
        << ":" << (endMin_ < 10 ? "0" : "") << endMin_
        << "\n";
}


void Movie::addRating(int rating) {
    ratingSum_ += rating;
    ratingCount_ += 1;
    rating_ = ratingSum_ / ratingCount_;
}
