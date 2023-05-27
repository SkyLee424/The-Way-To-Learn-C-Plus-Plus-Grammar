#pragma once
#include <iostream>

struct A
{
    int val = 2;
    std::string s;

    A(int val = 2, std::string s = "default")
        : val(val), s(s) {}

    A(const A &a)
        : val(a.val), s(a.s)
    {
        std::cout << "拷贝构造" << std::endl;
    }

    A &operator=(const A &a)
    {
        val = a.val;
        s = a.s;
        std::cout << "拷贝赋值" << std::endl;
        return *this;
    }

    A(A &&a) noexcept
        : val(std::move(a.val)), s(std::move(a.s))
    {
        std::cout << "移动构造" << std::endl;
    }

    A &operator=(A &&a) noexcept
    {
        val = std::move(a.val);
        s = std::move(a.s);
        std::cout << "移动赋值" << std::endl;
        return *this;
    }

    ~A()
    {
        std::cout << "析构" << std::endl;
    }
};

std::ostream& operator<<(std::ostream &os, const A &a)
{
    os << a.val << " " << a.s;
    return os;
}