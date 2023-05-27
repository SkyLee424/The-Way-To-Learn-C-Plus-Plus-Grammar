#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
using namespace std::placeholders; // 有占位符的定义

template <typename T>
std::ostream &operator<<(std::ostream &cout, const std::vector<T> &arr)
{
    for (const T &num : arr)
        std::cout << num << " ";
    return cout;
}

bool cmp(const int &a, const int &b)
{
    return a > b;
}

bool biggerThanBase(const int &val, int base)
{
    return val > base;
}

void test0(void)
{
    std::vector<int> arr = {1, 1, 1, 2, 1, 1, 3, 3, 2, 2, 6, 5, 6, 5};
    std::sort(arr.begin(), arr.end(), cmp);
    std::cout << arr << std::endl;
    // bind(funcName, args)
    // 例如，auto cmp0 = std::bind(cmp, _1, base)
    // 其中，cmp 是函数名，形如 _1 是占位符，占据了传给 cmp 的第一个参数的位置
    // base 是传给 cmp 的第二个参数
    auto cmp0 = std::bind(cmp, _2, _1); // cmp0(a, b) == cmp(b, a)
    std::sort(arr.begin(), arr.end(), cmp);
    std::cout << arr << std::endl;
}

void test1(void)
{
    std::vector<int> arr = {1, 1, 1, 2, 1, 1, 3, 3, 2, 2, 6, 5, 6, 5};
    // partition 接受一个一元谓词
    // 因此，直接将 biggerThanBase 传给 partition 不行
    // bind 就可以用于解决这个问题
    int base = 3;
    std::partition(arr.begin(), arr.end(), std::bind(biggerThanBase, _1, base));
    std::cout << arr << std::endl;
}

void printByFunction(std::ostream &os, const char &ch)
{
    os << ch << " ";
}

void test2(void)
{
    auto &os = std::cout;
    auto printByLambda = [&os](const char &ch) -> void
    {
        os << ch << " ";
    };

    std::string s = "Hello, ref!";

    std::for_each(s.begin(), s.end(), printByLambda);
    std::cout << std::endl;

    // 错误，不能拷贝 os
    // std::for_each(s.begin(), s.end(), std::bind(printByFunction, os, _1));
    // 使用 std::ref() 获得 os 的引用
    // 也定义在 functional 里
    std::for_each(s.begin(), s.end(), std::bind(printByFunction, std::ref(os), _1));
    std::cout << std::endl;
}

int main(void)
{
    // test0();
    // test1();
    test2();
}