#include "Admin.h"

Admin::Admin(int id,
    const MyString& name,
    const MyString& password,
    double balance)
    : User(id, name, password, balance)
{
}

