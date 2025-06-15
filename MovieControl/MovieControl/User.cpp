#include "User.h"

User::User(int id, const MyString& name, const MyString& password, double balance)
    : id_(id)
    , name_(name)
    , password_(password)
    , balance_(balance)
{
}

bool User::authenticate(const MyString& password) const {
    return password_ == password;
}

void User::deposit(double amount) {
    if (amount > 0) balance_ += amount;
}

bool User::charge(double amount) {
    if (amount < 0 || balance_ < amount) return false;
    balance_ -= amount;
    return true;
}

