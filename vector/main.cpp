#include "vector.h"
#include "A.h"
#include <vector>

template <typename T>
std::ostream &operator<<(std::ostream &cout, const std::vector<T> &arr)
{
    for (const T &num : arr)
        std::cout << num << " ";
    return cout;
}

void test0(void)
{
    LTC::vector<int> vec0;
    LTC::vector<int> vec1(10);
    LTC::vector<int> vec2(10, 1);
    LTC::vector<int> vec3 = {1, 2, 3, 4};
    std::cout << vec0 << std::endl;
    std::cout << vec1 << std::endl;
    std::cout << vec2 << std::endl;
    std::cout << vec3 << std::endl;
    LTC::vector<int> vec4 = vec1;
    LTC::vector<int> vec5 = std::move(vec2), vec6, vec7;
    vec6 = vec3;
    vec7 = std::move(vec3);
    std::cout << vec4 << std::endl;
    std::cout << vec5 << std::endl;
    std::cout << vec6 << std::endl;
    std::cout << vec7 << std::endl;
}

void test1(void)
{
    LTC::vector<int> vec = {1, 4, 3, 2, 5};
    std::cout << vec.at(0) << " " << vec[1] << std::endl;
    std::cout << *vec.begin() << " " << *(vec.end() - 1) << std::endl;
    std::cout << *vec.rbegin() << " " << *(vec.rend() - 1) << std::endl;
    std::cout << *vec.crbegin() << " " << *(vec.crend() - 1) << std::endl;
    std::sort(vec.begin(), vec.end());
    std::cout << vec << std::endl;

    // *(vec.cbegin()) = 1;
    // *(vec.crbegin()) = 1;
    const LTC::vector<int> cVec = {5, 2, 3, 1, 4};
    // *(cVec.begin()) = 1;
}

void test2(void)
{
    // reserve
    // 由于 LTC::vector 并不是先 allocate 再 construct
    // 如果 A 类没有默认构造函数，将会出现问题
    // 导致以下代码报错
    // 待修改
    // 2023.5.22 已修改
    // std::vector<A> vec(10, 1);
    LTC::vector<A> vec(10, 1);
    std::cout << vec.size() << " " << vec.capacity() << std::endl; // 10 10
    vec.reserve(20);
    std::cout << vec.size() << " " << vec.capacity() << std::endl; // 10 20
    vec.reserve(5);
    std::cout << vec.size() << " " << vec.capacity() << std::endl; // 10 20

    // 以下代码与 STL 的 vector 运行结果不同
    // 应该是 resize 机制不同
    // 2023.5.22 已解决
    vec.resize(30, 2);
    std::cout << vec.size() << " " << vec.capacity() << std::endl; // 30 40
}

void test3(void)
{
    // resize
    LTC::vector<int> vec(10, 1);
    std::cout << vec.size() << " " << vec.capacity() << std::endl;
    std::cout << vec << std::endl;

    vec.resize(15, 2);
    // resize 在 newSize > capacity 时，会尝试扩容到原来的两倍
    // 如果还不够，那就以 newSize 作为新的 capacity
    std::cout << vec.size() << " " << vec.capacity() << std::endl; // 15 20
    std::cout << vec << std::endl;

    vec.resize(50);
    std::cout << vec.size() << " " << vec.capacity() << std::endl; // 50 50
    std::cout << vec << std::endl;
}

void test4(void)
{
    // shrink_to_fit
    LTC::vector<A> vec(10, 1);
    std::cout << vec.size() << " " << vec.capacity() << std::endl;
    vec.reserve(20);
    std::cout << vec.size() << " " << vec.capacity() << std::endl;
    vec.shrink_to_fit();
    std::cout << vec.size() << " " << vec.capacity() << std::endl;
}

void test5(void)
{
    LTC::vector<A> vec(10, 1);
    vec.clear();
    std::cout << vec.size() << " " << vec.capacity() << std::endl;
    // 后面还要析构的原因也是因为：
    // 没有先 allocate 再 construct
    // 而是 allocate 的同时 construct
    // 2023.5.22 已解决
}

void test6(void)
{
    LTC::vector<A> vec;
    // std::vector<A> vec;
    A a(6);
    for (int i = 1; i <= 4; ++i)
    {
        vec.push_back(a); // const A &val
    }
    for(int i = 5; i <= 17; ++i)
    {
        vec.push_back(i); // A &&val
    }
    // std::cout << vec << std::endl;
    std::cout << vec.size() << " " << vec.capacity() << std::endl;
    std::cout << "copyConstructCnt = " << a.copyConstructCnt << std::endl;
    std::cout << "moveConstructCnt = " << a.moveConstructCnt << std::endl;
}

void test7(void)
{
    LTC::vector<A> vec;
    // std::vector<A> vec;
    for (int i = 1; i <= 5; ++i)
    {
        vec.emplace_back(i, "hhh");
    }
    std::cout << vec << std::endl;
    std::cout << vec.size() << " " << vec.capacity() << std::endl;
    std::cout << "copyConstructCnt = " << A::copyConstructCnt << std::endl;
    std::cout << "moveConstructCnt = " << A::moveConstructCnt << std::endl;
}

// insert(const_iterator _pos, _InputIterator _first, _InputIterator _last)
void test8(void)
{
    std::vector<A> vec = {1, 2, 3, 4};
    // LTC::vector<A> vec = {1, 2, 3, 4};
    std::vector<A> temp = {5, 6, 7};
    auto it = vec.insert(vec.begin() + 1, temp.begin(), temp.end());
    std::cout << vec << std::endl;
    std::cout << *it << std::endl;
    std::cout << vec.size() << " " << vec.capacity() << std::endl; // 7 8
    std::cout << vec << std::endl;
}

// insert(_pos, size_t, const _Tp &val) 
void test9(void)
{
    // std::vector<A> vec1 = {1, 2, 3};
    LTC::vector<A> vec1 = {1, 2, 3};
    auto it = vec1.insert(vec1.begin() + 2, size_t(10), 6); 

    std::cout << vec1 << std::endl;
    std::cout << *it << std::endl;
    std::cout << vec1.size() << " " << vec1.capacity() << std::endl; // 13 13
    std::cout << vec1 << std::endl;
}

// insert(_pos, const _Tp &val) 
void test10(void)
{
    // std::vector<A> vec2 = {2, 3, 4, 5, 6};
    LTC::vector<A> vec2 = {2, 3, 4, 5, 6};
    A a(1);
    auto it = vec2.insert(vec2.begin() + 1, a);
    std::cout << vec2 << std::endl;
    std::cout << *it << std::endl;
    std::cout << vec2.size() << " " << vec2.capacity() << std::endl; // 4 6
    std::cout << vec2 << std::endl;
}

// insert(_pos, _Tp &&val) 
void test11(void)
{
    // std::vector<A> vec3 = {2, 3, 4};
    LTC::vector<A> vec3 = {2, 3, 4};
    auto it = vec3.insert(vec3.begin() + 2, 1);
    std::cout << vec3 << std::endl;
    std::cout << *it << std::endl;
    std::cout << vec3.size() << " " << vec3.capacity() << std::endl; // 4 6
    std::cout << vec3 << std::endl;
}

// insert(_pos, const std::initializer_list<A> &list)
void test12(void)
{
    // std::vector<A> vec4 = {6, 7};
    LTC::vector<A> vec4 = {6, 7};
    auto it = vec4.insert(vec4.begin(), {2, 3, 4});
    std::cout << vec4 << std::endl;
    std::cout << *it << std::endl;
    std::cout << vec4.size() << " " << vec4.capacity() << std::endl; // 5 5
    std::cout << vec4 << std::endl;
}

// emplace
void test13(void)
{
    LTC::vector<A> vec(3, 1);
    vec.emplace(vec.begin() + 1, 2);
    // vec.assign(21, 2);
    std::cout << vec.size() << " " << vec.capacity() << std::endl; // 4 6
}

// assign(const std::initializer_list<A> &list)
void test14(void)
{
    // std::vector<A> vec(3, 1);
    LTC::vector<A> vec(3, 1);
    vec.assign({1, 2, 3, 4});
    std::cout << vec.size() << " " << vec.capacity() << std::endl;
    std::cout << "copyConstructCnt = " << A::copyConstructCnt << std::endl;
    std::cout << "moveConstructCnt = " << A::moveConstructCnt << std::endl;
    std::cout << "------------------------------" << std::endl;

    A::copyConstructCnt = 0;
    A::moveConstructCnt = 0;
    // std::vector<A> vec1(3, 1);
    LTC::vector<A> vec1(3, 1);
    vec1.assign({1});
    std::cout << vec1.size() << " " << vec1.capacity() << std::endl;
    std::cout << "copyConstructCnt = " << A::copyConstructCnt << std::endl;
    std::cout << "moveConstructCnt = " << A::moveConstructCnt << std::endl;
}

// assign(size_t n, const _Tp &val)
void test15(void)
{
    // std::vector<A> vec(3, 1);
    LTC::vector<A> vec(3, 1);
    vec.assign(size_t(1), 2);
    std::cout << vec.size() << " " << vec.capacity() << std::endl;
    std::cout << "copyConstructCnt = " << A::copyConstructCnt << std::endl;
    std::cout << "moveConstructCnt = " << A::moveConstructCnt << std::endl;
    std::cout << "------------------------------" << std::endl;

    A::copyConstructCnt = 0;
    A::moveConstructCnt = 0;
    // std::vector<A> vec1(3, 1);
    LTC::vector<A> vec1(3, 1);
    vec1.assign(size_t(4), 2);
    std::cout << vec1.size() << " " << vec1.capacity() << std::endl;
    std::cout << "copyConstructCnt = " << A::copyConstructCnt << std::endl;
    std::cout << "moveConstructCnt = " << A::moveConstructCnt << std::endl;
}

void test16(void)
{
    // std::vector<A> vec = {1, 2, 3, 4, 5, 6};
    LTC::vector<A> vec = {1, 2, 3, 4, 5, 6};
    auto it0 = vec.erase(vec.begin());
    std::cout << vec.size() << " " << vec.capacity() << std::endl;
    std::cout << vec << std::endl;
    std::cout << *it0 << std::endl;
    std::cout << "copyConstructCnt = " << A::copyConstructCnt << std::endl;
    std::cout << "moveConstructCnt = " << A::moveConstructCnt << std::endl;
    std::cout << "------------------------------" << std::endl;

    A::copyConstructCnt = 0;
    A::moveConstructCnt = 0;
    // std::vector<A> vec1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    LTC::vector<A> vec1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto it1 = vec1.erase(vec1.begin() + 2, vec1.begin() + 6);
    std::cout << vec1.size() << " " << vec1.capacity() << std::endl;
    std::cout << vec1 << std::endl;
    std::cout << *it1 << std::endl;
    std::cout << "copyConstructCnt = " << A::copyConstructCnt << std::endl;
    std::cout << "moveConstructCnt = " << A::moveConstructCnt << std::endl;
    std::cout << "------------------------------" << std::endl;
}

int main(void)
{
    int choose = 100;
    std::cin >> choose;
    switch (choose)
    {
    case 0:
        test0();
        break;
    case 1:
        test1();
        break;
    case 2:
        test2();
        break;
    case 3:
        test3();
        break;
    case 4:
        test4();
        break;
    case 5:
        test5();
        break;
    case 6:
        test6();
        break;
    case 7:
        test7();
        break;
    case 8:
        test8();
        break;
    case 9:
        test9();
        break;
    case 10:
        test10();
        break;
    case 11:
        test11();
        break;
    case 12:
        test12();
        break;
    case 13:
        test13();
        break;
    case 14:
        test14();
        break;
    case 15:
        test15();
        break;
    case 16:
        test16();
        break;
    default:
        break;
    }

}
