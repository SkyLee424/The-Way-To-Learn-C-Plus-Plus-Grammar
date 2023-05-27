# extern 关键字

## Basic Use

```c++
extern "C" void c_f(void); // 声明一个 C 函数
```

## extern 与头文件

```cpp
// 当头文件被包含在 复合链接指示 的花括号中
// 头文件里的所有函数声明都将被当做 链接指示 所指的语言编写的
extern "C"
{
    #include<string.h>
}
```

## extern 与函数指针

```cpp
extern "C" void (*c_pf)(int a); // c_pf 指向一个 C 函数

void (*cpp_pf)(int a); // cpp_pf 指向一个 C++ 函数

c_pf = cpp_pf; // 错误，不能给一个指向 C 函数的指针赋值为一个指向 C++ 函数的指针
cpp_pf = c_pf; // 错误，...

func(c_pf); // 正确
func(cpp_pf); // 错误，不能给一个 C 函数传入一个指向 C++ 函数的指针
```

## extern 对整句声明有效

```cpp
// 声明 func 为一个 C 函数
// 并且，extern 对整句声明有效
// 也就是说，传入 func 的 pf 也得是一个指向一个 C 函数的指针  
extern "C" void func(void (*pf)(int)); 

// 如果需要给一个 C++ 函数传入一个指向 C 函数的指针
// 需要使用类型别名

// using C_PF = extern "C" void(int);
extern "C" typedef void C_F(int);

void cppFunc(C_F *c_pf)
{

}

cppFunc(c_pf); // 正确

```

## 重载函数与 extern

```cpp
extern "C" void funcOverload(void);

// 错误，C 语言不支持函数重载
extern "C" void funcOverload(int);
```

如果重载函数中有一个是 C 函数，其余的 **必定** 是 C++ 函数

```cpp
class shortNum {};
class BigNum {};
extern "C" double func(double);
extern shortNum func(shortNum); // C++
extern BigNum func(BigNum);     // C++
```