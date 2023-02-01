# 运算符重载

运算符重载是一种非常实用的工具，运用运算符重载，可以实现对自定义数据类型的快速处理。

## 一、类型

一般来说，有如下几种类型的运算符重载：

* 加（减）运算符重载
* 递增（减）运算符重载
* 左（右）移运算符重载
* 赋值运算符重载
* 关系运算符重载
* 函数调用（括号运算符）重载

## 二、语法格式

``` 返回值类型 operator运算符(参数) ```

此外，运算符**可以在类内、类外实现**，具体要看是哪个运算符（例如：左（右）移运算符就不能在类内重载）
<!-- ~~~``` -->
下面，将以 ```MyInt类``` 作为例子，分别展开叙述

## 三、MyInt 类的结构

为了后续赋值运算符重载的讲解，我们用一个 ```int``` 类型的**指针**来存储数据

~~~C++
class MyInt
{
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

};
~~~

事实上，为了减少内存的占用，无参构造函数应该将 ```num``` 初始化为 ```nullptr```

为了保证代码的健壮性，同时省去对空指针的判断，这里的无参构造函数仍然在堆区开辟了内存

<!-- ## 左（右）移运算符重载

为了实现自定义数据类型 ```MyInt类``` 的输入输出，需要用到左（右）移运算符重载 -->

## 四、加（减）运算符重载

想要实现 ```MyInt``` 类型的数据的加（减），需要重载加（减）运算符

结合运算符重载格式，函数声明如下

~~~C++
// 类内实现
MyInt operator+(const MyInt &b);

// 类外实现
MyInt operator+(const MyInt &a, const MyInt &b);
~~~

由于需要一个相加后的对象，因此返回值类型为 ```MyInt```

在类内实现，只需要相加的另一个对象 ```b``` 即可

而在类外实现，就需要两个对象了

函数整体如下：
~~~C++
// 重载 '+' 运算符（类内实现）
MyInt operator+(const MyInt &b)
{
    MyInt res(*this->num + *b.num);
    return res;
}

// 重载 '+' 运算符（类外实现）
MyInt operator+(const MyInt &a, const MyInt &b)
{
    MyInt res(*a.num + *b.num);
    return res;
}
~~~

测试函数如下：
~~~C++
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
~~~

当然，```a + b``` 是一种简便的写法，它**等价于** ```a.operator+(b);```（类内实现），或者 ```operator+(a, b);```

减号运算符重载就可以依葫芦画瓢了

## 五、递增（减）运算符重载

递增（减）运算符分为：

* 前置递增（减）
* 后置递增（减）

两种方式的实现方法有所不同，看完接下来的介绍，你会知道为什么**后置**递增（减）运算符的**效率低于前置**递增（减）运算符

### 1. 前置递增

函数声明：
~~~C++
// 前置递增（类内实现）
MyInt& operator++(void);

// 前置递增（类外实现）
MyInt& operator++(MyInt& a);
~~~

为了可以输出对象（++a），返回```MyInt```；为了实现如 ```(++(++(++a)))``` 这种链式操作，我们需要以**引用**的方式返回递增对象

函数整体如下：
~~~C++
// 重载前置 ++ 运算符
// 为了后续可以输出对象（++a） ，返回 MyInt
// 为了实现链式编程，以引用的形式返回
MyInt& operator++(void)
{
    *this->num += 1;
    return *this;
}
~~~

### 2. 后置递增

函数声明：
~~~C++
// 后置递增（类内实现）
MyInt operator++(void);

// 后置递增（类外实现）
MyInt operator++(MyInt& a);
~~~

我们可以发现：后置递增函数声明仅仅是返回值类型不同。

然而编译器**不允许重载仅按返回值区别的函数**，并且要**同时实现**前置递增和后置递增

因此，需要在函数参数加上一个**占位参数**，就像这样

~~~C++
MyInt operator++(int);
MyInt operator++(MyInt& a, int);
~~~

为了可以输出对象（a++），返回 ```MyInt```

你可能会问：为啥这次就不以引用的方式返回呢？

来，我们看看函数的内部
~~~C++
// 重载后置 ++ 运算符
// 由于后置 ++ 的特性，无法实现链式编程
MyInt operator++(int)
{
    MyInt temp(*this->num);
    *this->num += 1;
    return temp;
}
~~~

可以看到，为了实现自身递增的同时，返回值仍为原来的值，需要创建一个**局部临时变量**保存原来的值

我们知道：不建议返回局部变量的引用

>如果 temp 是一个**局部变量**，存放在栈区，那将 temp 作为返回值是很危险的，因为在 ```operator++``` 函数调用完后，**temp 会被释放**。因此，**不要返回局部变量的引用**

这下知道为啥不以引用的方式返回 ```temp``` 了⑧

此外，通过上述分析，我们知道 ```(((a++)++)++``` 这种操作是不合法的，因此，我们可以加上 ```const``` 修饰符，即：```const MyInt operator++(int)``` ，防止他人错误的调用我们的函数

**注意：**

前面提到为了区别两个递增重载，我们让后置递增的函数参数中加上了占位参数。事实上，**在重载递增（减）运算符时，编译器会认为加了占位参数的重载为后置递增（减），因此，切记不要写反了**

## 六、左（右）移运算符重载

为了实现 ```MyInt``` 类的输入输出，我们需要重载左（右）移运算符

左移运算符重载函数声明：

~~~C++
// << 运算符重载（类外实现）
std::ostream& operator<<(std::ostream &cout, const MyInt &a);
~~~

为什么这里不再给出类内实现呢？

因为左（右）移运算符重载无法在类内实现！

如果在类内实现，那重载后，等价于 ```a << cout``` 

这与我们想要的结果是**相反的**！

为了实现链式编程（如 ```std::cout << a << b;``` ），返回值类型为 ```std::ostream&```；由于全局只允许有一个 ```cout``` 的存在，因此，传入的参数为 ```cout``` 的引用；最后，只需要输出 ```a```，我们不想改变 ```a``` 的值，因此，传入的参数为 ```const MyInt &a```

函数整体如下

~~~C++
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
~~~

同样地，给出右移运算符重载函数的实现

~~~C++
// >> 运算符重载
std::istream &operator>>(std::istream &cin, MyInt &a)
{
    int temp;
    std::cin >> temp;
    // 不加判断也行，因为实例化对象时，一定在堆区开辟了内存
    // 当然，加上更好
    if (a.num == nullptr)
    {
        a.num = new int(temp);
    }
    else
        *a.num = temp;
    return cin;
}
~~~

写到这里，你会发现：```a.num``` 无法被访问，因为它是 ```MyInt``` 类的私有成员

为了访问 ```a.num```，我们需要用到**友元**的技术，不了解的hxdm可以在[这里](https://blog.csdn.net/Sky_Lee_1/article/details/128740343)看看友元的相关知识

只需要在 ```MyInt``` 类内声明 ```std::ostream &operator<<(std::ostream &cout, const MyInt &a)``` 是**好朋友**就 ok 了

即
~~~C++
// 友元配合 << 运算符，可以实现自定义数据类型输出
friend std::ostream &operator<<(std::ostream &cout, const MyInt &a);
friend std::istream &operator>>(std::istream &cin, MyInt &a);
~~~

## 七、赋值运算符重载

赋值运算符与拷贝构造函数十分类似，区别在于前置在赋值的时候调用；后置在实例化对象的时候调用

在一个类中，和拷贝构造函数一样，如果我们不自己实现赋值运算符重载，编译器会在类内 **自动生成** 一个赋值运算符重载函数

先给出 ```MyInt``` 类的拷贝构造函数

~~~C++
MyInt(const MyInt &b)
{
    this->num = new int(*b.num);
}
~~~

赋值运算符重载函数声明如下：

~~~C++
// '=' 运算符重载（类内实现）
MyInt& operator=(const MyInt &b);
~~~

为啥这里只给出类内实现捏？

前面提到了编译器会在类内 **自动生成** 一个赋值运算符重载函数，如果我们还在类外编写一个赋值运算符重载，会产生**二义性**，编译器自然不允许这样的事发生

并且，编译器生成的赋值运算符重载函数是**浅拷贝**，如果**类中含有在堆区创建的属性**，那就最好**不要使用默认的赋值运算符重载函数**了

为了实现连续赋值操作，即 ```a = b = c```，需要返回 ```MyInt``` 的引用

函数整体如下：

~~~C++
// '=' 运算符重载（类内实现）
MyInt& operator=(const MyInt &b)
{
    if(this->num)
    {
        *num = *b.num;
    }
    else num = new int(*b.num);
    return *this;
}
~~~

这里的赋值运算符重载函数就是一个**深拷贝**

## 八、关系运算符重载

下面以重载 '==' 为例

函数声明：
~~~C++
// 重载 '==' （类内实现）
bool operator==(const MyInt& b);

// 重载 '==' （类外实现）
bool operator==(const MyInt &a, const MyInt& b);
~~~

整体函数如下：
~~~C++
// 重载 '==' 运算符

bool operator==(const MyInt &b)
{
    if (*this->num == *b.num)
        return true;
    return false;
}
~~~

## 九、函数调用（括号运算符）重载

### 语法

``` 返回值类型 operator()(参数) ```

### 实例
例如，我们想实现一个 ```Print类```，这个类可以打印 ```string``` 类型的数据
~~~C++
class Print
{
public:
    void operator()(const std::string &s)
    {
        std::cout << s << std::endl;
    }
};
~~~

返回值类型为 ```void```，参数为 ```string```

### 调用方式

有两种调用方式：

1. 实例化一个对象，用对象的成员函数调用
2. 匿名对象调用

#### 方式一

~~~C++
Print print;
print("homo")
~~~

先实例化了一个对象，再通过对象的成员函数调用

#### 方式二

~~~C++
Print()("homo")
~~~

这里，```Print()``` 就是一个匿名对象，该对象在执行完当前语句就会被销毁

无论是哪种方式，使用括号运算符都很像一个函数的调用，因此，重载的括号运算符也叫做**仿函数**

仿函数的书写与运用都十分灵活，使用时需多加注意

## 十、整体测试代码

经过上面的各种运算符重载分析，可以写出如下测试代码：

### 代码

~~~C++
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
            num = nullptr; // 防止野指针
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
    // 由于后置 ++ 的特性，无法实现链式编程
    // 此外，((a++)++)++ 是不合法的
    // 因此，加上 const 限定符，防止他人错误调用
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
~~~

### 测试用例
#### Test Case1

Input:
~~~
0
6 9
~~~

Expected Output:
~~~
6 + 9 = 15
6 - 9 = -3
(6 += 9) = 15
(15 += 9) = 6
~~~

#### Test Case2

Input:
~~~
1
1
~~~

Expected Output:
~~~
1
2
3
3
6
3
2
2
2
1
~~~
#### Test Case3

Input:
~~~
2
114514
~~~

Expected Output:
~~~
b = 114514
c = 114514
a = 114514, b = 114514, c = 114514
~~~
#### Test Case4

Input:
~~~
3
2 2
~~~

Expected Output:
~~~
2 == 2
2 == 2
~~~
#### Test Case5

Input:
~~~
4
homo
~~~

Expected Output:
~~~
homo
homo
~~~
最后，感谢你耐心看到这里，希望我的文章对你有所帮助