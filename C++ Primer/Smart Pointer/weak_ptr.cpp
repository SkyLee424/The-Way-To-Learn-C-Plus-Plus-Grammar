#include <iostream>
#include <vector>

// weak_ptr是用来解决shared_ptr相互引用时的死锁问题，
// 如果说两个shared_ptr相互引用，那么这两个指针的引用计数永远不可能下降为 0 ,资源永远不会释放。

void test0(void)
{
    auto sp = std::make_shared<std::string>("114514");

    std::weak_ptr<std::string> wp0;
    std::weak_ptr<std::string> wp1(sp); // 拷贝

    wp0 = wp1; // 赋值，可以是 sp，也可以是 wp

    std::cout << sp.use_count() << std::endl;  // 1
    std::cout << wp0.use_count() << std::endl; // 1，与其绑定的 sp 的 use_count 一致
}

void test1(void)
{
    std::shared_ptr<int> sp0 = std::make_shared<int>(1);

    std::weak_ptr<int> wp0;

    // 等价于 wp0.use_count() == 0
    if (wp0.expired())
    {
        std::cout << "wp0 未绑定任何 shared_ptr" << std::endl;
    }

    wp0 = sp0;
    // 如果 expired，返回空的 sp
    // 否则返回其绑定的 sp
    if (std::shared_ptr<int> sp1 = wp0.lock())
    {
        std::cout << *sp1 << std::endl;
    }
}

class B;
class C;
class A
{ 
public:
    std::shared_ptr<B> _pb;
    std::shared_ptr<C> _pc;
    ~A()
    {
        std::cout << "A 类析构函数的调用" << std::endl;
    }
};
class B
{ 
public:
    std::shared_ptr<A> _pa;
    ~B()
    {
        std::cout << "B 类析构函数的调用" << std::endl;
    }
};
class C
{
public:
    std::weak_ptr<A> _pa;
    ~C()
    {
        std::cout << "C 类析构函数的调用" << std::endl;   
    }
};

// 验证死锁问题
void test2(void)
{
    std::shared_ptr<A> pa(new A());
    std::shared_ptr<B> pb(new B());
    pa->_pb = pb; // pb.use_count() == 2
    pb->_pa = pa; // pa.use_count() == 2
    // pa、pb 相互引用
    // 离开 test2 时，计数值均为 1
    // 因此，不会调用 pa pb 的析构函数
    // 因此，资源永远不会被释放
}

// 解决死锁问题：weak_ptr
void test3(void)
{
    std::shared_ptr<A> pa(new A());
    std::shared_ptr<C> pc(new C());
    pa->_pc = pc; // pc.use_count() == 2
    pc->_pa = pa; // pa.use_count() == 1，因为用 sp 给 wp 赋值不会增加 sp 的计数
    std::cout << pa.use_count() << std::endl;
    std::cout << pc.use_count() << std::endl;
    // 离开 test3
    // pa 计数值 -1
    // 由于 pa 的计数值为 1
    // 因此，-1 后，pa 将被销毁
    // 而 pa 的销毁会导致 pc 的计数值 -1，即变成 1
    // 离开 test3 又会使 pc 的计数值 -1，即变成 0
    // 因此，pc 也会被销毁
}

// 不能直接通过 weak_ptr 访问对象的成员
void test4(void)
{
    auto sp = std::make_shared<std::vector<int>>();
    std::weak_ptr<std::vector<int>> wp = sp;
    // wp->begin() // 错误
    wp.lock()->begin(); // 正确，将 wp 转换为 sp，再访问成员属性
}


int main(void)
{
    test3();
}