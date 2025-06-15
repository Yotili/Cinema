# MovieControl Cinema Management System

## Overview

**MovieControl** is a C++14 console application for managing a cinema's operations, including user registration, movie scheduling, ticket sales, and administrative tasks. The system supports both regular customers and admin users, with persistent storage for all data (halls, movies, users, tickets).

## Features

- **User Management**
  - Register and log in as a customer or admin
  - Deposit funds and check balance
  - View and manage tickets and watch history

- **Admin Features**
  - Open and close cinema halls
  - Add, update, and remove movies
  - Move movies between halls (with time slot collision checks)
  - List all users and their tickets/history
  - Remove users

- **Movie & Ticket Management**
  - List all movies and hall layouts
  - Buy tickets for available seats and showings
  - Rate movies (if watched)
  - Automatic ticket expiration and history tracking

- **Persistence**
  - All data is saved to and loaded from the `data/` directory (`halls.txt`, `movies.txt`, `users.txt`)

## Getting Started

### Prerequisites

- Visual Studio 2022 (or any C++14-compatible compiler)
- C++14 standard support

### Building

1. Clone or download the project files.
2. Open the solution in Visual Studio 2022.
3. Build the solution (all dependencies are local, no external libraries required).

### Running

1. Run the compiled executable.
2. On first run, a default admin user is created:
   - **Username:** `admin`
   - **Password:** `admin`
3. Data files will be created in the `data/` directory as needed.

## Usage

### Basic Commands

- `register <username> <password>` — Register a new customer
- `login <username> <password>` — Log in as a user
- `logout` — Log out of the current session
- `deposit <amount>` — Add funds to your account
- `balance` — Show your current balance
- `list-movies` — List all available movies
- `list-tickets` — Show your upcoming tickets
- `list-history` — Show your watch history
- `buy-ticket <movieId> <row> <col>` — Buy a ticket for a movie
- `rate-movie <movieId> <rating>` — Rate a movie you have watched
- `help` — Show all available commands
- `exit` — Save and exit the application

### Admin-Only Commands

- `open-hall <rows> <cols>` — Open a new hall
- `close-hall <hallId>` — Close a hall (refunds tickets)
- `add-movie <genre> <title> <rating> <duration> <year> <hallId> <date> <sh> <sm> <eh> <em> [extra]`
  - `genre`: `Action`, `Drama`, or `Documentary`
  - `[extra]`: For `Action` (intensity 0–20), for `Drama` (hasComedy: 0/1), for `Documentary` (theme and basedOnTrue: 0/1)
- `remove-movie <movieId>` — Remove a movie
- `update-movie-title <movieId> <new title>` — Change a movie's title
- `update-movie-hall <movieId> <new hallId>` — Move a movie to another hall
- `remove-user <userId>` — Remove a user
- `list-users` — List all users
- `list-user-history <userId>` — Show a user's watch history
- `list-user-tickets <userId>` — Show a user's tickets

## Data Files

- `data/halls.txt` — Hall definitions and seat layouts
- `data/movies.txt` — Movie catalog and showtimes
- `data/users.txt` — User accounts, tickets, and histories

## Notes

- All input is case-sensitive.
- Dates must be in `YYYY-MM-DD` format.
- Time is in 24-hour format.
- Only admins can perform administrative commands.
- Tickets for past showings are automatically moved to history.
