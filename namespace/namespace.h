#include<iostream>
namespace test
{
    void print(void)
    {
        std::cout << "hello namespace" << std::endl;
    }
    void print1(void)
    {
        std::cout << "hello using" << std::endl;
    }
    // void print(void);
    // void print1(void);
    int a = 99;
    int b = 100;
    void print2(void)
    {
        std::cout << "hello alias" << std::endl;
    }
}
namespace test1
{
    double b = 99.99;
    namespace test2
    {
        void print2(void);
    }
    void test2::print2(void)
    {
        std::cout << "hello nestification" << std::endl;
    }
}
