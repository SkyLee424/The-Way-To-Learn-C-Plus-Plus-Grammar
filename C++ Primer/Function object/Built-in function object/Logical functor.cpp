// 逻辑仿函数
#include<iostream>
#include<functional>
#include<algorithm>
#include<vector>

// * `template<class T> bool logical_and<T>`              //逻辑与
// * `template<class T> bool logical_or<T>`                //逻辑或
// * `template<class T> bool logical_not<T>`              //逻辑非

std::ostream& operator<<(std::ostream& cout, const std::vector<bool> &vec)
{
    std::for_each(vec.begin(), vec.end(), [&](const bool &val)
    {
        val ? std::cout << "True " : std::cout << "False ";
    });
    return cout;
}

void test(void)
{
    std::vector<bool> vec;
    for(int i = 0; i < 5; ++i)
    {
        vec.push_back(i % 2 == 0);
    }
    std::vector<bool> vec1;
    vec1.resize(vec.size()); // 否则 vec1 中没有足够的空间用于 transform
    std::transform(vec.begin(), vec.end(), vec1.begin(), std::logical_not<bool>());
    std::cout << "Vec: \n" << vec << std::endl;
    std::cout << "Vec1: \n" << vec1 << std::endl;
}

int main(void)
{
    test();
}