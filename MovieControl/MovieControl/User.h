#ifndef USER_H
#define USER_H

#include "MyString.h"

class User {
protected:
    int      id_;
    MyString name_;
    MyString password_;
    double   balance_;

public:
    User(int id, const MyString& name, const MyString& password, double balance = 0.0);
    virtual ~User() = default;

    int        getId()      const { return id_; }
    const MyString& getName()  const { return name_; }
    double     getBalance() const { return balance_; }

    const MyString& getPassword() const { return password_; }

    /// check password
    bool       authenticate(const MyString& password) const;

    /// add funds
    void       deposit(double amount);

    /// attempt to deduct; returns false if insufficient
    bool       charge(double amount);

    /// only Admin overrides this
    virtual bool isAdmin() const { return false; }
};

#endif

