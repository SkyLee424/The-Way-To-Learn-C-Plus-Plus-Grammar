#include"Workers.h"
void Workers::showBasicInfo(void)
{
    std::cout << "Name: " << name << std::endl;
    std::cout << "Worker's ID: " << id << std::endl;
    std::cout << "Sex: " << sex << std::endl;
    std::cout << "Age: " << age << std::endl;
}

Staff::Staff(const std::string &name, char sex, int age, const std::string &id)
{
    this->name = name;
    this->sex = sex;
    this->age = age;
    this->id = id;
    this->departmentId = "0";
}

void Staff::showInfo(void)
{
    showBasicInfo();
    std::cout << "Department: Staff" << std::endl;
    std::cout << "Responsibilities: Complete the manager's task." << std::endl;
}

std::string Staff::getDeptName(void)
{
    return "Staff";
}

Manager::Manager(const std::string &name, char sex, int age, const std::string &id)
{
    this->name = name;
    this->sex = sex;
    this->age = age;
    this->id = id;
    this->departmentId = "1";
}

void Manager::showInfo(void)
{
    showBasicInfo();
    std::cout << "Department: Manager" << std::endl;
    std::cout << "Responsibilities: Complete the boss's task and have issued the task to the employees." << std::endl;
}

std::string Manager::getDeptName(void)
{
    return "Manager";
}

Boss::Boss(const std::string &name, char sex, int age, const std::string &id)
{
    this->name = name;
    this->sex = sex;
    this->age = age;
    this->id = id;
    this->departmentId = "2";
}

void Boss::showInfo(void)
{
    showBasicInfo();
    std::cout << "Department: Boss" << std::endl;
    std::cout << "Responsibilities: manage all affairs of the company." << std::endl;
}

std::string Boss::getDeptName(void)
{
    return "Boss";
}