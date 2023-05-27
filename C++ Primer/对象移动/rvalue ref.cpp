#include <iostream>

int func(void)
{
    return 0;
}

int main(void)
{
    int &&rr0 = 42;
    // rr0 是变量表达式
    // 变量表达式都是左值
    // int &&rr1 = rr0; // 错误，无法将右值引用绑定到左值
    int &&rr2 = std::move(rr0);
    rr0 = 43;
    std::cout << rr2 << std::endl; // 43

    int &&r1 = func();
    int &&r3 = std::move(r1);
    int &&r4 = std::move(r3 * func());
}