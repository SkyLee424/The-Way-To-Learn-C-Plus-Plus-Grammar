#include<iostream>
#include<vector>
// 在自定义对象中，添加移动构造函数，可以避免扩容带来的拷贝问题

int cnt = 0;

class Test
{
public:
    int val;
    int *p;
    Test()
        : val(0), p(nullptr) {}
    Test(int val)
    {
        this->val = val;
        this->p = new int(val);
    }
    Test(const Test &t)
    {
        ++cnt;
        std::cout << "普通拷贝构造的调用" << std::endl;
        this->val = t.val;
        this->p = new int(t.val);
    }
    Test(Test &&t) noexcept
    {
        std::cout << "移动构造的调用" << std::endl;
        this->val = t.val;
        this->p = t.p;
        t.p = nullptr;
    }
    Test &operator=(const Test &t)
    {
        std::cout << "普通赋值运算符的调用" << std::endl;
        this->val = t.val;
        if (this->p)
        {
            *this->p = t.val;
        }
        else
            this->p = new int(t.val);
        return *this;
    }
    Test &operator=(Test &&t)
    {
        std::cout << "移动赋值运算符的调用" << std::endl;
        if(this != &t)
        {
            this->val = t.val;
            if (this->p)
            {
                delete this->p;
                this->p = nullptr;
            }
            this->p = t.p;
            t.p = nullptr;
        }
        return *this;
    }
    ~Test()
    {
        std::cout << "析构函数的调用" << std::endl;
        if (this->p)
        {
            delete this->p;
            this->p = nullptr;
        }
    }
};

int main(void)
{
    std::vector<Test> vec;
    Test t(1);
    for (int i = 0; i < 9; ++i)
    {
        vec.push_back(t);
    }
    std::cout << cnt << std::endl;            // 9（只有在插入新元素时才拷贝，扩容不拷贝）
    std::cout << vec.capacity() << std::endl; // 16
    
    vec.clear();
    vec.resize(0);
    vec.shrink_to_fit();
    cnt = 0;
    std::cout << "-----------------------------" << std::endl;

    for (int i = 0; i < 9; ++i)
    {
        vec.emplace_back(1);
    }
    std::cout << cnt << std::endl;            // 0
    std::cout << vec.capacity() << std::endl; // 16
}