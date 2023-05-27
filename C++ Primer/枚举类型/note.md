```cpp
enum color
{
    RED,
    YELLOW,
    GREEN = INT64_MAX
}; // 不限定作用域的枚举类型，成员的潜在类型足够大，肯定可以容纳枚举值

enum stoplight {RED}; // 错误，重定义 ‘RED’

enum class peppers
{
    RED,
    YELLOW,
    GREEN
};

enum class A : short // 指定枚举类型的大小可以确保在不同的编译环境生成的代码一致
{
    val = 114514 // 错误，114514 大于 short 的范围
};

void test(void)
{
    color c0 = RED;
    peppers p0 = RED;     // 错误，"color"类型的值不能用于初始化 "peppers" 类型的实体
    peppers p1 = peppers::RED;
    color c1 = 1;         // 错误，int 类型的值不能用于初始化 "color" 类型的实体
    int i = color::RED;   // 正确，不限定作用域的枚举类型的成员隐式地转化为 int
    int j = peppers::RED; // 错误，"peppers"类型的值不能用于初始化 "int" 类型的实体
}
```

```cpp
enum D0; // 错误，未限定作用域的枚举声明必须指定大小

enum class D1; // 正确，限定作用域的枚举声明可以不指定大小，使用默认大小（int）

enum TOKEN
{
    INLINE = 128,
    VIRTUAL = 129
};

void func(int) {}

void func(unsigned char) {}

void test1(void)
{
    TOKEN t0 = INLINE, t1 = VIRTUAL;
    func(t0); // void func(int)
    func(t1); // void func(int)，即使 t1 可以用 unsigned char 存储，它也会被提升至整形
    unsigned char uc = t1;
    func(uc); // void func(unsigned char)
}
```