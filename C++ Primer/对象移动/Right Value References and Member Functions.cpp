#include <iostream>
#include <vector>

void leading(void)
{
    std::string s0 = "ma", s1 = "baoguo";
    std::string s2;
    s0 + s1 = "wow!"; // 对右值赋值！
    s2 = s0 + s1;
    std::cout << s0 << std::endl;
}

// 如果希望在自己的类中阻止对右值赋值
// 即：希望左侧运算对象是一个左值（this 所指对象）
// 需要用到引用限定符

class Test
{
private:
    std::vector<int> data;
public:

    // 只能向非 const 的左值进行赋值
    Test &operator=(const Test &test) &
    {
        this->data = test.data;
        return *this;
    }

    // 一个成员函数可以同时由 const 限定符和 引用限定符 修饰
    // 注意：const 限定符在前
    void func(void) const &
    {}

    // 重载与引用函数
    // 与 const 限定符一致，引用限定符也可以区分函数的重载版本
    Test sort(void) && // 可用于可改变的右值
    {
        // 对于右值来说，可以直接对 data 排序
        // 因为他没有其他的用户
        std::sort(this->data.begin(), this->data.end());
        return *this;
    }

    Test sort(void) const & // 可用于所有类型的 Test 对象
    {
        // 对于一个左值，或者一个常对象来说
        // 不能直接改变其 data 对象
        // 因此，先拷贝一个副本，对副本进行排序
        Test copied = *this;
        std::sort(copied.data.begin(), copied.data.end());
        return copied;
        // return copied.sort(); // 死递归
        // return Test(*this).sort(); // 正确，Test(*this) 是一个匿名对象，作为右值使用
    }
};

// 返回一个左值
Test &getTest(void)
{
    static Test t;
    return t;
} 

// 返回一个右值
Test getVal(void)
{
    Test t;
    return t;
} 

void test0(void)
{
    Test t0, t1;
    t0 = t1; // 正确，左值可以给左值赋值
    t0 = getTest(); // 正确，同上
    t1 = getVal(); // 正确，右值可以给左值赋值
    // getVal() = t1; // 错误，不能给右值赋值
}

void test1(void)
{
    auto t0 = getTest().sort(); // 调用 Test::sort() const &
    auto t1 = getVal().sort(); // 调用 Test::sort() &&
}

// 如果定义两个及以上名字相同，且参数列表完全一致的成员函数
// 必须都加上引用限定符
class Foo
{
    std::vector<int> data;
public:
    Foo sort(void) &&;
    // Foo sort(void) const; // 错误，重载参数类型相同的两个成员函数需要它们同时具有或缺少引用限定符

    using cmp = bool(const int &, const int &);
    Foo sort(cmp*);       // 正确，参数列表不同
    Foo sort(cmp*) const; // 正确，两个版本均不包含引用限定符
};