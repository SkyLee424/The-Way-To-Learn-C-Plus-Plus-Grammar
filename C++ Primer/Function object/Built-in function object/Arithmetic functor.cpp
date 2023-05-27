// 算数仿函数
#include<iostream>
#include<functional>

// * `template<class T> T plus<T>`                 //加法仿函数
// * `template<class T> T minus<T>`               //减法仿函数
// * `template<class T> T multiplies<T>`         //乘法仿函数
// * `template<class T> T divides<T>`           //除法仿函数
// * `template<class T> T modulus<T>`          //取模仿函数
// * `template<class T> T negate<T>`          //取反仿函数

struct Person
{
    int age;
    Person()
    : age(0) {}
    Person(int age)
    : age(age) {}
    int operator+(const Person &p) const
    {
        return age + p.age;
    }
    int operator-(void) const
    {
        return -age;
    }
    // int operator-(const Person &p) const
    // {
    //     return std::max(age, p.age) - std::min(age, p.age);
    // }
};


void test0(void)
{
    // plus
    // 二元仿函数
    // 先实例化一个对象
    // 模板参数列表仅有一个，即：只能实现相同数据类型的加法
    std::plus<int> p;
    std::cout << p(10, 20) << std::endl;
    Person p0(11), p1(21), p2;
    // 自定义数据类型
    // 需要类内重载 '+'
    std::plus<Person> _p;
    p2 = _p(p0, p1);
    std::cout << p2.age << std::endl;
}

void test1(void)
{
    std::negate<int> n;
    std::cout << n(10) << std::endl;

    Person p0(11);
    std::negate<Person> _n; 
    std::cout << _n(p0).age << std::endl; 
}

int main(void)
{
    test1();
}