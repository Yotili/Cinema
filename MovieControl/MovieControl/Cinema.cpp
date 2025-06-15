#define _CRT_SECURE_NO_WARNINGS

#include "Cinema.h"
#include <iostream>
#include <ctime>
#include <fstream>

// Loads the state of the cinema from files (halls, movies, users)
void Cinema::loadState() {
    std::ifstream hallIn("data/halls.txt");
    if (hallIn) {
        int id, open, rows, cols;
        // Read each hall's data from file
        while (hallIn >> id >> open >> rows >> cols) {
            Hall* h = new Hall(id, rows, cols);
            // Read seat availability for each seat
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    bool free;
                    hallIn >> free;
                    if (!free) h->reserveSeat(r, c);
                }
            }
            halls_.push_back(h);
            if (id >= nextHallId_) nextHallId_ = id + 1;
        }
    }

    std::ifstream movieIn("data/movies.txt");
    if (movieIn) {
        int id, dur, year, hallId, sh, sm, eh, em;
        double rating;
        MyString genre, title, date;

        // Read each movie's data from file
        while (movieIn >> id >> genre >> title >> rating >> dur >> year >> hallId >> date
            >> sh >> sm >> eh >> em) {

            Movie* m = nullptr;

            // Create the appropriate movie type based on genre
            if (genre == "Action") {
                int intensity;
                movieIn >> intensity;
                m = new ActionMovie(id, title, rating, dur, year, hallId, date, sh, sm, eh, em, intensity);
            }
            else if (genre == "Drama") {
                int comedy;
                movieIn >> comedy;
                m = new DramaMovie(id, title, rating, dur, year, hallId, date, sh, sm, eh, em, comedy != 0);
            }
            else if (genre == "Documentary") {
                MyString theme;
                int based;
                movieIn >> theme >> based;
                m = new DocumentaryMovie(id, title, rating, dur, year, hallId, date, sh, sm, eh, em, theme, based != 0);
            }

            if (m) {
                movies_.push_back(m);
                if (id >= nextMovieId_) nextMovieId_ = id + 1;
            }
        }
    }

    std::ifstream userIn("data/users.txt");
    if (userIn) {
        int id;
        double balance;
        MyString name, type, password;

        // Read each user's data from file
        while (userIn >> id >> name >> balance >> type >> password) {
            User* u = nullptr;

            // Create Admin or Customer based on type
            if (type == "admin") {
                u = new Admin(id, name, password, balance);
            }
            else {
                u = new Customer(id, name, password, balance);

                std::string label;
                int ticketCount;
                userIn >> label >> ticketCount;
                // Read all tickets for this customer
                for (int i = 0; i < ticketCount; ++i) {
                    int tid, mid, row, col;
                    double price;
                    userIn >> tid >> mid >> row >> col >> price;
                    static_cast<Customer*>(u)->addTicket(
                        Ticket(tid, mid, row, col, price));
                    if (tid >= nextTicketId_) nextTicketId_ = tid + 1;
                }

                int histCount;
                userIn >> label >> histCount;
                // Read watch history for this customer
                for (int i = 0; i < histCount; ++i) {
                    int mid;
                    userIn >> mid;
                    static_cast<Customer*>(u)->addToHistory(mid);
                }
            }

            if (u) {
                users_.push_back(u);
                if (id >= nextUserId_) nextUserId_ = id + 1;
            }
        }
    }
}

// Saves the current state of the cinema to files (halls, movies, users)
void Cinema::saveState() const {
    std::ofstream hallOut("data/halls.txt");
    // Save all halls and their seat layouts
    for (size_t i = 0; i < halls_.size(); ++i) {
        hallOut << halls_[i]->getId() << ' '
            << halls_[i]->isOpen() << ' '
            << halls_[i]->getRows() << ' '
            << halls_[i]->getCols() << '\n';

        for (int r = 0; r < halls_[i]->getRows(); ++r) {
            for (int c = 0; c < halls_[i]->getCols(); ++c) {
                hallOut << halls_[i]->isSeatFree(r, c) << ' ';
            }
            hallOut << '\n';
        }
    }

    std::ofstream movieOut("data/movies.txt");
    // Save all movies and their specific data
    for (size_t i = 0; i < movies_.size(); ++i) {
        Movie* m = movies_[i];
        movieOut << m->getId() << ' '
            << m->getGenre() << ' '
            << m->getTitle() << ' '
            << m->getRating() << ' '
            << m->getDuration() << ' '
            << m->getYear() << ' '
            << m->getHallId() << ' '
            << m->getDate() << ' ';

        int sh, sm, eh, em;
        m->getStartTime(sh, sm);
        m->getEndTime(eh, em);
        movieOut << sh << ' ' << sm << ' ' << eh << ' ' << em << ' ';

        // Save genre-specific fields
        if (m->getGenre() == "Action") {
            movieOut << static_cast<ActionMovie*>(m)->getIntensity();
        }
        else if (m->getGenre() == "Drama") {
            movieOut << static_cast<DramaMovie*>(m)->hasComedyElements();
        }
        else if (m->getGenre() == "Documentary") {
            auto* d = static_cast<DocumentaryMovie*>(m);
            movieOut << d->getTheme() << ' ' << d->isBasedOnTrue();
        }
        movieOut << '\n';
    }

    std::ofstream userOut("data/users.txt");
    // Save all users and their data
    for (size_t i = 0; i < users_.size(); ++i) {
        User* u = users_[i];
        userOut << u->getId() << ' '
            << u->getName() << ' '
            << u->getBalance() << ' '
            << (u->isAdmin() ? "admin" : "customer") << ' '
            << u->getPassword() << '\n';

        // Save tickets and history for customers
        if (!u->isAdmin()) {
            auto* c = static_cast<Customer*>(u);
            userOut << "Tickets: " << c->getUpcomingTickets().size() << '\n';
            for (const Ticket& t : c->getUpcomingTickets()) {
                userOut << t.getId() << ' ' << t.getMovieId() << ' '
                    << t.getRow() << ' ' << t.getCol() << ' '
                    << t.getPrice() << '\n';
            }

            userOut << "History: " << c->getWatchHistory().size() << '\n';
            for (int mid : c->getWatchHistory()) {
                userOut << mid << ' ';
            }
            userOut << '\n';
        }
    }
}


// ————— Helpers —————
// Find a hall by its ID
Hall* Cinema::findHall(int id) const {
    for (size_t i = 0; i < halls_.size(); ++i)
        if (halls_[i]->getId() == id)
            return halls_[i];
    return nullptr;
}

// Find a movie by its ID
Movie* Cinema::findMovie(int id) const {
    for (size_t i = 0; i < movies_.size(); ++i)
        if (movies_[i]->getId() == id)
            return movies_[i];
    return nullptr;
}

// Find a user by their name
User* Cinema::findUserByName(const MyString& name) const {
    for (size_t i = 0; i < users_.size(); ++i)
        if (users_[i]->getName() == name)
            return users_[i];
    return nullptr;
}

// Find a user by their ID
User* Cinema::findUserById(int id) const {
    for (size_t i = 0; i < users_.size(); ++i)
        if (users_[i]->getId() == id)
            return users_[i];
    return nullptr;
}

// ————— Construction / Destruction —————
// hard coding admin
// Constructor: initializes the cinema with a default admin user
Cinema::Cinema() {
    users_.push_back(new Admin(nextUserId_++, MyString("admin"), MyString("admin")));
}

// Destructor: saves state and cleans up all allocated memory
Cinema::~Cinema() {
    saveState();

    for (auto h : halls_) delete h;
    halls_.clear();  // prevents double free

    for (auto m : movies_) delete m;
    movies_.clear();

    for (auto u : users_) delete u;
    users_.clear();
}


// ————— User Management —————
// Attempt to log in a user by name and password
User* Cinema::login(const MyString& name, const MyString& pwd) const {
    if (User* u = findUserByName(name))
        if (u->authenticate(pwd))
            return u;
    return nullptr;
}

// Register a new customer if the name is not already taken
bool Cinema::registerCustomer(const MyString& name, const MyString& pwd) {
    if (findUserByName(name)) return false;
    users_.push_back(new Customer(nextUserId_++, name, pwd));
    return true;
}

// ————— Hall Management —————
// Open a new hall with the given dimensions
int Cinema::openHall(int rows, int cols) {
    Hall* h = new Hall(nextHallId_, rows, cols);
    halls_.push_back(h);
    return nextHallId_++;
}

// Close a hall by its ID, refunding tickets and updating movies
bool Cinema::closeHall(int hallId) {
    Hall* h = findHall(hallId);
    if (!h) return false;

    // For each movie in this hall, refund tickets and update hallId
    for (size_t i = 0; i < movies_.size(); ++i) {
        Movie* m = movies_[i];
        if (m->getHallId() == hallId) {
            int mid = m->getId();

            // Refund tickets for all customers
            for (size_t j = 0; j < users_.size(); ++j) {
                if (Customer* c = dynamic_cast<Customer*>(users_[j])) {
                    const auto& tickets = c->getUpcomingTickets();
                    for (size_t t = 0; t < tickets.size(); ++t) {
                        const Ticket& ticket = tickets[t];
                        if (ticket.getMovieId() == mid) {
                            c->deposit(ticket.getPrice());
                            try {
                                h->freeSeat(ticket.getRow(), ticket.getCol());
                            }
                            catch (...) {}
                            c->removeTicketById(ticket.getId());
                        }
                    }
                }
            }

            m->setHallId(-1);  // Unknown
        }
    }

    // Remove the hall from the list and delete it
    for (size_t i = 0; i < halls_.size(); ++i) {
        if (halls_[i]->getId() == hallId) {
            delete halls_[i];
            halls_.removeAt(i);
            return true;
        }
    }

    return false;
}

// ————— Movie Management —————
// Add a new Action movie to the catalog
Movie* Cinema::addActionMovie(const MyString& title, double rating, int duration, int year,
    int hallId, const MyString& date, int sh, int sm, int eh, int em, int actionIntensity) {
    if (!findHall(hallId)) return nullptr;
    auto* m = new ActionMovie(nextMovieId_++, title, rating, duration, year, hallId, date, sh, sm, eh, em, actionIntensity);
    movies_.push_back(m);
    return m;
}

// Add a new Drama movie to the catalog
Movie* Cinema::addDramaMovie(const MyString& title, double rating, int duration, int year,
    int hallId, const MyString& date, int sh, int sm, int eh, int em, bool hasComedy) {
    if (!findHall(hallId)) return nullptr;
    auto* m = new DramaMovie(nextMovieId_++, title, rating, duration, year, hallId, date, sh, sm, eh, em, hasComedy);
    movies_.push_back(m);
    return m;
}

// Add a new Documentary movie to the catalog
Movie* Cinema::addDocumentaryMovie(const MyString& title, double rating, int duration, int year,
    int hallId, const MyString& date, int sh, int sm, int eh, int em,
    const MyString& theme, bool basedOnTrue) {
    if (!findHall(hallId)) return nullptr;
    auto* m = new DocumentaryMovie(nextMovieId_++, title, rating, duration, year, hallId, date, sh, sm, eh, em, theme, basedOnTrue);
    movies_.push_back(m);
    return m;
}

// Remove a movie from the catalog, refunding tickets and updating histories
bool Cinema::removeMovie(int movieId) {
    Movie* m = findMovie(movieId);
    if (!m) return false;

    // 1. Refund tickets, free seats, and remove tickets for all customers
    for (size_t i = 0; i < users_.size(); ++i) {
        if (Customer* c = dynamic_cast<Customer*>(users_[i])) {
            const auto& tickets = c->getUpcomingTickets();
            for (size_t t = 0; t < tickets.size(); ++t) {
                const Ticket& ticket = tickets[t];
                if (ticket.getMovieId() == movieId) {
                    c->deposit(ticket.getPrice());

                    if (Hall* h = findHall(m->getHallId())) {
                        try {
                            h->freeSeat(ticket.getRow(), ticket.getCol());
                        }
                        catch (...) {}
                    }

                    c->removeTicketById(ticket.getId());
                }
            }

            // 2. Remove from watch history if present
            const MyVector<int>& history = c->getWatchHistory();
            for (size_t h = 0; h < history.size(); ++h) {
                if (history[h] == movieId) {
                    c->removeFromHistory(movieId);
                    break;
                }
            }
        }
    }

    // 3. Delete the movie from the catalog
    for (size_t i = 0; i < movies_.size(); ++i) {
        if (movies_[i]->getId() == movieId) {
            delete movies_[i];
            movies_.removeAt(i);
            return true;
        }
    }

    return false;
}

// Check if any tickets exist for a given movie
bool Cinema::hasTickets(int movieId) const {
    for (User* u : users_) {
        if (auto* c = dynamic_cast<Customer*>(u)) {
            for (const Ticket& t : c->getUpcomingTickets()) {
                if (t.getMovieId() == movieId)
                    return true;
            }
        }
    }
    return false;
}

// Update the title of a movie
bool Cinema::updateMovieTitle(int movieId, const MyString& newTitle) {
    if (Movie* m = findMovie(movieId)) {
        m->setTitle(newTitle);
        return true;
    }
    return false;
}

// Update the hall for a movie, checking for time slot collisions
bool Cinema::updateMovieHall(int movieId, int newHallId) {
    // Prevent updating to "closed" (-1) or non-existent hall
    if (newHallId == -1) return false;
    Hall* newHall = findHall(newHallId);
    if (!newHall) return false;

    Movie* m = findMovie(movieId);
    if (!m) return false;

    // === Time slot collision logic ===
    const MyString& date = m->getDate();
    int sh, sm, eh, em;
    m->getStartTime(sh, sm);
    m->getEndTime(eh, em);

    for (Movie* other : movies_) {
        if (other == m) continue;
        if (other->getHallId() != newHallId) continue;
        if (other->getDate() != date) continue;

        int osh, osm, oeh, oem;
        other->getStartTime(osh, osm);
        other->getEndTime(oeh, oem);

        int start1 = sh * 60 + sm;
        int end1 = eh * 60 + em;
        int start2 = osh * 60 + osm;
        int end2 = oeh * 60 + oem;

        if (start1 < end2 && start2 < end1) {
            // Overlap!
            return false;
        }
    }

    m->setHallId(newHallId);
    return true;
}


// ————— Ticketing —————
#include <ctime> // Already present

// Attempt to buy a ticket for a customer for a specific movie and seat
bool Cinema::buyTicket(Customer* cust, int movieId, int row, int col) {
    Movie* m = findMovie(movieId);
    if (!m) return false;

    // --- Prevent buying for past showings ---
    std::time_t now = std::time(0);
    std::tm* local = std::localtime(&now);

    int nowYear = local->tm_year + 1900;
    int nowMonth = local->tm_mon + 1;
    int nowDay = local->tm_mday;
    int nowHour = local->tm_hour;
    int nowMin = local->tm_min;

    const char* date = m->getDate().c_str();
    int movYear = (date[0] - '0') * 1000 + (date[1] - '0') * 100 + (date[2] - '0') * 10 + (date[3] - '0');
    int movMonth = (date[5] - '0') * 10 + (date[6] - '0');
    int movDay = (date[8] - '0') * 10 + (date[9] - '0');

    int startHour, startMin;
    m->getStartTime(startHour, startMin);

    bool isPast = false;
    if (movYear < nowYear) isPast = true;
    else if (movYear == nowYear && movMonth < nowMonth) isPast = true;
    else if (movYear == nowYear && movMonth == nowMonth && movDay < nowDay) isPast = true;
    else if (movYear == nowYear && movMonth == nowMonth && movDay == nowDay) {
        if (startHour < nowHour) isPast = true;
        else if (startHour == nowHour && startMin <= nowMin) isPast = true;
    }
    if (isPast) return false;

    // --- Prevent buying tickets for closed halls ---
    if (m->getHallId() == -1)
        return false;

    Hall* h = findHall(m->getHallId());
    if (!h || !h->reserveSeat(row, col)) return false;

    // Calculate ticket price based on genre
    double price = m->calculatePrice(
        m->getGenre() == "Action" ? baseAction_ :
        m->getGenre() == "Drama" ? baseDrama_ :
        baseDoc_
    );

    if (!cust->charge(price)) return false;

    Ticket t(nextTicketId_++, movieId, row, col, price);
    cust->addTicket(t);
    return true;
}


// ————— Listings —————
// List all halls and their layouts
void Cinema::listHalls(std::ostream& os) const {
    for (size_t i = 0; i < halls_.size(); ++i)
        halls_[i]->printLayout(os);
}

// List all movies
void Cinema::listMovies(std::ostream& os) const {
    for (size_t i = 0; i < movies_.size(); ++i)
        os << *movies_[i];
}

// List all users (with admin indication)
void Cinema::listUsers(std::ostream& os) const {
    for (size_t i = 0; i < users_.size(); ++i)
        os << "[" << users_[i]->getId() << "] " << users_[i]->getName()
        << (users_[i]->isAdmin() ? " (admin)" : "") << "\n";
}

// List all tickets for a customer
void Cinema::listTickets(const Customer* c, std::ostream& os) const {
    const auto& v = c->getUpcomingTickets();
    for (size_t i = 0; i < v.size(); ++i)
        os << v[i] << "\n";
}

// List the watch history for a customer
void Cinema::listHistory(const Customer* c, std::ostream& os) const {
    const auto& hist = c->getWatchHistory();
    for (size_t i = 0; i < hist.size(); ++i) {
        if (Movie* m = findMovie(hist[i]))
            os << *m;
        else
            os << "[Movie " << hist[i] << "] (no longer in catalog)\n";
    }
}

// List the watch history for a user by ID
void Cinema::listUserHistory(int userId, std::ostream& os) const {
    if (Customer* c = dynamic_cast<Customer*>(findUserById(userId)))
        listHistory(c, os);
    else
        os << "No such customer\n";
}

// List all tickets for a user by ID
void Cinema::listUserTickets(int userId, std::ostream& os) const {
    if (Customer* c = dynamic_cast<Customer*>(findUserById(userId)))
        listTickets(c, os);
    else
        os << "No such customer\n";
}

// Remove a user by ID, freeing their tickets
bool Cinema::removeUser(int userId) {
    User* u = findUserById(userId);
    if (!u || u->isAdmin()) return false;

    if (Customer* c = dynamic_cast<Customer*>(u)) {
        const auto& tickets = c->getUpcomingTickets();
        for (size_t i = 0; i < tickets.size(); ++i) {
            const Ticket& t = tickets[i];
            if (Movie* m = findMovie(t.getMovieId()))
                if (Hall* h = findHall(m->getHallId()))
                    h->freeSeat(t.getRow(), t.getCol());
        }
    }

    for (size_t i = 0; i < users_.size(); ++i) {
        if (users_[i]->getId() == userId) {
            delete users_[i];
            users_.removeAt(i);
            return true;
        }
    }
    return false;
}

// ————— Rating —————
// Allow a customer to rate a movie only if it's in their watch history
bool Cinema::rateMovie(Customer* cust, int movieId, int rating) {
    const auto& hist = cust->getWatchHistory();
    for (size_t i = 0; i < hist.size(); ++i)
        if (hist[i] == movieId)
            return true;
    return false;
    // Only allow rating if movieId is in this customer's history
    for (int mid : hist) {
        if (mid == movieId) {
            if (Movie* m = findMovie(movieId)) {
                m->addRating(rating);
                return true;

            }
            break;

        }

    }
    return false;
}

// ————— Изтичане на стари билети —————
// Move past tickets from upcoming to history for a customer
void Cinema::expirePastTickets(Customer* c) const {
    std::time_t now = std::time(nullptr);
    std::tm today = *std::localtime(&now);

    char buf[11];
    std::snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
        today.tm_year + 1900,
        today.tm_mon + 1,
        today.tm_mday);
    // now buf is e.g. "2025-06-14"

    const auto tickets = c->getUpcomingTickets();
    for (size_t i = 0; i < tickets.size(); ++i) {
        const Ticket& tk = tickets[i];
        Movie* m = findMovie(tk.getMovieId());
        if (!m) continue;

        // use string compare, not pointer compare
        if (std::strcmp(m->getDate().c_str(), buf) < 0) {
            c->removeTicketById(tk.getId());
            c->addToHistory(m->getId());
        }
    }
}

// Check if a time slot is free in a hall for a given date
bool Cinema::isTimeSlotFree(int hallId,
    const MyString& date,
    int sh, int sm,
    int eh, int em) const
{
    // convert new slot to minutes since midnight
    int newStart = sh * 60 + sm;
    int newEnd = eh * 60 + em;

    for (Movie* m : movies_) {
        // only consider same hall and same date
        if (m->getHallId() != hallId) continue;
        if (m->getDate() != date)    continue;

        int oldSh, oldSm, oldEh, oldEm;
        m->getStartTime(oldSh, oldSm);
        m->getEndTime(oldEh, oldEm);

        int oldStart = oldSh * 60 + oldSm;
        int oldEnd = oldEh * 60 + oldEm;

        // overlap if [newStart,newEnd) intersects [oldStart,oldEnd)
        if (newStart < oldEnd && oldStart < newEnd) {
            return false;
        }
    }
    return true;
}