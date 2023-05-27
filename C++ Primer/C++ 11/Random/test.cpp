#include <iostream>
#include <random>
#include <vector>

template <typename T = int>
void print(std::default_random_engine &e, std::uniform_int_distribution<T> &u, int length = 10)
{
    for(int i = 0; i < length; ++i)
    {
        std::cout << u(e) << " ";
    }
    std::cout << std::endl;
}

template <typename T = double>
void print(std::default_random_engine &e, std::uniform_real_distribution<T> &u, int length = 10)
{
    for(int i = 0; i < length; ++i)
    {
        std::cout << u(e) << " ";
    }
    std::cout << std::endl;
}

void test0(void)
{
    std::default_random_engine e; // 随机数引擎对象
    std::uniform_int_distribution<int> u(0, 1); // 分布对象
    print(e, u);
}

// 观察 test0，每次生成的随机数序列事实上是一样的
// 虽然有助于调试，但这与我们最终要达到的需求是不同的

// 解决方式 1：将 引擎和分布对象 声明为 静态对象

void test1(void)
{
    static std::default_random_engine e;
    static std::uniform_int_distribution<int> u(0, 9);
    print(e, u);
}

void verify(void)
{
    test0();
    test0();
    std::cout << "-------------------" << std::endl;
    test1();
    test1();
}

// 解决方式 2：使用随机数种子

void test2(void)
{
    // 在初始化时给定种子
    std::default_random_engine e0(114514);
    std::uniform_int_distribution<int> u(0, 9);
    print(e0, u);
    
    // 使用成员函数 seed
    std::default_random_engine e1;
    e1.seed(114514);
    print(e1, u);

    // 使用 time 的返回值做 seed
    std::cout << "About to print the result of e2..." << std::endl;
    std::default_random_engine e2;
    e2.seed(time(NULL));
    for(int i = 0; i < 3; ++i)
    {
        print(e2, u);
    }
}

// 注意：如果设置种子的时间间隔小于 1s，使用这个方法就无效了
// 有可能使用多个相同的种子
void test3(int loops = 3)
{
    if(loops == 0)
        return ;
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0, 9);
    e.seed(time(NULL));
    print(e, u);
    test3(loops - 1);
}

// 生成非均匀分布的随机数

void test4(void)
{
    std::default_random_engine e(114514);
    // 正态分布
    // 均值 == 4，标准差 == 1.5
    // 期望生成的数有 99% 都在 [0, 8] 内
    std::normal_distribution<> n(4, 1.5);
    std::vector<int> cnt(9);
    for(int i = 0; i < 200; ++i) // 生成 200 个随机数
    {
        auto val = lround(n(e)); // 将 n(e) 转换为 最接近的整数
        if(val >= 0 && val <= 8)
            ++cnt[val];
    }
    for(int i = 0; i < 8; ++i)
    {
        std::cout << i << ": " << std::string(cnt[i], '*') << std::endl;
    }
}

// 注意：需将 引擎、分布对象 定义在循环外部
// 因为要保持它们的状态，否则，
// 每次循环都新建一个引擎或者分布对象
// 导致生成相同的值
void test5(void)
{
    std::default_random_engine e; // 随机数引擎对象
    std::uniform_int_distribution<int> u(0, 1); // 分布对象
    for(int i = 0; i < 3; ++i)
    {
        print(e, u);
    }
    std::cout << "---------------" << std::endl;
    std::cout << "引擎定义在循环内部" << std::endl;
    for(int i = 0; i < 3; ++i)
    {
        std::default_random_engine e1;
        print(e1, u);
    }
    std::cout << "---------------" << std::endl;
    std::cout << "分布对象定义在循环内部" << std::endl;
    for(int i = 0; i < 3; ++i)
    {
        // print(e, u1);
        for(int i = 0; i < 10; ++i)
        {
            // 似乎没啥影响？
            std::uniform_int_distribution<int> u1(0, 9);
            std::cout << u1(e) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "---------------" << std::endl;
    std::cout << "引擎、分布对象定义在循环内部" << std::endl;
    for(int i = 0; i < 3; ++i)
    {
        std::default_random_engine e1;
        std::uniform_int_distribution<int> u1(0, 9);
        print(e1, u1);
    }
}

int main(void)
{
    
}