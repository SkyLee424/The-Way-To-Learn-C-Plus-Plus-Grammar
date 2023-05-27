#include<iostream>
#include<algorithm>
#include<vector>

// * 返回bool类型的仿函数称为**谓词**
// * 如果operator()接受一个参数，那么叫做一元谓词
// * 如果operator()接受两个参数，那么叫做二元谓词


// 一元谓词：
struct GreaterThan5
{
    bool operator()(int val) const
    {
        return val > 5;
    }
};

// 二元谓词
struct s_cmp
{
    bool operator()(int &v0, int &v1) const
    {
        return v0 > v1;
    }
};


bool greaterThan5(const int &val)
{
    return val > 5;
}

bool cmp(const int &v0, const int &v1)
{
    return v0 > v1;
}

void test0(void)
{
    std::vector<int> vec;
    for(int i = 0; i < 10; ++i)
        vec.push_back(i);

    std::vector<int>::iterator it = vec.begin();
    // std::find_if(_beg, _end, __pred)
    // _pred：谓词
    // 返回：指向第一个查找到的元素的迭代器
    // 此处可以创建一个匿名对象，传入 find_if
    while ((it = std::find_if(it, vec.end(), GreaterThan5())) != vec.end())
    {
        std::cout << "A number greater than 5 was found. The value is: " << *it << std::endl;
        ++it;
    }
    std::cout << "------------------------------------------------------" << std::endl;
    // 当然，也可以使用这种方式，即传入一个函数
    it = vec.begin();
    while ((it = std::find_if(it, vec.end(), greaterThan5)) != vec.end())
    {
        std::cout << "A number greater than 5 was found. The value is: " << *it << std::endl;
        ++it;
    }
    // 也可以利用 lambda（lambda 是函数对象）
    std::cout << "------------------------------------------------------" << std::endl;
    it = vec.begin();
    auto lambda = [](const int &val) -> bool
    {
        return val > 5;
    };
    while ((it = std::find_if(it, vec.end(), lambda)) != vec.end())
    {
        std::cout << "A number greater than 5 was found. The value is: " << *it << std::endl;
        ++it;
    }
}

void test1(void)
{
    srand((unsigned int) time(NULL));
    std::vector<int> vec;
    for(int i = 0; i < 10; ++i)
    {
        vec.push_back(rand() % 101);
    }
    // 二元谓词
    // std::sort(vec.begin(), vec.end(), s_cmp());
    // 也可以这样写
    std::sort(vec.begin(), vec.end(), cmp);
    // lambda
    std::for_each(vec.begin(), vec.end(), [&](const int &val)
    {
        std::cout << val << " ";
    });

}

int main(void)
{
    test0();
}
