#ifndef CINEMA_H
#define CINEMA_H

#include "MyString.h"
#include "MyVector.hpp"
#include "Hall.h"
#include "Movie.h"
#include "ActionMovie.h"
#include "DramaMovie.h"
#include "DocumentaryMovie.h"
#include "User.h"
#include "Customer.h"
#include "Admin.h"
#include "Ticket.h"
#include <iostream>
#include <ctime>
#include <fstream>

class Cinema {
public:
    Cinema();
    ~Cinema();

    void loadState();
    void saveState() const;

    // User management
    User* login(const MyString& name, const MyString& pwd) const;
    bool registerCustomer(const MyString& name, const MyString& pwd);

    // Hall management
    int openHall(int rows, int cols);
    bool closeHall(int hallId);

    // Movie management
    Movie* addActionMovie(const MyString& title, double rating, int duration, int year,
        int hallId, const MyString& date, int sh, int sm, int eh, int em, int actionIntensity);

    Movie* addDramaMovie(const MyString& title, double rating, int duration, int year,
        int hallId, const MyString& date, int sh, int sm, int eh, int em, bool hasComedy);

    Movie* addDocumentaryMovie(const MyString& title, double rating, int duration, int year,
        int hallId, const MyString& date, int sh, int sm, int eh, int em,
        const MyString& theme, bool basedOnTrue);

    bool removeMovie(int movieId);
    bool updateMovieTitle(int movieId, const MyString& newTitle);
    bool updateMovieHall(int movieId, int newHallId);

    bool isTimeSlotFree(int hallId,
        const MyString& date,
        int startHour, int startMin,
        int endHour, int endMin) const;

    // Ticketing
    bool buyTicket(Customer* c, int movieId, int row, int col);
    bool hasTickets(int movieId) const;

    // Listings
    void listHalls(std::ostream& os = std::cout) const;
    void listMovies(std::ostream& os = std::cout) const;
    void listUsers(std::ostream& os = std::cout) const;
    void listTickets(const Customer* c, std::ostream& os = std::cout) const;
    void listHistory(const Customer* c, std::ostream& os = std::cout) const;

    // By user (admin)
    void listUserHistory(int userId, std::ostream& os = std::cout) const;
    void listUserTickets(int userId, std::ostream& os = std::cout) const;
    bool removeUser(int userId);

    // Rating
    bool rateMovie(Customer* c, int movieId, int rating);

    // Прехвърляне на изтекли билети
    void expirePastTickets(Customer* c) const;

private:
    MyVector<Hall*> halls_;
    MyVector<Movie*> movies_;
    MyVector<User*> users_;

    int nextHallId_ = 1;
    int nextMovieId_ = 1;
    int nextUserId_ = 1;
    int nextTicketId_ = 1;

    const double baseAction_ = 9.0;
    const double baseDrama_ = 7.0;
    const double baseDoc_ = 5.0;

    Hall* findHall(int id) const;
    Movie* findMovie(int id) const;
    User* findUserByName(const MyString& n) const;
    User* findUserById(int id) const;
};

#endif // CINEMA_H
