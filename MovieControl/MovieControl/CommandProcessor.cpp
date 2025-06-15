#include "CommandProcessor.h"
#include "MyString.h"
#include <sstream>
#include <string>
#include <iostream>
#include <regex>

// Prints a formatted box with a title and content to the console
static void printBox(const std::string& title, const std::string& content) {
    // Split content into lines
    std::vector<std::string> lines;
    std::istringstream iss(content);
    std::string line;
    size_t maxlen = title.size();
    while (std::getline(iss, line)) {
        lines.push_back(line);
        maxlen = std::max(maxlen, line.size());
    }

    // Compute total inner width (padding of 1 space each side)
    size_t width = maxlen + 2;

    // Top border
    std::cout << "+" << std::string(width, '-') << "+\n";

    // Title line (centered)
    size_t padLeft = (width - title.size()) / 2;
    size_t padRight = width - title.size() - padLeft;
    std::cout << "|"
        << std::string(padLeft, ' ')
        << title
        << std::string(padRight, ' ')
        << "|\n";

    // Separator
    std::cout << "+" << std::string(width, '-') << "+\n";

    // Content lines
    for (auto& ln : lines) {
        std::cout << "|"
            << ln
            << std::string(width - ln.size(), ' ')
            << "|\n";
    }

    std::cout << "+" << std::string(width, '-') << "+\n";
}

// Validates if a string is a date in YYYY-MM-DD format
static bool validDate(const std::string& date) {
    if (date.size() != 10) return false;
    // YYYY-MM-DD
    for (int i = 0; i < 4; ++i)
        if (date[i] < '0' || date[i] > '9') return false;
    if (date[4] != '-') return false;
    for (int i = 5; i < 7; ++i)
        if (date[i] < '0' || date[i] > '9') return false;
    if (date[7] != '-') return false;
    for (int i = 8; i < 10; ++i)
        if (date[i] < '0' || date[i] > '9') return false;
    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));
    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

// Validates if the given hour and minute represent a valid time
static bool validTime(int h, int m) {
    return (h >= 0 && h < 24) && (m >= 0 && m < 60);
}

// Checks if the start time is before the end time
static bool startBeforeEnd(int sh, int sm, int eh, int em) {
    return (sh * 60 + sm) < (eh * 60 + em);
}

// Constructor: initializes the command processor and loads cinema state
CommandProcessor::CommandProcessor()
    : currentUser_(nullptr), exitRequested_(false)
{
    cinema_.loadState();
    printBox("Welcome to MovieApp", "Type 'help' to see available commands\n");
}

// Destructor: saves cinema state
CommandProcessor::~CommandProcessor() {
    cinema_.saveState();
}

// Main loop: reads and processes user commands until exit is requested
void CommandProcessor::run() {
    std::string raw;
    while (!exitRequested_) {
        std::cout << "-> " << std::flush;
        if (!std::getline(std::cin, raw)) break;
        MyString line(raw.c_str());
        handleLine(line);
    }
}

// Parses and dispatches a single command line
void CommandProcessor::handleLine(const MyString& line) {
    std::string raw(line.c_str());
    size_t pos = 0;
    // Lambda to extract the next token from the input
    auto nextToken = [&](std::string& out) {
        while (pos < raw.size() && raw[pos] == ' ') ++pos;
        size_t start = pos;
        while (pos < raw.size() && raw[pos] != ' ') ++pos;
        out = raw.substr(start, pos - start);
        };

    std::string cmd;
    nextToken(cmd);

    // Command dispatch
    if (cmd == "help") {
        doHelp();
    }
    else if (cmd == "register") {
        std::string u, p;
        nextToken(u); nextToken(p);
        doRegister(u.c_str(), p.c_str());
    }
    else if (cmd == "login") {
        std::string u, p;
        nextToken(u); nextToken(p);
        doLogin(u.c_str(), p.c_str());
    }
    else if (cmd == "logout") {
        doLogout();
    }
    else if (cmd == "exit") {
        doExit();
    }
    else if (cmd == "deposit") {
        std::string amt;
        nextToken(amt);
        doDeposit(std::stod(amt));
    }
    else if (cmd == "balance") {
        doBalance();
    }
    else if (cmd == "list-movies") {
        doListMovies();
    }
    else if (cmd == "list-halls") {
        doListHalls();
    }
    else if (cmd == "list-tickets") {
        doListTickets();
    }
    else if (cmd == "list-history") {
        doListHistory();
    }
    else if (cmd == "list-users") {
        doListUsers();
    }
    else if (cmd == "has-tickets") {
        std::string m; nextToken(m);
        doHasTickets(std::stoi(m));
    }
    else if (cmd == "buy-ticket") {
        std::string m, r, c;
        nextToken(m); nextToken(r); nextToken(c);
        doBuyTicket(std::stoi(m), std::stoi(r), std::stoi(c));
    }
    else if (cmd == "rate-movie") {
        std::string m, rt;
        nextToken(m); nextToken(rt);
        doRateMovie(std::stoi(m), std::stoi(rt));
    }
    // Admin-only commands
    else if (currentUser_ && currentUser_->isAdmin()) {
        if (cmd == "open-hall") {
            std::string r, c;
            nextToken(r); nextToken(c);
            doOpenHall(std::stoi(r), std::stoi(c));
        }
        else if (cmd == "close-hall") {
            std::string h; nextToken(h);
            doCloseHall(std::stoi(h));
        }
        else if (cmd == "remove-user") {
            std::string u; nextToken(u);
            doRemoveUser(std::stoi(u));
        }
        else if (cmd == "list-user-history") {
            std::string u; nextToken(u);
            doListUserHistory(std::stoi(u));
        }
        else if (cmd == "list-user-tickets") {
            std::string u; nextToken(u);
            doListUserTickets(std::stoi(u));
        }
        else if (cmd == "remove-movie") {
            std::string m; nextToken(m);
            doRemoveMovie(std::stoi(m));
        }
        else if (cmd == "update-movie-title") {
            std::string m; nextToken(m);
            while (pos < raw.size() && raw[pos] == ' ') ++pos;
            std::string nt = raw.substr(pos);
            doUpdateMovieTitle(std::stoi(m), MyString(nt.c_str()));
        }
        else if (cmd == "update-movie-hall") {
            std::string m, nh; nextToken(m); nextToken(nh);
            doUpdateMovieHall(std::stoi(m), std::stoi(nh));
        }
        else if (cmd == "add-movie") {
            std::string g, t, rt, dur, yr, h, date, sh, sm, eh, em, ex;
            nextToken(g); nextToken(t); nextToken(rt); nextToken(dur);
            nextToken(yr); nextToken(h); nextToken(date);
            nextToken(sh); nextToken(sm); nextToken(eh); nextToken(em);
            // basic validation
            if (t.empty() || date.empty()) {
                printBox("Error", "Movie title and date cannot be empty\n");
                return;
            }
            if (!validDate(date)) {
                printBox("Error", "Date must be in YYYY-MM-DD format\n");
                return;
            }
            if (!validTime(std::stoi(sh), std::stoi(sm)) ||
                !validTime(std::stoi(eh), std::stoi(em))) {
                printBox("Error", "Hours must be 0–23 and minutes 0–59\n");
                return;
            }
            if (!startBeforeEnd(std::stoi(sh), std::stoi(sm),
                std::stoi(eh), std::stoi(em))) {
                printBox("Error", "Start time must be before end time\n");
                return;
            }
            if (!cinema_.isTimeSlotFree(std::stoi(h), MyString(date.c_str()),
                std::stoi(sh), std::stoi(sm),
                std::stoi(eh), std::stoi(em))) {
                printBox("Error", "This time slot overlaps an existing showing\n");
                return;
            }
            Movie* mptr = nullptr;
            if (g == "Documentary") {
                nextToken(ex);
                if (ex.empty()) {
                    printBox("Error", "Documentary theme cannot be empty\n");
                    return;
                }
                bool based = (ex == "1" || ex == "true");
                mptr = cinema_.addDocumentaryMovie(
                    t.c_str(), std::stod(rt), std::stoi(dur), std::stoi(yr),
                    std::stoi(h), MyString(date.c_str()),
                    std::stoi(sh), std::stoi(sm), std::stoi(eh), std::stoi(em),
                    MyString(ex.c_str()), based
                );
            }
            else if (g == "Action") {
                nextToken(ex);
                int intensity = std::stoi(ex);
                if (intensity < 0 || intensity > 20) {
                    printBox("Error", "Action intensity must be 0–20\n");
                    return;
                }
                mptr = cinema_.addActionMovie(
                    t.c_str(), std::stod(rt), std::stoi(dur), std::stoi(yr),
                    std::stoi(h), MyString(date.c_str()),
                    std::stoi(sh), std::stoi(sm), std::stoi(eh), std::stoi(em),
                    intensity
                );
            }
            else if (g == "Drama") {
                nextToken(ex);
                bool hasComedy = (ex == "1" || ex == "true");
                mptr = cinema_.addDramaMovie(
                    t.c_str(), std::stod(rt), std::stoi(dur), std::stoi(yr),
                    std::stoi(h), MyString(date.c_str()),
                    std::stoi(sh), std::stoi(sm), std::stoi(eh), std::stoi(em),
                    hasComedy
                );
            }
            else {
                printBox("Error", "Invalid genre\n");
                return;
            }
            if (mptr)
                printBox("Success", "Added movie [" + std::to_string(mptr->getId()) + "]\n");
            else
                printBox("Error", "Failed to add movie (invalid hall ID?)\n");
        }
    }
    else {
        printBox("Error", "Unknown or unauthorized command: " + cmd + "\n");
    }
}

// Handles user registration
void CommandProcessor::doRegister(const MyString& name, const MyString& pwd) {
    if (name.size() == 0 || pwd.size() == 0) {
        printBox("Error", "Username and password cannot be empty\n");
        return;
    }
    bool ok = cinema_.registerCustomer(name, pwd);
    printBox(ok ? "Success" : "Error",
        ok ? "Registered " + std::string(name.c_str()) + "\n"
        : "Registration failed (name taken)\n");
}

// Handles user login
void CommandProcessor::doLogin(const MyString& name, const MyString& pwd) {
    if (name.size() == 0 || pwd.size() == 0) {
        printBox("Error", "Username and password cannot be empty\n");
        return;
    }
    User* u = cinema_.login(name, pwd);
    if (!u) {
        printBox("Error", "Login failed (wrong credentials)\n");
    }
    else {
        currentUser_ = u;
        printBox("Welcome", std::string(name.c_str()) + "\n");
        if (auto* c = dynamic_cast<Customer*>(u))
            cinema_.expirePastTickets(c);
    }
}

// Handles user logout
void CommandProcessor::doLogout() {
    if (currentUser_) {
        printBox("Goodbye", std::string(currentUser_->getName().c_str()) + "\n");
        currentUser_ = nullptr;
    }
    else {
        printBox("Info", "No user logged in\n");
    }
}

// Handles application exit
void CommandProcessor::doExit() {
    cinema_.saveState();
    printBox("Exiting", "Goodbye!\n");
    exitRequested_ = true;
}

// Lists all movies
void CommandProcessor::doListMovies() {
    std::string title = "Movie List";
    int len = title.length();
    // header
    for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
    std::cout << "| " << title << " |\n";
    for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
    // content
    cinema_.listMovies(std::cout);
    // footer
    for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
}

// Lists all tickets for the current customer
void CommandProcessor::doListTickets() {
    if (auto* c = dynamic_cast<Customer*>(currentUser_)) {
        std::string title = "Your Tickets";
        int len = title.length();
        for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
        std::cout << "| " << title << " |\n";
        for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
        cinema_.listTickets(c, std::cout);
        for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
    }
    else {
        printBox("Error", "Not a customer\n");
    }
}

// Lists the watch history for the current customer
void CommandProcessor::doListHistory() {
    if (auto* c = dynamic_cast<Customer*>(currentUser_)) {
        std::string title = "Watch History";
        int len = title.length();
        for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
        std::cout << "| " << title << " |\n";
        for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
        cinema_.listHistory(c, std::cout);
        for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
    }
    else {
        printBox("Error", "Not a customer\n");
    }
}

// Lists all users (admin only)
void CommandProcessor::doListUsers() {
    if (currentUser_ && currentUser_->isAdmin()) {
        std::string title = "User List";
        int len = title.length();
        for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
        std::cout << "| " << title << " |\n";
        for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
        cinema_.listUsers(std::cout);
        for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
    }
    else {
        printBox("Error", "Admin only\n");
    }
}

// Lists the watch history for a specific user (admin only)
void CommandProcessor::doListUserHistory(int userId) {
    std::string title = "User Watch History";
    int len = title.length();
    for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
    std::cout << "| " << title << " |\n";
    for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
    cinema_.listUserHistory(userId, std::cout);
    for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
}

// Lists all tickets for a specific user (admin only)
void CommandProcessor::doListUserTickets(int userId) {
    std::string title = "User Tickets";
    int len = title.length();
    for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
    std::cout << "| " << title << " |\n";
    for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
    cinema_.listUserTickets(userId, std::cout);
    for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
}

// Lists all hall layouts
void CommandProcessor::doListHalls() {
    std::string title = "Hall Layouts";
    int len = title.length();
    for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
    std::cout << "| " << title << " |\n";
    for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
    cinema_.listHalls(std::cout);
    for (int i = 0; i < len + 4; ++i) std::cout << "-"; std::cout << "\n";
}

// Handles ticket purchase for a customer
void CommandProcessor::doBuyTicket(int mid, int r, int c) {
    if (auto* cust = dynamic_cast<Customer*>(currentUser_)) {
        try {
            bool ok = cinema_.buyTicket(cust, mid, r, c);
            printBox(ok ? "Success" : "Error",
                ok ? "Ticket purchased\n"
                : "Purchase failed (maybe already taken or insufficient funds or hall is closed)\n");
        }
        catch (const std::exception& e) {
            printBox("Error", std::string(e.what()) + "\n");
        }
    }
    else {
        printBox("Error", "Please log in as customer\n");
    }
}

// Handles movie rating for a customer
void CommandProcessor::doRateMovie(int mid, int rating) {
    if (auto* cust = dynamic_cast<Customer*>(currentUser_)) {
        bool ok = cinema_.rateMovie(cust, mid, rating);
        printBox(ok ? "Success" : "Error",
            ok ? "Thank you for rating\n"
            : "Cannot rate this movie\n");
    }
    else {
        printBox("Error", "Please log in as customer\n");
    }
}

// Opens a new hall (admin only)
void CommandProcessor::doOpenHall(int rows, int cols) {
    try {
        int hid = cinema_.openHall(rows, cols);
        printBox("Success", "Opened hall " + std::to_string(hid) + "\n");
    }
    catch (const std::invalid_argument& e) {
        printBox("Error", std::string(e.what()) + "\n");
    }
}

// Closes a hall (admin only)
void CommandProcessor::doCloseHall(int hallId) {
    bool ok = cinema_.closeHall(hallId);
    printBox(ok ? "Success" : "Error",
        ok ? "Closed hall " + std::to_string(hallId) + "\n"
        : "Failed to close hall\n");
}

// Removes a movie (admin only)
void CommandProcessor::doRemoveMovie(int mid) {
    bool ok = cinema_.removeMovie(mid);
    printBox(ok ? "Success" : "Error",
        ok ? "Removed movie\n" : "Failed to remove movie\n");
}

// Checks if any tickets have been sold for a movie
void CommandProcessor::doHasTickets(int movieId) {
    bool ok = cinema_.hasTickets(movieId);
    printBox("Tickets sold?",
        ok ? "Yes, at least one ticket has been sold.\n"
        : "No tickets sold for that showing.\n");
}

// Updates a movie's title (admin only)
void CommandProcessor::doUpdateMovieTitle(int mid, const MyString& newTitle) {
    bool ok = cinema_.updateMovieTitle(mid, newTitle);
    printBox(ok ? "Success" : "Error",
        ok ? "Title updated\n" : "Failed to update title\n");
}

// Updates a movie's hall (admin only)
void CommandProcessor::doUpdateMovieHall(int mid, int newHall) {
    bool ok = cinema_.updateMovieHall(mid, newHall);
    if (ok)
        printBox("Success", "Moved to hall " + std::to_string(newHall) + "\n");
    else
        printBox("Error", "Failed to move movie (hall is busy at that time or invalid hall)\n");
}

// Removes a user (admin only)
void CommandProcessor::doRemoveUser(int userId) {
    bool ok = cinema_.removeUser(userId);
    printBox(ok ? "Success" : "Error",
        ok ? "Removed user\n" : "Failed to remove user\n");
}

// Deposits funds to the current user's account
void CommandProcessor::doDeposit(double amount) {
    if (!currentUser_) {
        printBox("Error", "Log in first\n");
        return;
    }
    currentUser_->deposit(amount);
    printBox("Success", "Deposited " + std::to_string(amount) + "\n");
}

// Shows the current user's balance
void CommandProcessor::doBalance() {
    if (!currentUser_) {
        printBox("Error", "Log in first\n");
        return;
    }
    double b = currentUser_->getBalance();
    printBox("Balance", "Your balance is: " + std::to_string(b) + "\n");
}

// Prints help information for all commands
void CommandProcessor::doHelp() {
    const std::string text =
        "register <user> <pwd>\n"
        "login <user> <pwd>\n"
        "logout\n"
        "exit\n"
        "help\n"
        "list-movies\n"
        "list-tickets\n"
        "list-history\n"
        "list-users\n"
        "buy-ticket <movieId> <row> <col>\n"
        "rate-movie <movieId> <rating>\n"
        "Admin-only:\n"
        "  open-hall <rows> <cols>\n"
        "  close-hall <hallId>\n"
        "  add-movie <genre> <title> <rating> <duration> <year> <hallId> <date> <sh> <sm> <eh> <em> [extra]\n"
        "  remove-movie <movieId>\n"
        "  update-movie-title <movieId> <new title>\n"
        "  update-movie-hall <movieId> <new hallId>\n"
        "  remove-user <userId>\n";
    printBox("Help", text);
}