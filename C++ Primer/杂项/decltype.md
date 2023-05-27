# decltype

## 作用于变量

当 decltype 作用于变量时，返回变量的类型

与 auto 不同的是，作用于变量，会保留顶层const和引用

例如：

```cpp
int a = 10;
decltype(a) b; // int
const int c = 10, &rc = c;
decltype(c) d = 6; // const int
decltype(c) rd = d; // const int&
```

## 作用于表达式

当 decltype 作用于表达式时，返回值取决于表达式的结果类型

* 表达式结果为左值，返回该表达式的结果类型的引用
* 表达式结果为右值，直接返回表达式的结果类型

### 表达式结果为左值

```cpp
int a = 10;
int *p = &a;
decltype(*p) b = a; // b 的类型为 int&
```

```*``` 运算符作用于指针，返回一个左值（*p = 2 是正确的）

因此，b 的类型为 int&

### 表达式结果为右值

```cpp
int a = 10;
decltype(a + 0) b; // b 的类型为 int
```

```a + 0``` 返回一个右值，因此，b 的类型为 int

此外，当 decltype 单独作用于变量，获得的是变量的类型，如果想要获得变量作为表达式的值，可以加一个括号

```cpp
int a = 10;
decltype((a)) b = a; // b 的类型为 int&
```

## 作用于函数

当 decltype 作用于函数时，返回该函数的类型

```cpp
void _swap(int &a, int &b);

using funcName = void(int &, int&);
funcName *f = _swap;

int a = 10, b = 20;
f(a, b);
std::cout << a << " " << b << std::endl;
```

像上面这种方式获得函数类型比较麻烦，可以用 decltype 简化操作

```cpp
decltype(_swap) *f = _swap;
```

**注意：** 作用于函数名只会得到函数类型，并不会转换为函数指针