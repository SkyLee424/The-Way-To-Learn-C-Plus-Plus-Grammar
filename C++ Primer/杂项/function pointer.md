# 函数指针

> 函数指针就是指向函数的指针变量。每一个函数都有一个入口地址，该入口地址就是函数指针所指向的地址。

函数指针可以让「函数」成为一个「变量」

## 函数指针的适用场景

> 回调（callback）。我们调用别人提供的 API函数(Application Programming Interface,应用程序编程接口)，称为Call；如果别人的库里面调用我们的函数，就叫Callback。

实例：

```cpp
// 函数类型固定
int cmp(const void *a0, const void *a1)
{
    int *_a0 = (int*)a0; // 将 void* 强转为 int*
    int *_a1 = (int*)a1;
    return *_a0 - *_a1; // 升序
}

int main(void)
{
    int *arr = new int[10];
    for(int i = 0; i < 10; ++i)
    {
        arr[i] = rand() % 100;
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    // void qsort(void *base, // void*类型，代表原始数组
    //    size_t nmemb, // 第二个是size_t类型，代表数据数量
    //    size_t size, // 第三个是size_t类型，代表单个数据占用空间大小
    //    int(*compar)(const void *,const void *)// 第四个参数是函数指针
    //   );
    qsort(arr, 10, sizeof(int), cmp); // Callback
    for(int i = 0; i < 10; ++i)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}
```

这里，我们向 qsort 函数传入了我们自定义的函数的函数指针

## 函数指针的声明

先给出一个函数的声明

```cpp
int add(int a, int b);
```

要想声明一个指向 add 函数的函数指针，只需要这样做

```cpp
int (*pFunc)(int, int);
pFunc = add;
```

从声明来看，pFunc 是一个指针类型；右侧含有一个形参列表，因此，pFunc 指向的是一个函数；再看最左侧，可以发现该函数的返回值类型为 int

**注意：** (*pFunc) 两端的括号一定不能少，比如，错误的将 pFunc 声明成如下形式

```cpp
int *pFunc(int, int);
```
可以明显看出：我们**声明了一个函数**，并不是一个函数指针，该函数的返回值为 int*

## 函数指针的使用

### 给函数指针赋值

声明完函数指针后，就应该给其赋值，函数指针的赋值方式与常规的指针类型类似

```cpp
int (*pFunc)(int, int);
pFunc = add;
pFunc = &add; // 取地址符是可选的
pFunc = nullptr;
```

**注意：** 在赋值时，目标函数的类型应该与函数指针类型精确匹配

```cpp
// 函数的声明
int func0(int a, int b);
double func1(int a, int b);
int func2(int a);

// 函数指针的声明
int (*pFunc0)(int, int);

// 赋值操作
pFunc0 = func0; // 正确
pFunc0 = func1; // 错误，返回值类型不同
pFunc0 = func2; // 错误，参数列表不同
```

### 使用函数指针
当我们声明了一个函数指针，并将其赋值以后，我们就可以使用该函数指针了

```cpp
int (*pFunc)(int, int);
pFunc = add;
std::cout << pFunc(1, 2) << std::endl;
std::cout << (*pFunc)(1, 2) << std::endl; // 与上面是等价的
```

## 重载函数的指针

与给函数指针赋值类似，当我们使用重载函数时，上下文必须清晰地界定应该选择哪个函数

```cpp
int func(int a);
double func(int a, int b);
int func(void);

int (*pf)(int) = func; // 正确，指向 int func(int a);
void (*pf1)(int, int) = func; // 错误
int (*pf2)(int, int) = func; // 错误
```

## 使用 auto 与 decltype 简化操作

当我们明确知道需要指向的函数时，可以使用 auto 和 decltype 来简化声明的过程

```cpp
using Func = decltype(funcPointer); // Func 是函数类型
using PFunc = decltype(funcPointer) *; // PFunc 是指针类型
auto p0 = funcPointer; // void (*p0)(int)
Func* p1 = funcPointer;
decltype(funcPointer)* p2 = funcPointer; // void (*p2)(int a)
PFunc p3 = funcPointer;

p0(1);
p1(2);
p2(3);
p3(4);
```

**注意：** 使用 decltype 时，切记其返回的是函数类型而不是指针类型，因此，记得加上 '*' 号来显式的指定我们需要的是函数指针类型