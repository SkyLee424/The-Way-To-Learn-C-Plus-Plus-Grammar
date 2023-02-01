#include <iostream>
// // 函数重载的必要条件：
// // 1. 作用域相同
// // 2. 函数名相同
// // 3. 函数参数不同（可以是类型不同、数量不同、顺序不同等）

// void func(void)
// {
//     std::cout << "func(void)" << std::endl;
// }

// void func(int a)
// {
//     std::cout << "func(int a)" << std::endl;
// }

// void func(double a)
// {
//     std::cout << "func(double a)" << std::endl;
// }

// void func(int a, int b)
// {
//     std::cout << "func(int a, int b)" << std::endl;
// }

// // 需要注意的是：无法仅按返回值来重载函数
// // 例如：

// int func(int a, int b)
// {
//     std::cout << "func(int a, int b)" << std::endl;
//     return 0;
// }

void func(int &a)
{
    std::cout << "func(int &a)" << std::endl;
}

void func(const int &a)
{
    std::cout << "func(const int &a)" << std::endl;
}

// void func(int a, int b = 10)
// {
//     std::cout << "func(int a, int b = 10)" << std::endl;
// }

int main(void)
{
    // func();
    // func(0);
    // func(0.0);
    // func(0, 0);
    int a = 10;
    const int b = 10;
    func(a);
    func(b);
    // func(a, b);
}