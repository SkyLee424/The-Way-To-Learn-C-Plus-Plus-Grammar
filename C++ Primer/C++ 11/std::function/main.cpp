#include <iostream>
#include <functional>
#include <unordered_map>

int add(int a, int b)
{
    return a + b;
}

struct multiply
{
    int operator()(int a, int b)
    {
        return a * b;
    }
};

void test0(void)
{
    using pf = int(*)(int, int);
    std::unordered_map<std::string, pf> oper;
    auto subtract = [](int a, int b) -> int
    {
        return a - b;
    };
    oper.insert({"+", add});
    oper.insert({"-", subtract}); // 错误？
    // oper.insert({"*", multiply()}); // 错误，muliply 不是函数指针
    std::cout << oper["-"](10, 2) << std::endl;
}

void test1(void)
{
    std::function<int(int, int)> f1 = add;
    auto subtract = [](int a, int b) -> int
    {
        return a - b;
    };
    std::function<int(int, int)> f2 = subtract;
    std::function<int(int, int)> f3 = multiply();
    std::cout << f1(1, 2) << std::endl;
    std::cout << f2(1, 2) << std::endl;
    std::cout << f3(1, 2) << std::endl;
}

// 利用 std::function 解决上述问题
void test2(void)
{
    std::unordered_map<std::string, std::function<int(int, int)>> oper;
    auto subtract = [](int a, int b) -> int
    {
        return a - b;
    };
    oper.insert({"+", add}); // 函数指针
    oper.insert({"-", subtract});  // 匿名 lambda
    oper.insert({"*", multiply()}); // 函数对象
    oper.insert({"/", std::divides()}); // 内置函数对象
}

// 重载函数与 std::function
struct Test
{
    int val;   
    /*explicit*/ Test(int val)
        :val(val){}
};

Test add(Test a, Test b)
{
    return a.val + b.val; // 隐式类型转换
}

void test3(void)
{
    std::unordered_map<std::string, std::function<int(int, int)>> oper;
    // oper.insert({"+", add}); // 错误，add 不明确
    
    // 解决方式
    // 1. 利用函数指针
    int (*pAdd)(int, int) = add;
    oper.insert({"+", pAdd}); // 正确

    // 2. 利用 lambda
    oper.insert({"+", [](int a, int b)
    {
        return add(a, b);
    }});
}

int main(void)
{
    test2();
}