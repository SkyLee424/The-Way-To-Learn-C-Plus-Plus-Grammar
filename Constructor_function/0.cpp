#include<iostream>
class Test
{
    static int st1;
public:
    int val;
    int* ptr;
    static int st0;
    Test()
        :val(0), ptr(nullptr){}
    Test(int val)
        :val(val), ptr(nullptr) {}
    Test(int val, int* ptr)
        :val(val), ptr(ptr){}
    Test(const Test& t)
    {
        val = t.val;
        ptr = new int(*t.ptr);
    }
    ~Test()
    {
        if (ptr)
        {
            delete ptr;
            ptr = nullptr; // 防止野指针
        }
    }
    static void init(void)
    {
        st0 = 1;
        st1 = 2;
    }
};
int Test::st0 = 1;
int Test::st1 = 1;
Test func(void)
{
    Test t0;
    std::cout << (int*)&t0 << std::endl;
    return t0;
}
int main(void)
{
    Test t1 = func();
    std::cout << (int*)&t1 << std::endl;
}