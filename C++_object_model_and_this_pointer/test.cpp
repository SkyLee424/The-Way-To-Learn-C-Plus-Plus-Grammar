#include <iostream>
// class Test
// {
// public:
//     int val;
//     Test()
//     :val(0){}
//     Test(int val)
//     :val(val){}
//     Test(const Test &t)
//     {
//         std::cout << "Copy constructor called" << std::endl;
//         val = t.val;
//     }
//     ~Test()
//     {
//         std::cout << "Destructor called." << std::endl;
//     }
// };
class Test
{
    int age;

public:
    Test()
        : age(100) {}
    Test(int age)
        : age(age) {}
    void func(void)
    {
    }
    void Print(void) const
    {
        if (this)
        {
            std::cout << "Her age is " << age << "." << std::endl;
        }
    }
};
int main(void)
{
    const Test t0(18);
    t0.Print();
}