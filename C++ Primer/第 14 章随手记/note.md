## 14.1

1. 除了重载的函数调用运算符 operator() 外，其它重载的运算符均 **不能** 有默认实参
2. 如果某个重载运算符是一个成员函数，那么，它的第一个参数将隐式地绑定到 this 上，因此，它的显式参数数量总要少一个
3. 对于一个运算符函数来说，它或者是类的成员，或者 **至少** 含有 **一个** 类类型成员的参数，因此，不能重载作用与内置类型的运算符：   
   
    ```cpp
    int operator+(int, int); // 错误
    ```

4. 不能重载的运算符：```.``` ```.*```（被用于正则表达式） ```::``` ```? :```
6. 不建议重载 ```&&``` ```||```，因为无法保留 短路求值 的规则，两个对象总会 **都** 被求值

    > 短路求值是指 ```&&``` 和 ```||``` 都是先求左侧运算对象的值，当左侧运算对象的值无法确定表达式的结果时才计算右侧运算对象的值

7. 不建议重载 ```&（取地址）``` ```,``` 运算符，因为这两个运算符已经有了内置的含义，如果重载，可能使类的用户不适应
8. 通常，我们希望在含有 *混合类型* 的表达式中使用 **对称性运算符**

    例如：
    ```cpp
    std::string a = "114";
    auto c = a + "514"; // 正确
    auto d = "514" + a; // 如果 string 的 operator+ 是成员函数，此句错误
    ```
    
    对于这个例子，如果 string 类的 operator+ 是一个成员函数，那么，```auto d = "514" + a;``` 这句话就是错误的了，因为 const char* 是一种内置类型，根本就没有一个成员函数，也没有一个 operator+ 可以实现 const char* + std::string，因此，此句错误

    可以发现，如果将 operator 定义成一个普通函数，这个问题将得到解决

    因此，不是所有情况都可以将一个重载运算符函数定义成一个成员函数，即使这样做没有语法问题


## 14.2

1. 自定义 IO 运算符，必须定义为非成员函数，一般来说，可能需要访问类的私有成员，因此，一般将 IO 运算符 声明为友元
2. 在实现 *输入* 运算符时，需考虑发生的 IO 错误：

    ```cpp
    class A;
    std::istream& operator>>(std::istream &is, A &a);
    std::ostream& operator<<(std::ostream &os, const A &a);
    
    class A
    {
        friend std::istream& operator>>(std::istream &is, A &a);
        friend std::ostream& operator<<(std::ostream &os, const A &a);
        int val;
    public:
        A()
            :val(114514){}
    };
    
    std::istream& operator>>(std::istream &is, A &a)
    {
    
        is >> a.val;
        if(!is)
        {
            // 发生 IO 错误，将对象置于默认的状态
            a = A();
        }
        return is;
    }
    
    std::ostream& operator<<(std::ostream &os, const A &a)
    {
        os << a.val;
        return os;
    }
    
    void test(void)
    {
        A a;
        std::cin >> a;
        std::cout << a << std::endl; // 如果发生 IO 错误，输出 114514
    }
    ```
    
    而实现 *输出* 运算符不需要处理 IO 错误

    发生 IO 错误可能的原因：

    * 读入的数据与实际数据不符
    * 遇到文件末尾，或者其它流的错误

    此外，我们是在读取完所有数据才进行一次性检查，因为我们并不在乎是哪里出现异常，只要有一个地方发生异常，对象的数据就是未定义的了，这时，**将对象置于正确的状态显得极其重要！**

3. 如果类同时定义了算术运算符和相关的复合赋值运算符，则通常应该使用复合赋值运算符来实现算术运算符：

    ```cpp
    class A
    {
        int val;
        std::string data;
    public:
        // 不允许对一个右值使用 +=
        A operator+=(const A &a) &
        {
            val += a.val;
            data += a.data;
        }
    };
    
    A operator+(const A &a, const A &b)
    {
        A res = a;
        res += b;
        return res;
    }
    ```

    这样可以避免编写重复代码，并且可能可以省去友元声明

4. 如果存在 **唯一一种** 逻辑可靠的 < 定义，才考虑为该类定义 operator<；特别地，如果 obj0 != obj1，满足 obj0 < obj1 || obj0 > obj1
5. 倾向于将复合赋值运算符定义在类内
6. 最好定义 operator[] 的常量版本，并返回 const 引用，以确保不会给返回对象赋值
7. 倾向于将递增（减）运算符定义在类内
8. 利用一个额外的 int 类型的形参来区分前置递增（减）后后置递增（减）。其中，有 int 形参的是后置递增（减）
    
    ```cpp
    obj0.operator++(); // 前置递增
    obj0.operator++(0); // 后置递增
    ```

9. operator-> **必须** 是类的成员，而 operator* **通常** 是类的成员（原因自己想）

### 箭头运算符

对于形如 point->mem 的表达式来说，point 必须是指向类对象的指针或者是重载了 operator-> 的类的对象。根据 point 类型的不同，point->mem 分别等价于：

```cpp
(*point).mem  //point是一个指针
point.operator()->mem //point是类的一个对象
```

point->mem 的执行过程如下：

1. 如果返回类型是 **指针**，则内置箭头操作符可用于该指针，编译器 **对该指针解引用** 并 **从结果对象获取指定成员**。如果被指向的类型没有定义那个成员，则编译器产生一个错误。
2. 如果返回类型是 **重载了 operator-> 的类对象**，则调用该返回值的 operator->，重复第 2 步，直到返回类型为指针，执行第一步

也就是说：重载箭头操作符必须返回 **指向类类型的指针**，或者返回 **定义了自己的箭头操作符的类类型对象**

例如：

```cpp
struct A
{
    int val = 1;
};

class B
{
    A a;
public:
    int val = 2;
    A* operator->(void) // 返回指针类型
    {
        return &a;
    }
};

class C
{
    B b;
public:
    int val = 3;
    B operator->(void) // 返回定义了 operator-> 的对象
    {
        return b;
    }
};

void test(void)
{
    C c;
    C *pc = new C;
    // 常规使用
    std::cout << pc->val << std::endl; // 3

    // c 是一个非指针类型的变量
    // 但却可以使用箭头访问成员！
    // 这是因为其定义了 operator->
    // C 类的 operator-> 返回一个 B 类对象
    // 由于 B 类也重载了 operator-> 
    // 重复这一步骤（继续调用 B 类的 operator->），直到返回一个指针类型的变量为止，因为此时可以使用内置的 operator-> 了
    // 此时，由于 B 类的 operator-> 返回 A 类的指针
    // 可以使用内置的 operator-> 了，结束调用
    // 对应的 val 就是 A 类的 val，默认为 1
    std::cout << c->val << std::endl; // 1
} 
```

## 14.8

### 标准库函数对象

```cpp
bool cmp(const std::string *s0, const std::string *s1)
{
    return *s0 < *s1;
}

void test(void)
{
    std::vector<int> vec;
    vec.reserve(10);
    for(int i = 0; i < 10; ++i)
        vec.emplace_back(i * 2);
    
    int base = 2;

    // isNone: 如果 vec 中所有的数据都能被 base 整除，则为 true
    // none_of: 如果在给定范围内的元素，没有一个满足 pred 这个一元谓词，返回 true 
    // modulus<T>()(a, b): 返回 a % b

    // 如果 vec的某一个元素 % base != 0，则 std::bind(std::modulus<int>(), _1, base) == true
    // isNone = false
    bool isNone = std::none_of(vec.begin(), vec.end(), std::bind(std::modulus<int>(), _1, base));
    if(isNone)
    {
        std::cout << "均是 " << base << " 的整数倍" << std::endl;
    }
    else std::cout << "某一元素不是 " << base << " 的整数倍" << std::endl;
}
```

### 可调用对象与 std::function

```cpp
#include <iostream>
#include <functional>
#include <unordered_map>

int add(int a, int b)
{
    return a + b;
}

struct multiply
{
    int operator()(int a, int b)
    {
        return a * b;
    }
};

void test0(void)
{
    using pf = int(*)(int, int);
    std::unordered_map<std::string, pf> oper;
    auto subtract = [](int a, int b) -> int
    {
        return a - b;
    };
    oper.insert({"+", add});
    oper.insert({"-", subtract}); // 错误？
    // oper.insert({"*", multiply()}); // 错误，muliply 不是函数指针
    std::cout << oper["-"](10, 2) << std::endl;
}

void test1(void)
{
    std::function<int(int, int)> f1 = add;
    auto subtract = [](int a, int b) -> int
    {
        return a - b;
    };
    std::function<int(int, int)> f2 = subtract;
    std::function<int(int, int)> f3 = multiply();
    std::cout << f1(1, 2) << std::endl;
    std::cout << f2(1, 2) << std::endl;
    std::cout << f3(1, 2) << std::endl;
}

// 利用 std::function 解决上述问题
void test2(void)
{
    std::unordered_map<std::string, std::function<int(int, int)>> oper;
    auto subtract = [](int a, int b) -> int
    {
        return a - b;
    };
    oper.insert({"+", add}); // 函数指针
    oper.insert({"-", subtract});  // 匿名 lambda
    oper.insert({"*", multiply()}); // 函数对象
    oper.insert({"/", std::divides()}); // 内置函数对象
}

// 重载函数与 std::function
struct Test
{
    int val;   
    /*explicit*/ Test(int val)
        :val(val){}
};

Test add(Test a, Test b)
{
    return a.val + b.val; // 隐式类型转换
}

void test3(void)
{
    std::unordered_map<std::string, std::function<int(int, int)>> oper;
    // oper.insert({"+", add}); // 错误，add 不明确
    
    // 解决方式
    // 1. 利用函数指针
    int (*pAdd)(int, int) = add;
    oper.insert({"+", pAdd}); // 正确

    // 2. 利用 lambda
    oper.insert({"+", [](int a, int b)
    {
        return add(a, b);
    }});
}
```

## 14.9（类类型转换）

1. 语法（3 点注意）
2. 向 bool 的类型转换运算符
    * 为什么要声明为 explicit 的？
    * 什么时候声明即使是 explicit 的也会发生隐式类型转换？

3. 如何解决二义性问题