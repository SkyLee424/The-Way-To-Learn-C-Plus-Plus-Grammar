# 函数默认参数

## 定义

默认参数，又名缺省参数，是指：声明或定义函数时**为函数的参数指定一个默认值**。如果在调用该函数时，没有指定对应的形参，那么采用默认值；否则，采用传入的参数

例如

~~~C++
int add(int a, int b = 10)
{
    return a + b;
}
int main(void)
{
    std::cout << add(1) << std::endl; // 不指定参数
    std::cout << add(1, 2) << std::endl; // 指定参数
}
~~~

上述代码执行结果是

~~~
11
3
~~~

## 语法

``` 返回值类型 函数名(参数，默认参数)  ```

其中，参数可以全部为默认参数，也叫做**全缺省**参数；可以部分为默认参数，也叫做**半缺省**参数

## 注意事项

### 一、在指定默认参数时，需从右到左

例如下面的代码：

~~~C++
int add(int a = 10, int b)
{
    return a + b;
}
int main(void)
{
    std::cout << add(1) << std::endl; 
    std::cout << add(1, 2) << std::endl; 
}
~~~

这样写，编译器会报错，因为它不知道```add(1)```中的 1 到底给 a 还是给 b

### 二、全缺省参数在传参时，应从左到右

例如下面的代码：

~~~C++
int add(int a = 10, int b = 10, int c = 10, int d = 10)
{
    return a + b + c + d;
}
int main(void)
{
    std::cout << add(1) << std::endl; 
    std::cout << add(1, 2) << std::endl;
    std::cout << add(1, 2, 3) << std::endl;
    std::cout << add(1, 2, 3, 4) << std::endl; 
}
~~~

上述代码是完全ok的，但是，你不能像这样写

~~~C++
int add(int a = 10, int b = 10, int c = 10, int d = 10)
{
    return a + b + c + d;
}
int main(void)
{
    int a = 4;
    std::cout << add(, , , a) << std::endl; 
}
~~~

这样就违背了从左到右传参的原则

## 三、缺省参数不能在函数的声明和定义里同时出现，只能任选其一

例如：

~~~C++
int add(int a = 10, int b = 10);
int main(void)
{
    std::cout << add() << std::endl; 
}
int add(int a = 10, int b = 10)
{
    return a + b;
}
~~~

上述代码中，缺省参数在函数的声明和定义里同时出现，这是不可行的，因为编译器会为了**防止声明和定义里的缺省参数值不同而导致二义性**

因此，在代码实现中，只能选其中一个来放缺省参数

~~~C++
int add(int a = 10, int b = 10);
int main(void)
{
    std::cout << add() << std::endl; 
}
int add(int a, int b)
{
    return a + b;
}
~~~

## That's All