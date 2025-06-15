#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"

class Admin : public User {
public:
    Admin(int id,
        const MyString& name,
        const MyString& password,
        double balance = 0.0);

    bool isAdmin() const override { return true; }
};

#endif