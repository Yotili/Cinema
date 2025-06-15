#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include "Cinema.h"
#include "MyString.h"
#include <iostream>

class CommandProcessor {
    Cinema  cinema_;
    User* currentUser_;
    bool    exitRequested_;

public:
    CommandProcessor();
    ~CommandProcessor();
    void run();

private:
    void handleLine(const MyString& line);

    // basic commands
    void doRegister(const MyString& name, const MyString& pwd);
    void doLogin(const MyString& name, const MyString& pwd);
    void doLogout();
    void doExit();
    void doHelp();

    void doListMovies();
    void doListTickets();
    void doListHistory();

    void doBuyTicket(int movieId, int row, int col);
    void doRateMovie(int movieId, int rating);

    void doDeposit(double amount);
    void doBalance();

    void doListHalls();

    void doHasTickets(int movieId);

    // admin-only
    void doListUsers();
    void doOpenHall(int rows, int cols);
    void doCloseHall(int hallId);
    void doRemoveMovie(int movieId);
    void doUpdateMovieTitle(int movieId, const MyString& newTitle);
    void doUpdateMovieHall(int movieId, int newHallId);
    void doRemoveUser(int userId);

    // Липсващи, но използвани методи:
    void doListUserHistory(int userId);
    void doListUserTickets(int userId);
};

#endif
