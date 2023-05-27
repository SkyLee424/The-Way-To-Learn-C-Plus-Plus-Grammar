#include <iostream>
#include "shared_ptr.hpp"

struct MyStruct
{
    int a;
    int *pa;
    MyStruct(int a)
        : a(a), pa(new int(a))
    {
        std::cout << "MyStruct 转换构造函数的调用" << std::endl;
    }

    MyStruct(MyStruct &&m)
        : a(m.a), pa(m.pa)
    {
        m.pa = nullptr;
        std::cout << "移动构造" << std::endl;
    }

    ~MyStruct()
    {
        delete pa;
        pa = nullptr;
        std::cout << "MyStruct 析构函数的调用" << std::endl;
    }
};

void test0(void)
{
    shared_ptr<int> s0;
    std::cout << s0.use_count() << std::endl;
    shared_ptr<int> s1 = make_shared<int>(10); // 拷贝构造
    shared_ptr<int> s2;                        // 默认构造
    s2 = s1;                                   // 赋值运算符
    shared_ptr s3(s1);                         // 直接拷贝构造
    std::cout << s1.use_count() << std::endl;
    s3 = s3; // 自赋值
    std::cout << s3.use_count() << std::endl;
    std::cout << "------------" << std::endl;
    std::cout << *s1 << std::endl;
    std::cout << *s2 << std::endl;
    std::cout << *s3 << std::endl;
    std::cout << "------------" << std::endl;
    std::cout << s1.get() << std::endl;
    std::cout << s2.get() << std::endl;
    std::cout << s3.get() << std::endl;
}

void test1(void)
{
    std::shared_ptr<int> s0;
    std::cout << s0.use_count() << std::endl;
    std::shared_ptr<int> s1 = std::make_shared<int>(10);
    std::shared_ptr<int> s2;
    s2 = s1;
    std::shared_ptr<int> s3(s1);
    std::cout << s1.use_count() << std::endl;
    s3 = s3;
    std::cout << s3.use_count() << std::endl;
    std::cout << "------------" << std::endl;
    std::cout << *s1 << std::endl;
    std::cout << *s2 << std::endl;
    std::cout << *s3 << std::endl;
    std::cout << "------------" << std::endl;
    std::cout << s1.get() << std::endl;
    std::cout << s2.get() << std::endl;
    std::cout << s3.get() << std::endl;
}

void test2(void)
{
    shared_ptr<MyStruct> s0;
    // 我们实现的 shared_ptr 两个都报错的原因：
    // 对于这两个语句而言，实际上是等价的
    // 因为我们的 make_shared 是没有直接构造一个对象的能力的（就像 emplace 一样）
    // 都是创建一个临时右值对象，然后将这个临时对象 移动 过去
    // 而默认的移动构造函数对于指针类型的成员是浅拷贝的
    // shared_ptr<MyStruct> s1 = make_shared<MyStruct>(MyStruct(0)); // SIGBAT
    shared_ptr<MyStruct> s1 = make_shared<MyStruct>(0); // SIGBAT
    shared_ptr<MyStruct> s2;
    s2 = s1;
    auto s3(s1);
    std::cout << s1.use_count() << std::endl;
    s3 = s3;
    std::cout << s3.use_count() << std::endl;
    std::cout << "------------" << std::endl;
    std::cout << *((*s1).pa) << std::endl;
    std::cout << *((*s2).pa) << std::endl;
    std::cout << *((*s3).pa) << std::endl;
    std::cout << "------------" << std::endl;
    std::cout << s1.get() << std::endl;
    std::cout << s2.get() << std::endl;
    std::cout << s3.get() << std::endl;
}

void test3(void)
{
    std::shared_ptr<MyStruct> s0;
    // SIGBRT 原因：
    // 先创建一个临时对象
    // 再调用移动构造，将该临时对象移动到 s1
    // 而 MyStruct 合成的移动构造函数对于指针类型的成员是浅拷贝的
    // 在这一行结束后，临时对象 MyStruct(0) 被释放，对应的 pa 也被释放
    // 而在 test3 结束后，shared_ptr 的引用计数为 0，会再次释放 pa
    // 导致错误
    // 当然，如果我们自己实现一个 移动构造函数，就可以避免这个问题

    // 4.21 更新:（之前的分析有点问题）
    // 其实就是函数匹配优先级的问题
    // make_shared 是使用用户传入的参数，直接实例化一个对象
    // std::shared_ptr<MyStruct> s1 = std::make_shared<MyStruct>(MyStruct(0));
    // 传入的参数是 MyStruct(0)，是一个右值，与 移动构造函数 参数类型精确匹配
    // 优先使用 移动构造函数
    // 而合成的移动构造函数对于指针类型的成员是浅拷贝的
    // 在释放临时对象 MyStruct(0) 时，释放一次 pa
    // test3 结束时，再释放异常一次 pa，导数 SIGBAT
    // 调用 MyStruct::MyStruct(int a) 是因为要实例化 MyStruct(0)
    std::shared_ptr<MyStruct> s1 = std::make_shared<MyStruct>(MyStruct(0)); // SIGBRT
    // OK 原因：
    // std::shared_ptr<MyStruct> s1 = std::make_shared<MyStruct>(0)
    // 传入参数为 0
    // 与 MyStruct::MyStruct(int a); 精确匹配
    // 调用 MyStruct::MyStruct(int a);
    // 直接构造好对象，不用移动对象了（也就是调用默认合成的移动构造函数，导致 SIGBAT 的根本原因）
    // std::shared_ptr<MyStruct> s1 = std::make_shared<MyStruct>(0); // OK
    std::shared_ptr<MyStruct> s2;
    s2 = s1;
    auto s3(s1);
    std::cout << s1.use_count() << std::endl;
    s3 = s3;
    std::cout << s3.use_count() << std::endl;
    std::cout << "------------" << std::endl;
    std::cout << *((*s1).pa) << std::endl;
    std::cout << *((*s2).pa) << std::endl;
    std::cout << *((*s3).pa) << std::endl;
    std::cout << "------------" << std::endl;
    std::cout << s1.get() << std::endl;
    std::cout << s2.get() << std::endl;
    std::cout << s3.get() << std::endl;
}

int main(void)
{
    // test0();
    // test1();
    // test2();
    test3();
    // auto t = std::move(MyStruct(0));
}