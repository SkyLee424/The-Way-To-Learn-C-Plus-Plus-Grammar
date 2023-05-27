#include <iostream>

/**
 * @brief unique_ptr 不支持拷贝与赋值
 * 但是可以通过移动操作进行拷贝与赋值
 * 
 */
void test0(void)
{
    // std::unique_ptr<std::string> up0 = new std::string("NiuMa"); // 错误，需采用直接初始化方式
    std::unique_ptr<std::string> up0(new std::string("NiuMa"));

    // std::unique_ptr<std::string> up1 = up0; // 错误，不支持拷贝
    // 正确，将 up0 转换为右值引用，相当于将 up0 对 "NiuMa" 的控制权转移给 up1
    std::unique_ptr<std::string> up1 = std::move(up0);
    std::cout << *up1 << std::endl;
    // std::cout << *up0 << std::endl; // 错误，原因不表

    std::unique_ptr<std::string> up2;
    // // up2 = up0; // 错误，不支持赋值
    // // 正确，将 up1 转换为右值引用，相当于将 up1 对 "NiuMa" 的控制权转移给 up2
    up2 = std::move(up1);
    std::cout << *up2 << std::endl;
    // std::cout << *up1 << std::endl; // 错误，原因不表
}

/**
 * @brief 虽然不可以直接拷贝或赋值
 * 但是可以通过 reset、release 结合使用实现拷贝与赋值
 * 也就是 test0 中的移动构造和移动赋值的内部实现
 * 
 */
void test1(void)
{
    std::unique_ptr<std::string> up0(new std::string("IKun"));
    // release()
    // up放弃对指针的控制权，返回指针
    // 并将 up 置空
    auto p = up0.release();
    std::unique_ptr<std::string> up1(p); // 实现拷贝
    std::cout << *up1 << std::endl;
    // std::cout << *up0 << std::endl; // 错误

    // reset(): 释放 up 所指的对象，并将 up 置空
    // reset(p): 释放 up 所指的对象，并令 up 指向 p
    std::unique_ptr<std::string> up2;
    up2.reset(up1.release());
    std::cout << *up2 << std::endl;
    // std::cout << *up1 << std::endl; // 错误

    p = up2.release(); // 正确，但如果不将 p 赋与一个智能指针，就要记得自己释放
    delete p;
}

void del(int *p)
{
    // 此处为空实现
}

/**
 * @brief 向 unique_ptr 传递删除器
 * 如果要自己制定删除器，需要在实例化 unique_ptr 时制定删除器的类型
 * unique_ptr<objType, delType> p(obj, fcn);
 * 
 */
void test2(void)
{
    int a = 42; // a 是在栈区分配的
    // std::unique_ptr<int> p(&a); // 错误
    std::unique_ptr<int, decltype(del)*> p(&a, del);
    std::cout << *p << std::endl;
}

void test3(void)
{
    int *p = new int(42);
    std::unique_ptr<int> up(p);
    {
        std::unique_ptr<int> up1(up.get());
    } // 老问题了，up1 离开此作用域，会被释放
    std::cout << *up << std::endl; // 未定义行为
    // 离开 test3
    // 第二次释放 p，未定义行为
}

int main(void)
{
    test3();
}