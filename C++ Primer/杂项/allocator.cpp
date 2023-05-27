#include <iostream>
#include <memory>
#include <vector>
#include <random>

// 使用 allocator 分配未构造的内存
// std::allocator<T> alloc
// alloc.allocate(n)
// 分配一段原始的、未构造的内存，存储 n 个类型为 T 的元素

void test0(void)
{
    std::allocator<std::string> alloc;
    auto const end = alloc.allocate(3);
    auto beg = end;
    *(beg++) = std::string();
    *(beg++) = std::string(10,'h');
    *(beg++) = "hello";
    while (beg != end)
    {
        std::cout << *beg << std::endl;
        --beg;
    }
}

void test1(void)
{
    std::vector<int> vec(10);
    std::default_random_engine e(time(NULL));
    std::uniform_int_distribution<int> u(0, 9);
    for(int i = 0; i < 10; ++i)
    {
        vec[i] = u(e);
    }
    std::allocator<int> alloc;
    // 前半部分用于 copy，后半部分用于 fill
    auto p = alloc.allocate(10 * 2);
    // uninitialized_copy(beg, end, p);
    // 将 beg ～ end 内的元素拷贝到 p 指向的 未分配的 内存中
    // 该内存空间应该至少能容纳拷贝的元素
    auto q = std::uninitialized_copy(vec.begin(), vec.end(), p);

    std::uninitialized_fill_n(q, 10, 1); // 在 q 指向的内存地址构造 10 个 1
    int cnt = 20;
    while (cnt--)
    {
        std::cout << *(p++) << " ";
    }
    std::cout << std::endl;
    
}


int main(void)
{
    test1();
}