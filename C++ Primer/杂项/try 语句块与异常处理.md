# try 语句块与异常处理

异常处理机制为程序的 **异常检测** 与 **异常处理** 这两部分提供支持，异常处理包括：
* throw 表达式

    异常检测部分用 throw 表达式来表明遇到无法处理的问题，throw **引发**了异常
* try 语句块

    用 try 语句块实现异常处理，try 语句块以关键字 try 开始，并以一个或多个 catch 子句结束。

    try 语句抛出的异常会被某个 catch 语句处理，因此，catch 语句也被称为 **异常处理代码**

* 一套 **异常类**

    用于在 throw 表达式和相关的 catch 语句之间传递异常的具体信息

## 标准异常

C++ 标准库定义了一组类，用于报告标准库函数遇到的问题，这些异常类也可以被用户所使用

* exception：抽象类，最常见的问题
* runtime_error：只有在运行时才能检测到的问题
* range_error：运行时错误，生成结果超出有意义的值域范围
* overflow_error：运行时错误，计算上溢
* underflow_error：运行时错误，计算下溢
* logic_error：逻辑错误
* domain_error：逻辑错误，参数对应的结果值不存在
* invalid_argument：逻辑错误，无效参数
* length_error：逻辑错误，试图创建一个超出该类型最大长度的对象
* out_of_range：逻辑错误，使用一个超出范围的值

## throw 表达式

异常检测部分用 throw 表达式来表明遇到无法处理的问题，引发一个异常

throw 表达式 包含关键字 throw 与紧随其后的表达式组成，表达式的类型就是抛出异常的类型

throw 表达式抛出一个异常，并把控制权交给能处理该异常的**最近的 catch 语句**

例如：

```cpp
int a, b;
std::cout << "请输入两个整数" << std::endl;
std::cin >> a >> b;
if(b == 0) throw std::runtime_error("除数不能为 0！");
std::cout << "a / b = " << a / b << std::endl;
```

在程序运行的过程中，如果用户输入的除数为 0，我们的程序就会抛出一个类型为：runtime_error 的异常

并且，在 throw 表达式后面的所有语句均不会被执行

## try 语句块

### 通用语法格式

```cpp
try
{
    /* program statements */
    /* code */
}
catch(/* exception declaration */)
{
    /* handler statements */
}
catch(/* exception declaration */)
{
    /* handler statements */
}
...
```
其中，

* program statements 为程序的正常逻辑（包括了抛出异常）
* exception declaration 为**异常声明**
* handler statements 为异常的处理

### what 成员函数

标准库定义的异常类均包含一个成员函数 what，其声明为

```const char* what() const noexcept```

返回一个 c 风格的字符串

### 编写处理代码

```cpp
int a, b;
while (1)
{
    try
    {   
        std::cout << "请输入两个整数" << std::endl;
        std::cin >> a >> b;
        getchar();
        if(b == 0) throw std::runtime_error("除数不能为 0！");
        std::cout << "a / b = " << a / b << std::endl;
        break;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        char op;
        std::cout << "是否需要重新计算？(Y/N)" << std::endl;
        std::cin >> op;
        getchar();
        if(op == 'Y') continue;
        else break;
    }
}
```

程序本来要执行的代码放在 try 语句内，因为可能会抛出一个异常

### 函数在寻找处理代码的过程中退出

在复杂系统中，程序在遇到拋出异常的代码前，其执行路径可能已经经过了多个 try 语句块。例如，一个 try 语句块可能调用了包含另一个try 语句块的函数，新的 try 语句块可能调用了包含又一个 try 语句块的新函数，以此类推。

寻找处理代码的过程与函数调用链刚好相反，此过程通常被称为 **栈展开** 。当异常被抛出时，首先搜索抛出该异常的函数。如果没找到匹配的 catch 子句，终止该函数，并在调用该函数的函数中继续寻找。如果还是没有找到匹配的 catch 子句，这个新的函数也被终止，继续搜索调用它的函数。以此类推，沿着程序的执行路径逐层回退，直到找到适当类型的 catch 子句为止。
如果**最终还是没能找到任何匹配的 catch 子句**，程序转到名为 **terminate** 的标准库函数。该函数的行为与系统有关，一般情况下，执行该函数将导致程序 **非正常退出**。

对于那些**没有任何 try 语句块定义的异常，也按照类似的方式处理**：毕竟，没有 try语句块也就意味着没有匹配的 catch 子句。如果一段程序没有 try 语句块且发生了异常，系统会调用 terminate 函数并终止当前程序的执行。

## 对两数相除的进一步补充

```cpp
int main(void)
{
    std::function<int(const std::string &s)> myStoi = [](const std::string &s) -> int
    {
        int res = 0;
        for(const auto &ch : s)
        {
            if(res > (INT32_MAX - (ch - '0')) / 10)
            {
                throw std::length_error("输入的数字超过了支持的最大值（INT32_MAX）");
            }
            res = res * 10 + ch - '0';
        }
        return res;
    };

    std::string s0, s1;
    while (1)
    {
        try
        {   
            std::cout << "请输入两个整数" << std::endl;
            std::cin >> s0 >> s1;
            if(s0.find('.') != std::string::npos || s1.find('.') != std::string::npos)
            {
                throw std::runtime_error("输入的数据不是整形数据！");
            }
            getchar();
            int a = myStoi(s0), b = myStoi(s1);
            if(b == 0) throw std::runtime_error("除数不能为 0！");
            std::cout << "a / b = " << a / b << std::endl;
            break;
        }
        // 异常声明类型：抽象类 expection
        // 如果对于不同的异常需要有不同的处理方式，就不要这样写
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            char op;
            std::cout << "是否需要重新计算？(Y/N)" << std::endl;
            std::cin >> op;
            getchar();
            if(op == 'Y') continue;
            else break;
        }
    }
}
```