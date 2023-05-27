# 类类型转换

在 C++ 中，**转换构造函数** 与 **类型转换运算符** 共同构成了 **类类型转换** ，也叫 **用户定义的类型转换**

## 类型转换运算符

类型转换运算符 是类的特殊转换函数，可以将类的对象转换为其他类型

### 语法

``` operator type() ```

其中，

* type 为转换后的类型
* 一般来说，类型转换运算符声明为 const 的，即 ``` operator type() const```

从声明来看，可以发现：

1. 无返回值类型
2. 形参列表为空
3. 只能定义成类的成员函数

以下是相对应的错误示例

```cpp
struct SmallInt
{
    SmallInt(int val = 0)
    {
        if(val < 0 || val > 255)
            throw std::out_of_range("Bad SmallInt");
        this->val = val;
    }
    int operator int() const {} // 错误，不能对转换函数指定返回类型
    operator int(int) const {} // 错误，用户定义的转换中不允许使用参数
    operator int*() const
    {
        return 42; // 错误，返回值类型与函数类型不匹配
    }
private:
    std::size_t val;
};
```

此外，由于类型转换一般不需要改变对象的值，因此，类型转换函数一般被定义为 const 的

### 定义含有类类型转换运算符的类

```cpp
struct SmallInt
{
    SmallInt(int val = 0)
    {
        if(val < 0 || val > 255)
            throw std::out_of_range("Bad SmallInt");
        this->val = val;
    }
    // 隐式定义的类类型转换运算符
    operator int() const
    {   
        return val;
    }
private:
    std::size_t val;
};

void test(void)
{
    SmallInt a;
    a = 10; 
    int b = a + 10;
}
```

在上面的示例中：

* ```a = 10``` 先调用 *转换构造函数* ，将 10 转换为 SmallInt 对象，然后，再调用 ```operator=``` 完成赋值操作
* ```int b = a + 10;``` 调用 *类类型转换运算符* ，将 a 转换为 int，然后完成剩下的操作

此外，尽管 **一次只能执行一个用户定义的类型转换**，但是可以和内置数据类型结合使用

也就是说，可以先将 SmallInt 对象转换为 int，再使用内置类型转换将 int 转换为其他类型

同样的，可以将某一数据类型（如 double ）转换为 int，再使用转换构造函数转换为 SmallInt

例如：

```cpp
SmallInt a = 3.14; // 调用转换构造函数
double b = a + 3.14; // 将 a 转换为 int，再将 int 转换为 double
```

#### 建议

如果类类型与转换类型不存在明显的映射关系，那么，定义这样的类型转化运算符可能是具有误导性的

因此，建议在这种情况定义普通的成员函数来满足需求，而不是定义类型转换运算符

### 使用类型转换运算符带来的意外

在实际编码中，没有特殊需求，一般不会提供类型转换运算符，然而，定义向 bool 类型的转换还是比较常见的

对于 bool 类型来说，它是一种算术类型，因此，如果类对象可以被转换为 bool 类型，那它就可以被用于任何算术表达式中

例如，下面的代码：

```cpp
int i = 3;
std::cin << i;
```

由于 istream 没有定义 ```<<``` 运算符，因此，这段代码应该产生错误

然而，如果 ```std::cin``` 可以被转换为 bool 类型（实际上不会，因为 **向 bool 类型的转换是被显式定义的**，下文有所提及）那么，这个 bool 值将会被进一步提升至 int 类型，并作为 ```<<``` 运算符的左侧运算对象（0 或者 1），最终，该 bool 值被左移 i 个位置

### 显式定义的类型转换运算符

为了解决上述代码可能带来的问题，新标准引入了 **显式定义的类型转换运算符** 来解决这个问题

```cpp
struct SmallInt
{
    SmallInt(int val = 0)
    {
        if(val < 0 || val > 255)
            throw std::out_of_range("Bad SmallInt");
        this->val = val;
    }
    // 显式定义的类类型转换运算符
    explicit operator int() const
    {   
        return val;
    }
private:
    std::size_t val;
};

void test(void)
{
    SmallInt a = 10; // 正确，a 的构造函数不是显式定义的
    int b = a + 3; // 错误，a 的类型转换运算符是显式定义的
    int c = static_cast<int>(a) + 3; // 正确，显式要求类型转换
}
```

当类型转换运算符是显式的，若需要类型转换，需要显式要求

#### 例外

当类型转换被**用做条件**，上述的规则无效，也就是说，显式的类型转换会被隐式执行

例如：

```cpp
struct SmallInt
{
    SmallInt(int val = 0);
    explicit operator int() const;
    // 显式定义的类类型转换运算符
    explicit operator bool() const;
private:
    std::size_t val;
};

void test0(void)
{
    SmallInt a = 20;
    if(a) // if statement
        std::cout << static_cast<int>(a) << std::endl;
    while (a) // while
        break;
    for(int i = 0; a; ++i) // for 
        break;    
    int b = a ? static_cast<int>(a) : 0; // 三元运算符
}
```

上述代码能够正常执行

#### 转换为 bool 类型的类型转换运算符

根据上面的示例可以看出，将 **转换为 bool 类型的类型转换运算符** 定义为 explicit 的，是较为恰当的做法，能避免很多问题，因此，应当将 **转换为 bool 类型的类型转换运算符** 定义为 explicit 的

## 具有二义性的类型转换

如果一个类内部有多个类型转换，需要 **保证类类型到目标类型只有一种转换方式**，否则会出现二义性

常见的情况有：

* A 类定义了 接收 B 类对象的转换构造函数，并且，B 类定义了转换为 A 类的类型转换运算符（简单的说，二者都能将 B 转换为 A）
* 类定义了多个转换规则，而这些转换涉及的类型本身可以通过其他类型转换联系在一起

看起来有点抽象，上代码

### 情况一

```cpp
struct B;
struct A
{
    int val;
    A(int val);
    A(const B &b); // 将 B 转换为 A
};

struct B
{
    int val;
    B(int val)
        :val(val){}
    operator A() const // 将 B 转换为 A
    {
        return A(val);
    }
};

A func(const A &a)
{
    return a;
}

void test(void)
{
    B b = 10;
    A a = func(b); // 二义性调用
}

A::A(int val)
    :val(val){}
A::A(const B &b)
    :val(b.val){}
```
<!-- 在我的编译环境下，并没有产生二义性调用，而是调用了 B::operator A()，有可能是编译器试图优化的问题？-->
<!-- 3.31: 在 MSVC 环境下报错 -->
在 ```A a = func(b);``` 这句中，因为同时存在两种将 b 转换为 A 的方法

* 调用 B::operator A()
* 调用 A::A(const B &)

因此，对 func 的调用是有二义性的

### 情况二

同样的，对于如下代码：

```cpp
struct A
{
    int val;
    A() = default;
    operator double() const
    {return static_cast<double>(val);}
    operator int() const
    {return val;}
};

void func(long double lg){}

void test()
{
    A a;
    func(a); // 二义性调用    
}
```

在对 func 的调用中，无论是哪种类型转换，均不能精确匹配 func，因此，调用 ```operator double()``` 不比调用 ```operator int()``` 更好，最终产生二义性调用

```cpp
struct A
{
    int val;
    A() = default;
    A(int);
    A(double);
};

void test()
{
    long double lg = 10.0;
    A a(lg); //二义性调用
}
```

这也是一样的道理

上面的两个例子之所以会产生二义性，根本原因是：它们所需的标准类型转换的级别一致

* 对于 func 的调用，A => double => long double 和 A => int => long double 的级别相同
* 对于 A::A() 的调用，long double => int => A 和 long double => double => A 的级别相同
 
因此，当我们使用类类型转换时，如果转换过程 **包含标准类型转换**，那么，**标准类型转换** 决定了最佳匹配的过程

```cpp
short s = 1;
A a(s); // 调用 A::A(int);
```

上述代码之所以可以通过编译，是因为：将 short 提升至 int 优于将 short 转换为 double

### 避免二义性的产生

一般来说，有两种方式避免二义性的产生

* 不要让两个类执行相同的类型转换（上面的 A 、B 类就是很好的例子）
* 避免转换为内置算术类型，如果已经定义了一个转换为内置算术类型的转换，那么：

    * 不要定义接受算术类型的重载运算符
    * 不要定义转换到多种算术类型的类型转换

    ```cpp
    struct A
    {
        int val;
        A() = default;
        A(int val)
        {
            this->val = val;
        };
        operator int() const
        {
            return val;
        }
        // 接下来不要定义这个类型转换！！！
        // 让标准类型转换帮你完成！！！
        // operator double() const
        // {
        //     return static_cast<double> (val);
        // }
    
        // 不要定义这个运算符，让 A 转换为 int
        // 再使用内置运算符
        int operator+(int a)
        {
            return val + a;
        }
    };
    
    void test(void)
    {
        A a, a1;
        long double b = a; // 如果定义了 operator double() const，将会产生二义性调用
        // 如果定义了 int operator+(int a)，可能与内置算术产生冲突
        // 4.25: 实际上并不会，这句话精确匹配 int operator+(int a)，不会使用内置算术运算符
        int i = a + 1; 
    }
    ```

### 重载函数与二义性

当某几个重载的函数的形参分属与不同的类类型，并且，这些类都 **定义了相同的转换构造函数**，那么，将会产生二义性调用问题

举个例子：

```cpp
struct A
{
    A() = default;
    A(int);
};

struct B
{
    B() = default;
    B(int);
};

void func(A a);
void func(B b);

void test(void)
{
    func(10); // 错误，调用 A::A(int) 还是 B::B(int) ?   
}
```

当然，可以通过显式构造来消除这种二义性，不过，这通常意味着程序设计存在缺陷

```cpp
func(A(10));
```

再看这个例子：

```cpp
struct A; // 与上面的定义一致
struct B
{
    B() = default;
    B(double); // 不同点：double => B
};

void func(A a);
void func(B b);

void test(void)
{
    func(10); // 错误，调用 A::A(int) 还是 B::B(double) ?   
}
```

可以发现，即使 B 的转换构造函数接受一个 double，我们向 func 传入一个 int 类型的参数，仍然产生二义性调用

因此，可以得出这样的结论：**即使其中一个调用需要额外的标准类型转换，并且另一个不用（也就是精确匹配），也会产生二义性调用**

### 函数匹配与重载运算符

表达式的运算符的候选函数包括成员函数与非成员函数

举个例子

```cpp
B operator+(const B &b0, const B &b1);

struct B
{
    int val;
    B() = default;
    B(int);
    operator int();
};

void test1(void)
{
    B b0, b1;
    B b2 = b0 + b1; // 正确，调用 B operator+(const B &b0, const B &b1);
    B b3 = b0 + 1; // 错误，与内置加法运算冲突
}
```

在 ```B b3 = b0 + 1;``` 中的 '+'，候选函数就有两个

* B operator+(const B &b0, const B &b1);
* 内置运算符 "arithmetic + arithmetic"

因为可以将 1 转换为 B，调用 B operator+(const B &b0, const B &b1);

当然，也可以将 B 转换为 int，调用 内置运算符 "arithmetic + arithmetic"

因此，有如下结论：

* 如果定义了向内置类型转换的转换函数，就不要再重载算术运算符；
* 如果重载了算术运算符，就不要定义向内置类型转换的转换函数

#### 实例

```cpp
SmallInt operator+(const SmallInt &a, const SmallInt &b);

struct SmallInt
{
    int val;
    SmallInt() = default;
    SmallInt(int);
    operator int() const;
};

LongDouble operator+(const LongDouble&, double);

struct LongDouble
{
    LongDouble operator+(const SmallInt&);
    long double val;
    LongDouble() = default;
    LongDouble(double);
    operator double();
    operator float();
};

void test(void)
{
    SmallInt si;
    LongDouble ld;
    ld = si + ld; // 错误
    ld = ld + si;
}
```

先来分析 ```ld = si + ld```

* 对于普通函数 ```SmallInt operator+(const SmallInt &a, const SmallInt &b);```，无法将 LongDouble 转化为 SmallInt，因为一次只允许进行一次用户定义的类型转换
* 对于普通函数 ```LongDouble operator+(const LongDouble&, double);```，无法将 SmallInt 转化为 LongDouble，因为一次只允许进行一次用户定义的类型转换
* 对于成员函数 ```LongDouble operator+(const SmallInt&);```，无法将 LongDouble 转化为 SmallInt，也无法将 SmallInt 转化为 LongDouble

因此，我们定义的重载运算符均无法调用，唯一的做法就是调用内置运算符了

* 对于 si，先调用类型转换函数，转化为 int，再使用内置转换
* 对于 ld，先调用类型转换函数，然而，转换为 double 和转换为 float 的级别一致，因此，产生二义性：*应用了多个从 "LongDouble" 到内置类型的转换函数*

再来分析 ```ld = ld + si```

* 对于成员函数 ```LongDouble operator+(const SmallInt&);```，实参与形参精确匹配，可以调用
* 对于普通函数 ```LongDouble operator+(const LongDouble&, double);```，需要将 SmallInt 转换为 int，再将 int 转换为 double，相较于成员函数 ```LongDouble operator+(const SmallInt&);``` 的优先级低

因此，最终，```ld = ld + si``` 将正确调用成员函数 ```LongDouble operator+(const SmallInt&);```