#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include "MyVector.hpp"
#include "Ticket.h"

class Customer : public User {
    MyVector<Ticket> upcomingTickets_;
    MyVector<int>    watchedMovieIds_;

public:
    Customer(int id,
        const MyString& name,
        const MyString& password,
        double balance = 0.0);

    // Upcoming tickets
    void addTicket(const Ticket& t);
    bool removeTicketById(int ticketId);
    const MyVector<Ticket>& getUpcomingTickets() const;

    // Watch history
    void addToHistory(int movieId);
    void removeFromHistory(int movieId);
    const MyVector<int>& getWatchHistory() const;
};

#endif

