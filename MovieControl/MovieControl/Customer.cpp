#include "Customer.h"

Customer::Customer(int id,
    const MyString& name,
    const MyString& password,
    double balance)
    : User(id, name, password, balance)
{
}

// add a newly purchased ticket
void Customer::addTicket(const Ticket& t) {
    upcomingTickets_.push_back(t);
}

// remove a ticket after either cancellation or it has been consumed
bool Customer::removeTicketById(int ticketId) {
    for (size_t i = 0; i < upcomingTickets_.size(); ++i) {
        if (upcomingTickets_[i].getId() == ticketId) {
            upcomingTickets_.removeAt(i);
            return true;
        }
    }
    return false;
}

// read-only access for listing
const MyVector<Ticket>& Customer::getUpcomingTickets() const {
    return upcomingTickets_;
}

// when a showtime passes, we move its movieId into history
void Customer::addToHistory(int movieId) {
    watchedMovieIds_.push_back(movieId);
}

// for list-history
const MyVector<int>& Customer::getWatchHistory() const {
    return watchedMovieIds_;
}

void Customer::removeFromHistory(int movieId) {
    for (size_t i = 0; i < watchedMovieIds_.size(); ++i) {
        if (watchedMovieIds_[i] == movieId) {
            watchedMovieIds_.removeAt(i);
            return;
        }
    }
}
