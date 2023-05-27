// 关系仿函数
#include<iostream>
#include<functional>
#include<algorithm>

// * `template<class T> bool equal_to<T>`                    //等于
// * `template<class T> bool not_equal_to<T>`               //不等于
// * `template<class T> bool greater<T>`                   //大于
// * `template<class T> bool greater_equal<T>`            //大于等于
// * `template<class T> bool less<T>`                    //小于
// * `template<class T> bool less_equal<T>`             //小于等于

void test(void)
{
    srand((unsigned int) time(NULL));
    std::vector<int> vec;
    for(int i = 0; i < 10; ++i)
    {
        vec.push_back(rand() % 101);
    }
    // 对 vec 进行降序排列
    // 我们当然可以自己写一个仿函数，或者是写一个 cmp 函数
    // 我们也可以直接使用 STL 提供的 greater 内建函数对象
    std::sort(vec.begin(), vec.end(), std::greater<int>());
    std::for_each(vec.begin(), vec.end(), [&](const int &val)
    {
        std::cout << val << " ";
    });
}

int main(void)
{
    test();
}