#ifndef MOVIE_H
#define MOVIE_H

#include "MyString.h"
#include <iostream>

class Movie {
protected:
    int          id_;
    MyString     title_;
    double       rating_;
    double ratingSum_;// sum of all ratings
    int    ratingCount_;// how many ratings contributed
    int          duration_;      // in minutes
    int          year_;
    MyString     genre_;
    int          hallId_;
    MyString     date_;          // e.g. "2025-06-10"
    int          startHour_, startMin_;
    int          endHour_, endMin_;

public:
    Movie(int id,
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
        int endMin);

    virtual ~Movie() = default;

    // basic accessors
    int          getId()       const { return id_; }
    const MyString& getTitle() const { return title_; }
    double       getRating()   const { return rating_; }
    int          getDuration() const { return duration_; }
    int          getYear()     const { return year_; }
    const MyString& getGenre() const { return genre_; }
    int          getHallId()   const { return hallId_; }
    const MyString& getDate()  const { return date_; }
    void         getStartTime(int& h, int& m) const { h = startHour_; m = startMin_; }
    void         getEndTime(int& h, int& m) const { h = endHour_;   m = endMin_; }

    // direct setters
    void         setTitle(const MyString& title) { title_ = title; }
    void         setHallId(int hallId) { hallId_ = hallId; }

    // price calculator to be implemented by subclasses
    virtual double calculatePrice(double basePrice) const = 0;

    virtual void addRating(int rating);

    // dump info
    virtual void   print(std::ostream& os) const;

    friend std::ostream& operator<<(std::ostream& os, const Movie& m) {
        m.print(os);
        return os;
    }
};

#endif 