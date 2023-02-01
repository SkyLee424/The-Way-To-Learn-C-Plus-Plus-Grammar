#include <iostream>
#include <string>

/**
 *  @brief 模板类 Person
 *
 *  @ingroup sequences
 *
 *  @tparam nameType  Type of name
 *  @tparam ageType   Type of age, default to int
 */
template <class nameType, class ageType = int>
class Person
{
public:
    nameType name;
    ageType age;
    Person(nameType name, ageType age)
        : name(name), age(age) {}
};

// 重载 << 运算符，用于输出Person类的成员
// 函数模板配合类模板
template <typename nameType, typename ageType>
std::ostream &operator<<(std::ostream &cout, const Person<nameType, ageType> &p)
{
    std::cout << "name: " << p.name << ", age: " << p.age << std::endl;
    return cout;
}

class Person1
{
public:
    void showPerson1(void)
    {
        std::cout << "Calling showPerson1" << std::endl;
    }
};

class Person2
{
public:
    void showPerson2(void)
    {
        std::cout << "Calling showPerson2" << std::endl;
    }
};

template <typename T>
class ShowPerson
{
    T person;

public:
    void show1(void)
    {
        person.showPerson1();
    }
    void show2(void)
    {
        person.showPerson2();
    }
};

class Test
{
public:
    // 基础语法
    static void test0(void)
    {
        Person<std::string, int> p("DingZheng", 999);
        std::cout << p;
        Person<const char *, double> p1("DingZheng", 114.514);
        std::cout << p1;
    }

    // 类模板与函数模板的区别
    // 1. C++ 17 标准支持类模板使用自动类型推导，以前的（如 C++ 11）不行
    // 2. 类模板的模板参数列表允许有默认参数
    static void test1(void)
    {
        Person p0("dingZheng", 999.9);        // 支持类模板使用自动类型推导
        Person<std::string> p1("NiuMa", 114); // 允许有默认参数
        std::cout << p0 << p1;
    }

    // 类模板成员函数的创建时机
    // 不是一开始就创建
    // 而是在调用时创建
    // 因为在调用前，编译器也不知道参数类型到底是什么
    static void test2(void)
    {
        ShowPerson<Person1> show;
        // 注释掉下面两行，能正常编译
        // 说明：只要不调用成员函数，它就不会创建
        show.show1();
        // show.show2(); // 编译出错，因为 Person1 没有 showPerson2 这个成员
    }
};

int main(void)
{
    Test::test0();
}