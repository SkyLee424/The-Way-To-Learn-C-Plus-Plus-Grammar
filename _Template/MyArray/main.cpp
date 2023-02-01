#include <iostream>
#include <string>
#include <vector>
#include "MyArray.hpp"

class Person
{
public:
    std::string name;
    Person()
        : name("DingZheng") {}

    ~Person()
    {
        
    }
};

class Test
{
public:
    // 测试默认构造函数、push_back、pop_back
    static void test0(void)
    {
        Nstd::MyArray<int> arr0;
        std::vector<int> arr1;
        for (int i = 0; i < 5; ++i)
        {
            arr0.push_back(i);
            arr1.push_back(i);
        }
        arr0.pop_back();
        arr1.pop_back();
        std::cout << arr0 << std::endl;
        std::cout << arr1 << std::endl;
    }

    // 测试构造函数 1、push_back、重载 '[]'运算符
    static void test1(void)
    {
        Nstd::MyArray<int> arr0(5);
        std::vector<int> arr1(5);
        for (int i = 0; i < 5; ++i)
        {
            arr0[i] = i;
            arr1[i] = i;
        }
        for (int i = 0; i < 5; ++i)
        {
            arr0.push_back(i);
            arr1.push_back(i);
        }
        std::cout << arr0 << std::endl;
        std::cout << arr1 << std::endl;
    }

    // 测试构造函数 2、push_back
    static void test2(void)
    {
        Nstd::MyArray<int> arr0(5, 1);
        std::vector<int> arr1(5, 1);
        std::cout << arr0 << std::endl;
        std::cout << arr1 << std::endl;
        for (int i = 0; i < 5; ++i)
        {
            arr0.push_back(i);
            arr1.push_back(i);
        }
        std::cout << arr0 << std::endl;
        std::cout << arr1 << std::endl;
    }

    // 测试拷贝构造函数、重载 '=' 运算符、析构函数
    static void test3(void)
    {
        Nstd::MyArray<int> arr0(5, 1);
        Nstd::MyArray<int> arr1 = arr0;
        Nstd::MyArray<int> arr2;
        arr2 = arr0;
        std::cout << arr0 << std::endl;
        std::cout << arr1 << std::endl;
        std::cout << arr2 << std::endl;
    }

    // 测试重载 '=' 运算符
    static void test4(void)
    {
        Nstd::MyArray<int> arr0(5, 1);
        Nstd::MyArray<int> arr1, arr2;
        arr1 = arr2 = arr0;
        std::cout << arr0 << std::endl;
        std::cout << arr1 << std::endl;
        std::cout << arr2 << std::endl;
    }

    // 测试 size()、capacity() 函数
    static void test5(void)
    {
        Nstd::MyArray<int> arr0;
        std::vector<int> arr1;
        for (int i = 0; i < 5; ++i)
        {
            arr0.push_back(i);
            arr1.push_back(i);
        }
        arr0.pop_back();
        arr1.pop_back();
        std::cout << arr0.size() << " " << arr1.size() << std::endl;
        std::cout << arr0.capacity() << " " << arr1.capacity() << std::endl;
    }

    static void test6(void)
    {
        Nstd::MyArray<Person> arr0(3);
        std::vector<Person> arr1(3);
        for(int i = 0; i < 3; ++i)
        {
            std::cout << arr0[i].name << " ";
        }
        std::cout << std::endl;
        for(int i = 0; i < 3; ++i)
        {
            std::cout << arr1[i].name << " ";
        }
        std::cout << std::endl;
    }
};

int main(void)
{
    int op;
    std::cin >> op;
    switch (op)
    {
    case 0:
        Test::test0();
        break;
    case 1:
        Test::test1();
        break;
    case 2:
        Test::test2();
        break;
    case 3:
        Test::test3();
        break;
    case 4:
        Test::test4();
        break;
    case 5:
        Test::test5();
        break;
    case 6:
        Test::test6();
        break;
    default:
        break;
    }
}