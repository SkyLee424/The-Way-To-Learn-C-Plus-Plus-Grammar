#include <iostream>
class MyInt
{
    // 友元配合 << 运算符，可以实现自定义数据类型输出
    friend std::ostream &operator<<(std::ostream &cout, const MyInt &a);
    friend std::istream &operator>>(std::istream &cin, MyInt &a);
    int *num;

public:
    MyInt()
    {
        this->num = new int(0);
    }

    MyInt(int num)
    {
        this->num = new int(num);
    }

    MyInt(const MyInt &b)
    {
        num = new int(*b.num);
    }

    ~MyInt()
    {
        if (num)
        {
            delete num;
            num = nullptr;
        }
    }

    // 重载 '=' 运算符
    MyInt &operator=(const MyInt &b)
    {
        if (num)
            *num = *b.num;
        else
        {
            num = new int(*b.num);
        }
        return *this;
    }

    // 重载 '+' 运算符
    MyInt operator+(const MyInt &b)
    {
        MyInt res(*this->num + *b.num);
        return res;
    }

    // 重载 '-' 运算符
    MyInt operator-(const MyInt &b)
    {
        MyInt res = *this->num - *b.num;
        return res;
    }

    // 重载前置 ++ 运算符
    // 为了后续可以重载 << ，返回 MyInt
    // 为了实现链式编程，以引用的形式返回
    MyInt &operator++(void)
    {
        *this->num += 1;
        return *this;
    }

    // 重载后置 ++ 运算符
    // 由于后置 ++ 的特性，不能以引用的方式返回
    // 也就是说：((a++)++)++ 是不合法的
    // 因此，无法实现链式编程
    const MyInt operator++(int) // 为了区别前置 ++ 与后置 ++，加一个占位参数，实现重载
    // 注意：编译器会默认加了占位参数的重载函数为后置 ++ 运算符重载
    {
        const MyInt temp = *this->num; // 新建一个局部变量保存原来的 a
        *this->num += 1;
        return temp;
    }

    // 重载前置 -- 运算符
    MyInt &operator--(void)
    {
        *this->num -= 1;
        return *this;
    }

    // 重载后置 -- 运算符
    const MyInt operator--(int) // 占位参数
    {
        const MyInt temp = *this->num;
        *this->num -= 1;
        return temp;
    }

    // 重载 '+=' 运算符
    MyInt &operator+=(const MyInt &b)
    {
        *this->num += *b.num;
        return *this;
    }

    // 重载 '-=' 运算符
    MyInt &operator-=(const MyInt &b)
    {
        *this->num -= *b.num;
        return *this;
    }

    // 重载 '==' 运算符

    bool operator==(const MyInt &b)
    {
        if (*this->num == *b.num)
            return true;
        return false;
    }

    // 重载 '!=' 运算符

    bool operator!=(const MyInt &b)
    {
        if (*this->num != *b.num)
            return true;
        return false;
    }

    // 重载 '!' 运算符
    bool operator!()
    {
        if (!this->num)
            return true;
        return false;
    }
};






// << 运算符重载
// 无法在类内实现
// extern ostream cout;		/// Linked to standard output
// cout 的类型为 ostream，即标准输出流
// 由于全局只允许有一个 cout，因此，在传入 cout 时，加引用
// 为了实现链式编程（多次调用cout），需要返回 cout 的引用
std::ostream &operator<<(std::ostream &cout, const MyInt &a)
{
    std::cout << *a.num;
    return cout;
}

// >> 运算符重载
std::istream &operator>>(std::istream &cin, MyInt &a)
{
    int temp;
    std::cin >> temp;
    if (a.num == nullptr)
    {
        a.num = new int(temp);
    }
    else
        *a.num = temp;
    return cin;
}

class Print
{
public:
    void operator()(const std::string &s)
    {
        std::cout << s << std::endl;
    }
};

class Test
{
public:
    // 测试加（减）号运算符的重载
    void test0(void)
    {
        MyInt a, b;
        std::cin >> a >> b;
        // auto c = a.operator+(b);
        // auto d = operator+(a, b);
        std::cout << a << " + " << b << " = " << a + b << std::endl;
        std::cout << a << " - " << b << " = " << a - b << std::endl;
        std::cout << "(" << a << " += " << b << ") = " << (a += b) << std::endl;
        std::cout << "(" << a << " += " << b << ") = " << (a -= b) << std::endl;
    }

    // 测试递增（减）运算符的重载
    void test1(void)
    {
        MyInt a;
        std::cin >> a;
        std::cout << a++ << std::endl;
        std::cout << a << std::endl;
        std::cout << ++a << std::endl;
        std::cout << a << std::endl;
        ++(++(++a));
        std::cout << a << std::endl;
        --(--(--a));
        std::cout << a << std::endl;
        std::cout << --a << std::endl;
        std::cout << a << std::endl;
        std::cout << a-- << std::endl;
        std::cout << a << std::endl;
    }

    // 测试赋值运算符的重载以及拷贝构造函数
    void test2(void)
    {
        MyInt a;
        std::cin >> a;
        MyInt b = a; // 调用拷贝构造函数（因为在实例化对象）
        std::cout << "b = " << b << std::endl;
        MyInt c;
        c = a; // 调用重载'='运算符（因为在赋值）
        std::cout << "c = " << c << std::endl;
        a = b = c; // 连续赋值操作（链式编程）
        std::cout << "a = " << a << ", b = " << b << ", c = " << c << std::endl;
    }
    // 测试关系运算符的重载
    void test3(void)
    {
        MyInt a, b;
        std::cin >> a >> b;
        if (a == b)
        {
            std::cout << a << " == " << b << std::endl;
        }
        else
            std::cout << a << " != " << b << std::endl;
        if (a != b)
        {
            std::cout << a << " != " << b << std::endl;
        }
        else
            std::cout << a << " == " << b << std::endl;
        // if (!a)
        // {
        //     std::cout << "!" << a << std::endl;
        // }
        // else
        //     std::cout << a << std::endl;
    }
    void test4(void)
    {
        std::string s;
        std::cin >> s;
        Print print; // 实例化一个对象
        print(s); 
        Print()(s); // 匿名对象调用
    }
};


int main(void)
{
    Test test;
    int opinion;
    std::cin >> opinion;
    switch (opinion)
    {
    case 0:
        test.test0();
        break;

    case 1:
        test.test1();
        break;

    case 2:
        test.test2();
        break;

    case 3:
        test.test3();
        break;

    case 4:
        test.test4();
        break;
    }
}
