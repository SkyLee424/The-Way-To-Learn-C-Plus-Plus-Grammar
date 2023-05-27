#pragma once

struct A
{
    static int copyConstructCnt;
    static int moveConstructCnt;
    int val = 2;
    std::string s;

    A(int val, std::string s = "default")
        :val(val), s(s) {}

    A(const A &a)
        :val(a.val), s(a.s)
    {
        std::cout << "拷贝构造" << std::endl;
        ++copyConstructCnt;
    }

    A& operator=(const A &a)
    {
        val = a.val;
        s = a.s;
        std::cout << "拷贝赋值" << std::endl;
        return *this;
    }

    A(A &&a) noexcept
        :val(std::move(a.val)), s(std::move(a.s))
    {
        std::cout << "移动构造" << std::endl;
        ++moveConstructCnt;
    }

    A& operator=(A &&a) noexcept
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

int A::copyConstructCnt = 0;
int A::moveConstructCnt = 0;

std::ostream& operator<<(std::ostream &os, const A &a)
{
    os << a.val << " " << a.s;
    return os;
}