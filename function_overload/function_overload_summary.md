# 函数重载总结

## 定义

函数重载是指在**同一个作用域内**，有多个**函数名相同**，但是**形参列表不同**（参数类型不同，参数个数不同，参数顺序不同）

## 重载条件

根据函数重载定义，不难得出：

1. 函数需在同一作用域
2. 函数名相同
3. 形参不同（可以是类型不同、数量不同、顺序不同等）

## 实例

~~~C++
#include<iostream>

void func(void)
{
    std::cout << "Running func(void)" << std::endl;
}

// 参数数量不同
void func(int a)
{
    std::cout << "Running func(int a)" << std::endl;
}

// 参数类型不同
void func(double a)
{
    std::cout << "Running func(double a)" << std::endl;
}

// 参数顺序不同
void func(int a, double b)
{
    std::cout << "Running func(int a, double b)" << std::endl;
}
void func(double a, int b)
{
    std::cout << "Running func(double a, int b)" << std::endl;
}

int main(void)
{
    func();
    func(0);
    func(0.0);
    func(0, 0.0);
    func(0.0, 0);
}
~~~

上述代码的执行结果如下
~~~
Running func(void)
Running func(int a)
Running func(double a)
Running func(int a, double b)
Running func(double a, int b)
~~~

## 注意事项：

### 一、 无法重载**仅按返回值区分**的函数

例如：

~~~C++
void func(void)
{
    std::cout << "Running void func(void)" << std::endl;
}
int func(void)
{
    std::cout << "Running int func(void)" << std::endl;
}
~~~

上述代码中，两个 func 的区别仅仅是返回值不同，编译器**无法判断**我们现在到底想要调用哪个函数，因为**形参是完全相同**的

### 二、引用作为重载条件

此项应该特别引起重视，例如：

~~~C++
void func(int &a)
{
    std::cout << "Running func(int &a)" << std::endl;
}

void func(const int &a)
{
    std::cout << "Running func(const int &a)" << std::endl;
}

int main(void)
{
    int a = 10;
    func(a);
    func(10);
}
~~~

执行结果如下

~~~
Running func(int &a)
Running func(const int &a)
~~~

上述代码能够正确执行的原因是：
1. a 作为一个变量，是可读可写的，而在函数： ```void func(const int &a)``` 中，a 是只读不可写的。因此，编译器会**优先调用** ```func(int &a)``` 。（实际上，调用 ```func(const int &a)``` 也是合法的。所以，在项目设计中，一般不允许像上面一样重载函数）

2. 10 作为一个常量，```int &a = 10``` 这句话**是不合法的**（具体结合引用的知识，这里不再赘述），```const int &a = 10``` 是合法的。因此，```func(const int &a)``` 也可以被正确调用

所以，此处的两个 func 的参数实际上是不同的，编译器能够知道我们要调用的是哪个函数，最终代码得以正确执行

如果像这样写：

~~~C++
void func(int &a)
{
    std::cout << "Running func(int &a)" << std::endl;
}
void func(const int a)
{
    std::cout << "Running func(const int a)" << std::endl;
}
int main(void)
{
    int a = 10;
    func(a);
}
~~~

上述代码无法经过编译，~~因为 a 无论是传入 ```func(int &a)```，还是 ```func(const int a)```，都是可行的，因此，编译器不知道我们到底要调用哪个函数，自然编译出错~~（其实我也没搞懂为什么之前的可以过编译，这个不行，明明调用两个函数都是合法的，为什么之前的就可以优先调用 ```func(int &a)``` 呢）

## 三、遇到含有默认参数的函数

例如：

~~~C++
void func(int a)
{
    std::cout << "Running func(int a)" << std::endl;
}

void func(int a, int b = 10)
{
    std::cout << "Running func(int a, int b = 10)" << std::endl;
}

int main(void)
{
    int a = 10;
    // func(a);
}
~~~

上述代码可以通过编译，然而，我们将main函数改为

~~~C++
int main(void)
{
    int a = 10;
    func(a);
}
~~~

就无法通过编译，因为无论调用哪一个都是合法的。

所以，应该像这样写：

~~~C++
int a = 10, b = 10;
func(a, b);
~~~

这样，就只有 ```func(int a, int b = 10) ```满足，自然可以通过编译

## That's All

