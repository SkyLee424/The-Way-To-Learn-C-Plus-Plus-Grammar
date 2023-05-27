#include <iostream>

/**
 * 智能指针陷阱
 * 1. 不使用 **相同的** 内置指针初始化 **多个** shared_ptr
 * 2. 不 delete get() 返回的指针
 * 3. 如果使用 get() 返回的指针，切记：最后一个对应的智能指针被销毁时，get() 返回的指针就无效了
 * 4. 不要用 get() 返回的指针初始化或 reset 一个智能指针
 * 5. 如果使用的智能指针管理的资源不是 new 分配的，记住，传给它一个 **删除器**
 */

struct A
{
    int val;
    A(int val)
        : val(val)
    {
        std::cout << "Val == " << val << " 调用构造函数" << std::endl;
    }
    ~A()
    {
        std::cout << "调用析构函数" << std::endl;
    }
};

void test0(void)
{
    A a(10);
    auto p0 = std::make_shared<A>(a);
    auto p1 = p0;
    auto p2 = std::make_shared<A>(a);
    std::cout << p0.get() << std::endl; // 0x6000015791b8
    std::cout << p1.get() << std::endl; // 0x6000015791b8
    std::cout << p2.get() << std::endl; // 0x6000015791d8
    auto p = p0.get();
    // delete p;                     // 报错，malloc: *** error for object 0x6000015791b8: pointer being freed was not allocated
    std::cout << &a << std::endl; // 0x7ff7b6690b18
    A *_p = new A(a);
    std::cout << _p << std::endl; // 0x600001dc0020
    std::cout << "---" << std::endl;
    delete _p;
}


// 不使用 **相同的** 内置指针初始化 **多个** shared_ptr
void test1(void)
{
    A *a = new A(10);
    std::shared_ptr<A> p0(a);
    std::shared_ptr<A> p1(a); // 危险！独立创建的 p0、p1 指向同一块内存！
    std::cout << a << std::endl;
    std::cout << p0.get() << std::endl;
    std::cout << p1.get() << std::endl;
    std::cout << p0.use_count() << std::endl; // 1
    std::cout << p1.use_count() << std::endl; // 1
    // 离开 test1
    // 由于 p1 的 use_count() == 1，因此，会释放其指向的内存，假设为 0x11
    // 由于 p0 的 use_count() == 1，因此，会释放其指向的内存，与 p1 一致，为 0x11
    // 可以发现，此时尝试释放一个已经释放过的指针
    // 因此，报错
    // delete a; // 错误，释放一个已经释放过的指针
}

void func(std::shared_ptr<A> p)
{
    std::cout << p.use_count() << std::endl;
}

void test2(void)
{
    // case 1
    A *a = new A(10);
    std::shared_ptr<A> p(a);
    func(p);
    A temp0 = *a; // 正确，p.use_count() == 1，a 没有被释放，可以解引用
    std::cout << temp0.val << std::endl;

    // case 2
    A *b = new A(10);
    func(std::shared_ptr<A>(b));
    // 错误，传入一个临时对象给 func
    // 离开 func 时，由于该临时对象的 use_count() == 1
    // 因此，其指向的对象（b）将被释放
    A temp1 = *b;
    std::cout << temp1.val << std::endl; // 未定义行为
    // delete b; // 错误，在调用完 func 后，b 已经被释放 
}


// 如果使用 get() 返回的指针，切记：最后一个对应的智能指针被销毁时，get() 返回的指针就无效了
void test3(void)
{
    std::shared_ptr<A> p(new A(10));
    A *q = p.get(); // 正确，但千万不要释放 q ！
    // 新的程序块
    {
        std::shared_ptr<A> p1(q);     // p1.use_count() == 1
    }                                 // 离开程序块，p1 指向的内存被释放！
    std::cout << q->val << std::endl; // 错误，q 已经被释放
    std::cout << "---" << std::endl;
    // 离开 test3
    // p.use_count() == 1
    // 因此，还要释放一次内存
    // 由于 p 指向的内存已经被释放
    // 因此，释放一个已经释放过的指针，程序崩溃
}

// 使用 reset 将一个新的指针赋与 shared_ptr
void test4(void)
{
    std::string s = "MaBaoGuo";
    std::shared_ptr<std::string> p0 = std::make_shared<std::string>(s);
    std::shared_ptr<std::string> p1 = p0;
    // 如果 p0 并非唯一用户
    // 需要制作一份新的拷贝
    // 否则，会改变源对象
    if(!p0.unique()) 
    {
        p0.reset(new std::string(*p0)); // p0 指向一块新的内存
    }
    *p0 += "NiuBi";
    std::cout << *p0 << std::endl; // MaBaoGuoNiuBi
    std::cout << *p1 << std::endl; // MaBaoGuo
}


// 不要用 get() 返回的指针初始化或 reset 一个智能指针
void test5(void)
{
    std::shared_ptr<A> p(new A(10));
    func(std::shared_ptr<A>(p)); // 正确
    func(std::shared_ptr<A>(p.get())); // 错误
}

void test6(void)
{
    int a = 42;
    // &a 并不是 new 分配来的内存
    // 所以，当 p 被销毁后，尝试释放 &a 所指的内存空间的行为是错误的
    // std::shared_ptr<int> p(&a); 

    std::shared_ptr<int> p1(&a, [](int *p)
    {
        // 删除器
        // 此处为空实现
    });
}

int main(void)
{
    test6();
}