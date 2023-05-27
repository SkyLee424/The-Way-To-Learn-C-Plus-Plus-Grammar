## 拷贝、赋值与销毁

1. 拷贝构造函数在以下情况会被隐式调用：

    * 用 ```operator=``` 定义变量

        ```cpp
        struct Test;

        Test t0;
        Test t1 = t0; // 隐式调用拷贝构造函数
        ```
    
    * 将实参传递给 非引用类型 的形参
    * 返回一个 非引用类型 的对象
    * 用花括号初始化一个 **聚合类** 成员
        ```cpp
        class Test{};

        struct A
        {
            int a;
            Test t;
        };
        
        Test t;
        Test t1 = t; // 隐式调用拷贝构造函数
        ```

    因此，拷贝构造函数通常不应该是 explicit 的

2. 根据上述第一点的分析，为什么拷贝构造函数自己的参数类型 **必须是引用？**

    如果不使用引用传递，而是值传递，在传入实参的过程中，需要拷贝该实参，为了拷贝实参，就会再次调用该类的拷贝构造函数，最终产生死循环，因此，拷贝构造函数自己的参数类型 **必须是引用**

3. 构造函数按成员声明顺序来构造成员，而**析构函数**按照成员声明的**逆序**来**析构**成员

    ```cpp
    struct A
    {
       std::string name;
       A(const std::string& s)
            :name(s)
        {
            std::cout << "正在构造 " << name << std::endl;
        } 
        ~A()
        {
            std::cout << "正在析构 " << name << std::endl;
        }
    };

    struct B
    {
        A a0;
        A a1;
        B()
            :a1("a1"), a0("a0") {}
        ~B()
        {
            std::cout << "B 类析构开始" << std::endl;
        }
    };
    
    void test(void)
    {
        B b;
    }
    ```

    **输出：**

    ```
    正在构造 a0
    正在构造 a1
    B 类析构开始：
    正在析构 a1
    正在析构 a0
    ```

    可以看出，析构函数先执行函数体，再销毁成员

4. 析构函数 **不会** 自动销毁指针、引用成员
5. 认识到析构函数的**函数体本身不销毁对象**是十分重要的，成员是在析构函数体之后隐含的析构阶段中销毁的
6. 用 default 关键字修饰成员的声明时，合成的函数默认是内联的，如果不希望是内联的，应该只对类外的成员函数的定义使用 defalut

    ```cpp
    class A {};
    A::A(const A&) = default;
    ```
7. 与 default 不同的是，=delete **只能** 出现在函数的第一次声明处
8. 与 default 不同的是，=delete 可以指定 **任意函数**
9. 合成的拷贝控制成员可能是删除的，简单来说，如果一个类有数据成员不能默认构造、拷贝、赋值、销毁，那么其对应的成员函数将被定义为删除的（想想有哪几种情况）

## 拷贝控制与资源管理

1. 编写赋值运算符时，必须保证自赋值时，赋值运算符能正常工作
2. 通过 **先拷贝右侧对象**，可以处理自赋值和异常情况，拷贝完右侧对象后，释放左侧对象的资源，并更新左侧对象的指针成员
    ```cpp
    HasPtr& operator=(const HasPtr &h) noexcept
    {
        val = h.val;
        std::string *newName = new std::string(*h.name);
        delete name; // 无论是否是自赋值，都是安全的
        name = newName; // 因为，这里会赋新值
        return *this;
    }
    ```
3. 使用智能指针代替内置指针会方便很多（析构不用我们干涉）
4. 对于一个类，如果其实现了 swap 方法，那么算法将使用该类的 swap，否则，使用 STL 的 swap，因此，对于分配了资源的类，定义 swap 是一种十分有效的优化方式
5. 在 ```operaotr=``` 中使用 swap

    ```cpp
    class HasPtr
    {
        friend void swap(HasPtr &p0, HasPtr &p1);
        std::string *ps;
    public:
        HasPtr(const std::string &s = std::string())
            :ps(new std::string(s)) {}
        HasPtr(const HasPtr &h)
            :ps(new std::string(*h.ps)) {}
        HasPtr& operator=(HasPtr h);
        ~HasPtr()
        {
            delete ps;
            ps = nullptr;
        }
    };
    
    inline
    void swap(HasPtr &p0, HasPtr &p1)
    {
        swap(p0.ps, p1.ps);
    }
    
    // 注意，这里是值传递，调用 HasPtr 的拷贝构造
    // 能够巧妙处理自赋值和异常情况
    // 1. 自赋值：
    // 在交换前，就已经拷贝了右侧对象的值
    // 2. 异常
    // 唯一发生异常的地方就是拷贝构造里的 new
    // 就算异常，自己的值也不会改变
    // 因为不会执行 swap 就已经结束了 operator=
    HasPtr& HasPtr::operator=(HasPtr h)
    {
        // 交换自己和 拷贝对象 的成员
        swap(*this, h);
        return *this;
    }
    ```

## 右值引用

1. **变量**表达式**是左值**
2. 对于自定义类型，可以添加移动构造函数避免 vector 扩容带来的拷贝问题
3. 移动构造函数必须确保 源对象 被移动后，**对其运行析构是安全的**
4. 移动构造函数 **通常不分配任何内存**
5. 移动构造函数 **通常不应该抛出异常**，因此，将其指明为 noexpect 的，并且，在移动构造函数的 **声明、定义** 处 **都** 指定 noexpect
6. 除非 vector 知道元素类型的移动构造函数不会抛出异常，否则，在 reallocate 时，会使用元素的拷贝构造而不是移动构造

    因为，vector 需要保证在调用 push_back 时，就算出现异常，vector 自身不会发生改变

    如果移动构造在移动某一个元素的过程抛出了异常，由于 vector 旧空间中移动源元素已经改变，这样就不能满足 vector 的需求

7. 与拷贝赋值运算符一致，移动赋值运算符也需要正确处理自赋值情况
8. 移动操作还必须保证移后源对象的有效性，即：可以安全的赋新值，或者仍可以安全使用而不依赖当前值
9. 如果类定义了

    * 拷贝构造函数
    * 拷贝赋值运算符
    * 析构函数

    编译器将不会自动合成移动构造函数和移动赋值运算符

10. 如果定义了移动构造函数或者移动赋值运算符，编译器将不会自动合成拷贝构造函数和拷贝赋值运算符，因此，需要定义自己的拷贝操作
11. 如果类没有定义自己的移动操作，右值也将被拷贝
12. 移动迭代器：一种迭代器适配器，它生成的迭代器解引用后，得到一个右值引用
13. 可以使用 ```make_move_iterator``` 将一个普通迭代器转换为移动迭代器
14. 标准库不保证哪些算法使用移动迭代器，因此，需要确保该算法在元素被移动后不再访问它，才将移动迭代器传递给算法

### 引用限定符

我们指出 this 的 左值/右值 属性与 指出 this 的 const 属性一致，即：在 **非 static 成员函数** 的参数列表后加上 **引用限定符**

例如：
```cpp
void memberFunc0(void) &;
void memberFunc1(void) &&;
```

其中，```memberFunc0``` 指明了 this 的左值属性，也就是说，只有当 this 是一个左值的时候，才能调用 ```memberFunc0``` 
而 ```memberFunc1``` 则指明了 this 的右值属性

一个函数可以同时由 const 和 引用限定符 同时修饰，但此时，**const 限定符必须在 引用限定符之前**

```cpp
void memberFunc2(void) const &;
```

与定义 const 函数不同，如果定义引用限定的函数，并且发生**重载**，就 **必须** 对所有函数加上 引用限定符，或者都不加：

```cpp
// 如果定义两个及以上名字相同，且参数列表完全一致的成员函数
// 必须都加上引用限定符
class Foo
{
    std::vector<int> data;
public:
    Foo sort(void) &&;
    // Foo sort(void) const; // 错误，重载参数类型相同的两个成员函数需要它们同时具有或缺少引用限定符

    using cmp = bool(const int &, const int &);
    Foo sort(cmp*);       // 正确，参数列表不同
    Foo sort(cmp*) const; // 正确，两个版本均不包含引用限定符
};
```

