#include "Ticket.h"

// out-of-line definition matching the declaration above:
Ticket::Ticket(int id, int movieId, int row, int col, double price)
    : id_(id)
    , movieId_(movieId)
    , row_(row)
    , col_(col)
    , price_(price)
{
}
