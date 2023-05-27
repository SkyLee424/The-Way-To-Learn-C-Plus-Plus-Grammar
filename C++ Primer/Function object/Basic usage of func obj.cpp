#include<iostream>
#include<string>

// 如果类定义了调用运算符'()'，则该类的对象称为函数对象
// 函数对象使用重载的()时，行为类似函数调用，也叫仿函数

// 本质上是一个类，不是函数

// 特点：
// * 函数对象在使用时，可以像普通函数那样调用, 可以有参数，可以有返回值
// * 函数对象超出普通函数的概念，函数对象可以有自己的状态
// * 函数对象可以作为参数传递


// 1. 函数对象在使用时，可以像普通函数那样调用, 可以有参数，可以有返回值
struct MyAdd
{
    int operator()(const int &a, const int &b)
    {
        return a + b;
    }
};

void test0()
{
    MyAdd myAdd;
    int a = 1, b = 2;
    std::cout << myAdd(a, b) << std::endl;
}

// 2. 函数对象超出普通函数的概念，函数对象可以有自己的状态
class MyPrint
{
    // 状态：记录函数被调用的次数
    int _count;
public:
    MyPrint()
    :_count(0) {}
    
    template <typename T>
    void operator()(T text)
    {
        ++_count;
        std::cout << text << std::endl;
    }

    inline int count(void)
    {return _count;}
};

void test1(void)
{
    MyPrint myPrint;
    myPrint("NiuMa");
    myPrint(114514);
    std::cout << "'myPrint' has been called " << myPrint.count() << " time(s)" << std::endl;
}


// 3. 函数对象可以作为参数传递
template <typename T>
void doPrint(MyPrint &myPrint, const T &text)
{
    myPrint(text);
}

void test2(void)
{
    MyPrint mP;
    doPrint(mP, "Relx 5");
    doPrint(mP, 114);
}

int main(void)
{
    // test0();
    // test1();
    test2();
}