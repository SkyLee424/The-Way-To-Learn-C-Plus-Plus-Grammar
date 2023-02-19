#pragma once
#include "kernel.h"
#include "status.h"
constexpr int Student = 0;
constexpr int Teacher = 1;
constexpr int Admin = 2;

class Identity
{
protected:
    std::string name;
    std::string id;
    std::string password;

public:
    Identity();

    Identity(std::string name, std::string id);

    virtual ~Identity();

    void basicLogin(void);

    virtual Status login(void) = 0;

    virtual void showMenu(void) = 0;

    void logout(void);
};

class _Student : public Identity
{
    ~_Student(void);

    Status login(void);

    void showMenu(void);

    Status applyForAppointment(void);

    Status cancelOrder(void);
};

class _Teacher : public Identity
{
    ~_Teacher(void);

    Status login(void);

    Status reviewAppointment();

    void showMenu(void);
};

class _Admin : public Identity
{
    ~_Admin(void);

    Status login(void);

    void showMenu(void);

    int addAccount(int identity);

    Status viewAccount(int identity);

    Status emptyOrders(void);

};