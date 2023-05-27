#include <iostream>

void f(int v0, int &v1)
{
    std::cout << "In Function F" << std::endl;
    std::cout << v0 << " " << ++v1 << std::endl;
}

void g(int &&v0, int &v1)
{
    std::cout << "In Function G" << std::endl;
    std::cout << v0 << " " << v1 << std::endl;
}

template <typename F, typename T0, typename T1>
void filp0(F f, T0 t0, T1 t1)
{
    std::cout << "In Function Filp0" << std::endl;
    std::cout << t0 << " " << t1 << std::endl;
    f(t1, t0);
    std::cout << "In Function Filp0" << std::endl;
    std::cout << t0 << " " << t1 << std::endl;
}

// t0, t1 的类型的 const 属性以及 左/右值属性 得到保持
// 与模版结合后，t0、t1 被称为通用引用（universal references），而不是右值引用
// 既可以接受左值，也可以接受右值
template <typename F, typename T0, typename T1>
void filp1(F f, T0 &&t0, T1 &&t1)
{
    std::cout << "In Function Filp1" << std::endl;
    std::cout << t0 << " " << t1 << std::endl;
    f(t1, t0);
    std::cout << "In Function Filp1" << std::endl;
    std::cout << t0 << " " << t1 << std::endl;
}

template <typename F, typename T0, typename T1>
void filp2(F f, T0 &&t0, T1 &&t1)
{
    std::cout << "In Function Filp2" << std::endl;
    std::cout << t0 << " " << t1 << std::endl;
    // filp2(g, t0, 42); // 错误，不能从一个左值实例化 int&&
    // t1 的类型为 int&&，是一个左值，不能用一个左值初始化一个右值引用
    // 因此，此处对 g 的调用相当于给 g 的 v0 传递了一个左值
    g(t1, t0); 
    std::cout << "In Function Filp2" << std::endl;
    std::cout << t0 << " " << t1 << std::endl;
}

// std::forward<T> 与 universal references 共同实现完美转发
template <typename F, typename T0, typename T1>
void filp3(F f, T0 &&t0, T1 &&t1)
{
    std::cout << "In Function Filp3" << std::endl;
    std::cout << t0 << " " << t1 << std::endl;
    // 当用于一个指向模版参数类型的右值引用函数参数时，forward 会保留实参类型的所有细节
    g(std::forward<T1>(t1), std::forward<T0>(t0)); 
    std::cout << "In Function Filp3" << std::endl;
    std::cout << t0 << " " << t1 << std::endl;
}

struct Test
{
    static void test0(void)
    {
        int t0 = 0, t1 = 0;
        filp0(f, t0, t1);
        std::cout << "In Function Test0" << std::endl;
        std::cout << t0 << " " << t1 << std::endl;
    }

    static void test1(void)
    {
        int t0 = 0, t1 = 0;
        filp1(f, t0, t1);
        std::cout << "In Function Test1" << std::endl;
        std::cout << t0 << " " << t1 << std::endl;
    }

    static void test2(void)
    {
        int t0 = 1;
        // filp2(g, t0, 42); // 错误，不能从一个左值实例化 int&&
    }

    static void test3(void)
    {
        int t0 = 1;
        filp3(g, t0, 42);
    }
};