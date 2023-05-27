#include <iostream>
#include <vector>

int cnt = 0;

class String
{
    char *s;
    int _size;
    void init(void)
    {
        if (this->s)
        {
            delete this->s;
            this->s = nullptr;
        }
        this->s = new char[_size];
    }

public:
    String()
    {
        this->s = nullptr;
        _size = 0;
        init();
    }
    String(const char *s)
    {
        this->s = nullptr;
        int len = 0;
        while (s[len++] != '\0')
            ;
        _size = len;
        init();
        for (int i = 0; i < len; ++i)
            this->s[i] = s[i];
    }
    String(const String &S)
    {
        this->s = nullptr;
        this->_size = S.size();
        init();
        const char *s = S.getS();
        for (int i = 0; i < _size; ++i)
            this->s[i] = s[i];
        std::cout << "普通拷贝构造" << std::endl;
        ++cnt;
    }
    // 移动拷贝构造函数
    // 第一个参数为右值引用
    // noexpect：声明移动构造函数不抛出异常
    String(String &&s) noexcept
    {
        this->_size = s._size;
        this->s = s.s;
        // 使在对 s 进行析构时，是安全的
        // 也就是说，源对象的析构不会影响到新对象
        // 同时保证移动后的源对象可以安全使用而不依赖当前值
        s.s = nullptr;
        s._size = 0;
        // std::cout << "移动构造函数" << std::endl;
    }
    ~String()
    {
        delete s;
    }

    String &operator=(const String &S)
    {
        if (this != &S)
        {
            this->_size = S.size();
            init();
            const char *s = S.getS();
            for (int i = 0; i < _size; ++i)
                this->s[i] = s[i];
            std::cout << "普通赋值" << std::endl;
            // ++cnt;
        }
        return *this;
    }

    String &operator=(String &&s) noexcept
    {
        // 注意：需要判断是否为自赋值
        // 因为传进来的参数可能是通过 std::move 得来的
        // 我们不能在使用右侧运算对象的值之前就释放左侧运算对象的值（可能是相同的资源）
        if (this != &s)
        {
            if (this->s)
            {
                delete this->s;
                this->s = nullptr;
            }
            this->_size = s._size;
            this->s = s.s;
            s.s = nullptr; // 将 s 置于可析构状态
            s._size = 0;
            std::cout << "移动赋值" << std::endl;
        }
        return *this;
    }

    const char *getS(void) const
    {
        // 按理说为了保证健壮性，需要加上空指针判断
        // 但是为了凸显 “调用 std::move 就意味着承诺：除了对源对象赋值或者销毁，我们将不再使用它”
        // 删掉了空指针判断
        // if(!this->s)
        //     return "";
        return this->s;
    }
    const int size(void) const
    {
        return _size;
    }
};

// A 类有默认的移动构造与移动赋值
class A
{
public:
    int val;    
};

// B 类没有默认的移动构造与移动赋值
class B
{
    int val;
    // B(B &&b){}
    // ~B(){}
public:
    B() = default;
    B(const B &b)
    {
        val = b.val;
    }
    B(B &&b)
    {
        val = b.val;
    }
    // 因为类 B 没有定义自己的移动构造函数，所以编译器会生成一个默认的移动构造函数，
    // 该构造函数将直接调用 B 的拷贝构造函数，而不是移动构造函数。
};

// B 类定义了自己的拷贝构造函数，但未定义自己的移动构造函数
class hasB
{
public:
    int a;
    B b;
    hasB() = default;
    hasB(hasB &&) = default;
    hasB& operator=(const hasB &b)
    {

    }
    hasB& operator=(hasB &&b)
    {
        
    }
};

struct Test
{
    static void test0(void)
    {
        String s = "test";
        std::vector<String> vec;
        for (int i = 0; i < 17; ++i)
        {
            // 如果 String 类没有移动构造函数
            // 在 push_back 和 扩容 都要拷贝
            // vec.push_back("t");
            vec.emplace_back("t"); // 仅在扩容时进行拷贝操作
            std::cout << vec.capacity() << " ";
        }
        std::cout << "\n" << cnt << std::endl;
    }

    static void test1(void)
    {
        String s = "t";
        String s1;
        s = std::move(s); // 自赋值

        s1 = std::move(s); // std::move 的返回值做右值
        std::cout << s.size() << std::endl; // 0
        std::cout << s1.getS() << std::endl;

        s1 = "haha"; // 字面值
        std::cout << s1.getS() << std::endl;

        // 错误，此时 s 被当做右值，不应该进行除了赋值和销毁以外的任何操作
        // std::cout << s.getS() << std::endl; 

        // 调用普通赋值（因为 = 运算符右侧的对象是左值）
        // 注意：在使用 s 前，需保证 s 不被当做右值看待
        // 因为，在调用 std::move() 后，就意味着承诺：除了对源对象赋值或者销毁，我们将不再使用它”
        s = "reAssign"; // 重新赋值（因为之前 s 被当做右值看待，已经无法再访问了，只能赋值或销毁）
        s1 = s;
        std::cout << s.getS() << std::endl;
    }

    static void test2(void)
    {
        hasB hb;
        hasB hb2 = std::move(hb);
        hb2 = hb;
        hb2 = std::move(hb);
    }
};

int main(void)
{
    // Test::test0();
    // Test::test1();
    Test::test2();
}