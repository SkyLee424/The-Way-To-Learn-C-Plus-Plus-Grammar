## 15.2

### 派生类构造函数

1. 派生类不能直接初始化基类的成员，而是必须使用基类的构造函数来初始化它的基类部分：

    ```cpp
    struct Dad
    {
        int a;
        int b;
    };
    
    struct Son : public Dad
    {
        Son()
            :a(1), b(2){} // 错误，"a" 不是类 "Son" 的非静态数据成员或基类
    };
    ```

2. 除非我们指出，否则派生类基类部分将执行默认初始化：

    ```cpp
    struct Dad
    {
        int a;
        int b;
        Dad(int a, int b)
            :a(a), b(b) {}
    };
    
    struct Son : public Dad
    {
        int c;
        int d;
        Son(int a, int b, int c, int d)
            :c(c), d(d) {}        // 错误，基类没有默认构造函数
    };
    ```

    在这个例子中，由于我们没有特殊指定如何初始化基类，因此，基类部分将执行默认初始化；

    然而，基类却没有默认构造函数，因此，Son 类的构造函数是错误的

3. 指定初始化基类的方式：

    ```cpp
    struct Dad
    {
        int a;
        int b;
        Dad(int a, int b)
            :a(a), b(b) {}
    };
    
    struct Son : public Dad
    {
        int c;
        int d;
        Son(int a, int b, int c, int d)
            :Dad(a, b), c(c), d(d){} // 将 a, b 用于初始化基类成员
    };
    ```

    如上所示，用 基类类名 + 圆括号的实参列表 的形式为构造函数提供初始值

**关键：** 明确一点：每个类负责实现自己的接口，尽管可以在派生类的构造函数体内给基类成员赋值，也 **建议** 不要这么做，而是通过调用基类的构造函数。**总而言之，派生类应该遵循基类的接口**

### 派生类的声明

派生类的声明 **不包含** 它的派生列表

### 被用作基类的类

1. 如果某个类要被用作基类，则 **该类必须被定义，而不是仅仅被声明：**

    ```cpp
    class Dad;
    class Son : public Dad {};
    ```

    上述代码就违反了这个规则

2. 一个类可以是基类，也可以是派生类：

    ```cpp
    struct Grand
    {
        int a;
    };
    
    struct Dad : Grand
    {
        int b;
    };
    
    struct Son : Dad
    {
        int c;
    };
    
    struct Grandson : Son
    {
        int d;
    };
    ```

    其中，Grand 是 Dad 的 **直接基类**，同时，它也是 Son 的 **间接基类**

    每个类会继承直接基类的 **所有** 成员（包括 private 的），因此，对于最终的派生类，它会继承继承链中的所有成员


### 防止继承的发生

C++11 引入了 **关键字 final** 来表示某一个类不能作为基类：

```cpp
struct Base{};

struct NoDerived final {}; // 不允许继承

struct Last final : Base {}; // 不允许继承

// struct A : NoDerived {}; // 错误，NoDerived 不能作为基类

// struct B : Last {}; // 错误，last 不能作为基类
```

### 类型转换与继承（重点）

1. 静态类型在编译时已知，但动态类型直到运行时才可知
2. 如果表达式既不是引用，也不是指针，那它的静态类型和动态类型相同
3. 派生类到基类的自动转换只对 **指针和引用类型** 有效
4. 派生类到基类的自动转换可能会由于 **访问受限** 而变得不可用 *（待补充）*
5. 不存在从基类到派生类的 **隐式** 类型转换
6. 由于第 4 条规则的存在，即使一个基类指针或引用绑定了一个派生类对象，也不能执行从基类到派生类的 **隐式** 类型转换（即使这样做是安全的）：

    ```cpp
    Bulk_quote b; // 派生类对象
    Quote &rq = b; // 基类对象
    Bulk_quote &rb = &rq; // 错误，不存在从基类到派生类的隐式类型转换
    ```

    原因：编译器无法在 **编译期** 判断某个特定的转换是否安全，因为编译器只能通过检查指针和引用的 **静态类型** 来确定一个转换是否安全

    当然，在我们确定该转换是安全的，可以使用以下方式实现 从基类到派生类的 **显式** 类型转换：

    1. 使用 dynamic_cast，该转换可以在 **运行时** 检查某个转换是否安全
    2. 使用 static_cast，该转换 **强制覆盖** 编译器的检查工作

    ```cpp
    Bulk_quote b;
    Quote &rq = b;
    Bulk_quote &rb0 = dynamic_cast<Bulk_quote&>(rq);
    Bulk_quote &rb1 = static_cast<Bulk_quote&>(rq);
    ```

    建议使用 dynamic_cast，因为它仍具有类型检查功能，相比 static_cast 更安全（特别是我们进行了错误的类型转换）

    ```cpp
    Quote q;
    // 注意：不能将基类对象转换为派生类对象！！
    Bulk_quote &rb0 = dynamic_cast<Bulk_quote&>(q);  // 会抛出异常
    Bulk_quote *pb0 = dynamic_cast<Bulk_quote*>(&q); // 不会抛出异常，但是返回一个 0
    Bulk_quote &rb1 = static_cast<Bulk_quote&>(q);  // 不会抛出异常，可能产生未定义行为！
    ```

## 15.3

1. 与普通函数不同，即使我们不会使用到某一个虚函数，也 **必须** 为其提供定义，因为虚函数是运行时绑定，连编译器也无法确定到底会使用哪个虚函数
2. C++ 支持多态的根本原因：引用和指针的静态类型与动态类型可以不同
3. 派生类 **重写** 基类的虚函数，即：
   * 形参列表相同
   * 返回值类型相同，该规则存在例外情况：当派生类**返回的是类本身的指针或引用**，并且**从派生类到基类的类型转换是可访问的**，规则失效（协变？）


### final 和 override 说明符

派生类如果定义了一个函数 **与基类的虚函数名字相同，形参列表不同**，仍是合法行为，编译器会认为这是派生类重载了一个函数，因此，该函数 **不会覆盖** 基类的版本，这可能是与预期不符的，因为我们可能错误地编写了形参列表，**debug 这个错误是十分困难的**

我们可以使用 override 说明符来处理这个问题

override 用于说明某个函数 **重写** 了基类的虚函数，虽然派生类重写基类虚函数可以不用 override 说明符来修饰，但这样做有两个好处：

* 使程序员的意图更加明显，别人一看就知道这个函数重写了基类的虚函数
* 让编译器发现一些错误，就像上文提到的一样

第二个好处更加重要，因为如果某个函数被标记为了 override，但该函数却没有覆盖已经存在的虚函数，编译器会报错：

```cpp
struct Dad
{
    virtual void f0() const &;
    virtual void f1() const &;
    void f2();
};

struct Son : Dad
{
    void f0() const & override; // 正确，f0 覆盖了基类的虚函数 f0
    
    // 错误，缺少引用限定符，与 void f1() const & 构成重载，没有覆盖基类虚函数 f1
    void f1() const override; 

    // 错误，与基类虚函数的形参列表不同，构成重载，没有覆盖基类虚函数 f1
    void f1(int) const & override; 

    void f2() override; // 错误，f2 不是虚函数

    void f3() override; // 错误，基类没有 f3
};
```

还可以将某个虚函数指定为 final，这样，之后任何尝试重写该函数的行为都将错误：

```cpp
struct Dad
{
    virtual void f0(void); 
};

struct Son : Dad
{
    void f0(void) override final;
};

struct GrandSon : Son
{
    void f0(void) override; // 错误，无法重写“final”函数 "Son::f0"
};
```

**注意：** final 与 override 说明符出现在形参列表（包括 const 和 引用限定符）之后，以及尾置返回类型之后

此外，在类外定义的成员函数 **不要** 添加 override 说明符

### 虚函数与默认实参

虚函数也可以有默认实参，但是，该实参值由基类决定，即使实际运行的是派生类的版本也是如此：

```cpp
struct Dad
{
    virtual void f0(int a = 0)
    {
        std::cout << "In Dad: " << a << std::endl;
    } 
};

struct Son : Dad
{
    void f0(int a = 1) override final
    {
        std::cout << "In Son: " << a << std::endl;
    }
};

void func(Dad &d)
{
    d.f0();
}

Son s;
func(s); // In Son: 0
```

**建议：** 如果虚函数需要使用默认实参，基类和派生类定义的默认实参最好一致

## 15.4

1. 可以为纯虚函数提供定义，但函数体必须在 **定义在类的外面**
2. 抽象基类负责定义接口，派生类负责定义
3. **重构**

    重构负责重新设计类的体系，以便将操作和（或）数据从一个类移动到另一个类中

    例如：

    ```cpp
    // 基类
    class Quote
    {
        std::string ISBN;
    
    protected:
        double price; // 书籍的价格
    
    public:
        Quote(const std::string &ISBN = "", double price = 0.0)
            : ISBN(ISBN), price(price) {}
        virtual ~Quote() = default;
        virtual double net_price(int cnt) const
        {
            return price * cnt;
        }
    };
    
    /**
     * @brief 用于保存折扣值和购买量的类，派生类可以使用这些数据实现不同的折扣策略
     * 利用到了重构这个概念
     * 
     */
    class Disc_quote : public Quote
    {
    protected:
        int quantity = 0; // 满足折扣的书籍临界数量
        double discount; // 折扣
    public:
        Disc_quote(const std::string &ISBN = "", double price = 0.0, int quantity = 0, double discount = 1.0)
            :Quote(ISBN, price), quantity(quantity), discount(discount) {}
        virtual double net_price(int cnt) const = 0;
    };
    
    class Bulk_quote : public Disc_quote
    {
    public:
        Bulk_quote(const std::string &ISBN = "", double price = 0.0, int minCnt = INT32_MAX, double discont = 1.0)
            : Disc_quote(ISBN, price, minCnt, discont) {}
        double net_price(int cnt) const override
        {
            return cnt >= quantity ? cnt * price * discount : cnt * price;
        }
    };
    ```

## 15.5

### protected 的性质

* 不能被用户访问
* 可以被派生类的成员和友元访问

    ```cpp
    struct Dad
    {
    protected:
        int val;
    };
    
    struct Son : Dad
    {
        friend void f1(void);
        void func(void)
        {
            val = 1; // 正确，派生类的成员可以访问自己受保护的成员
        }
    };

    void f1(void)
    {
        Son s;
        s.val = 1; // 正确，派生类的友元可以访问派生类受保护的成员
    }
    ```

* 派生类的成员和友元只能通过 **派生类对象** 访问受保护的成员，不能通过 **基类** 对象访问受保护的成员： 

    ```cpp
    void Son::func(Dad &d)
    {
        d.val = 1; // 错误，派生类的成员不能访问基类受保护的成员
    }
    ```

对基类的访问权限仅仅由基类的访问说明符有关

### 派生类到基类转换的可访问性

对于代码给定的某个节点来说，如果**可以通过派生类访问基类部分的公有成员**，那么派生类到基类转换是可访问的，否则就不行

下面的例子可以充分验证上述结论：

```cpp
class Dad
{
private:
    int priMem;
protected:
    int proMem;
public:
    int pubMem;
};

class PubSon : public Dad
{
    void func(void)
    {
        PubSon *s;
        Dad *d = dynamic_cast<Dad*>(s); // 正确，此时 Dad 的公有成员可以被访问
    }
};

class ProSon : protected Dad
{
    void func(void)
    {
        ProSon *s;
        Dad *d = dynamic_cast<Dad*>(s); // 正确，此时 Dad 的公有成员可以被访问
    }
};

class PriSon : private Dad
{
    void func(void)
    {
        PriSon *s;
        Dad *d = dynamic_cast<Dad*>(s); // 正确，此时 Dad 的公有成员可以被访问
    }
};

void testPublic(void) // 测试公有继承
{
    PubSon *s;
    Dad *d = dynamic_cast<Dad*>(s); // 正确，此时 Dad 的公有成员可以被访问   
}

void testProtected(void) // 测试保护继承
{
    ProSon *s;
    Dad *d = dynamic_cast<Dad*>(s); // 错误，此时 Dad 的公有成员不可以被访问   
}

void testPrivate(void) // 测试私有继承
{
    PriSon *s;
    Dad *d = dynamic_cast<Dad*>(s); // 错误，此时 Dad 的公有成员不可以被访问   
}

class GrandPubSon : public PubSon
{
    void func(void)
    {
        PubSon *p;
        Dad *d = dynamic_cast<Dad*>(p); // 正确，此时 Dad 的公有成员可以被访问   
    }
};

class GrandProSon : public ProSon
{
    void func(void)
    {
        ProSon *p;
        Dad *d = dynamic_cast<Dad*>(p); // 正确，此时 Dad 的公有成员可以被访问   
    }
};

class GrandPriSon : public PriSon
{
    void func(void)
    {
        PriSon *p;
        Dad *d = dynamic_cast<Dad*>(p); // 错误，此时 PriSon 中 Dad 的公有成员不可以被访问  
    }
};
```

### 友元与继承

友元关系不具有继承性

但最奇怪的是：基类 的友元可以访问 基类 的保护成员，**即使对于派生类的基类部分也是如此**

```cpp
class Pal;

class Dad
{
    friend class Pal;
protected:
    int proMem;
};

class Son : public Dad
{
private:
    int sonPriMem;
};

class Pal
{
    void f0(void)
    {
        Dad d;
        d.proMem; // 正确，基类 的友元可以访问 基类 的保护成员
    }

    void f1(void)
    {
        Son s;
        // 友元关系不具有继承性
        // 错误，基类 的友元不能访问派生类的私有成员
        s.sonPriMem; 
    }

    void f2(void)
    {
        Son s;
        s.proMem; // 正确，基类 的友元可以访问 基类 的保护成员，即使对于派生类的基类部分也是如此
    }
};
```

简单来说，**每个类控制自己的访问权限**

### 改变个别成员的可访问性

可以在派生类中使用 using 声明来改变个别成员的可访问性

更改后的访问权限由该 **using 声明前的访问说明符** 来决定

条件：该成员可以被该派生类访问

例子：

```cpp
class Dad
{
private:
    int priMem;
protected:
    int proMem;
public:
    int pubMem;
};

// 私有继承
// 理论上，priMem、proMem、pubMem 在 Son 类都是私有成员
class Son : private Dad
{
protected:
    using Dad::proMem; // 现在 proMem 在 Son 的访问权限为 protected
public:
    using Dad::pubMem; // 现在 pubMem 在 Son 的访问权限为 public
    using Dad::priMem; // 错误，派生类不可以访问基类的私有成员 priMem 
};

class Grandson : public Son
{
    void func()
    {
        // 正确，Grandson 的直接基类部分的保护成员 proMem 可以被派生类的成员函数访问
        this->proMem = 1;
    }
};

void f0(void)
{
   Son s;
   s.pubMem = 1; // 正确
}
```

### struct 与 class 的区别（两点）

## 15.6

派生类的类作用域 *嵌套* 在基类的类作用域

### 名字查找与继承

C++ 对函数的解析过程：

* 确定对象 obj 的 **静态类型**
* 在 obj 对应的类中寻找成员 mem，如果没找到，继续在直接基类中查找，直到查找成功，或者到达继承链的顶端
* 如果查找失败，抛出错误
* 如果查找成功，*进行类型检查*，判断调用 mem 是否合法
* 如果合法，将根据调用的是否是虚函数而产生不同的代码：

    * 如果是虚函数，并且 obj 是指针或引用类型，那么产生的代码将在运行时依据 obj 的动态类型，确定到底运行 mem 的哪个版本
    * 如果不是虚函数，则产生一个常规的函数调用

例如：

```cpp
Bulk_quote bulk;
std::cout << bulk.isbn() << std::endl;
```

对 isbn 函数的解析过程如下：

* 在 Bulk_quote 的类作用域查找是否有 isbn 这个成员函数，没有
* 由于 Bulk_quote 继承自 Disc_quote，在 Disc_quote 的类作用域查找是否有 isbn 这个成员函数，没有
* 由于 Disc_quote 继承自 Quote，在 Quote 的类作用域查找是否有 isbn 这个成员函数，有
* 进行类型检查，通过

最终，对 isbn 的调用确定为 Quote::isbn()

### 在编译时进行名字查找

即使动态类型与静态类型可能不一致，我们能使用的成员也 **只由静态类型决定：**

```cpp
class Dad {};
class Son : public Dad
{
    void func(void);
};

Son s;
Dad &d = s;
d.func(); // 错误
```

```d.func()``` 错误的原因是：虽然 Son 类确实有一个名叫 func 的成员函数，但它对 Dad 类型的对象是不可见的，因为 Dad 显然不含有名叫 func 的成员函数

### 名字冲突与继承

派生类成员将隐藏基类同名成员

因此，**除了覆盖虚析构函数之外**，派生类尽量不要定义与某个基类成员相同的成员

### 名字查找优于类型检查

对于下面这个例子：

```cpp
class Dad
{
public:
    virtual void f0(void);
    void f1(void);
};

class Son : public Dad
{
public:
    void f0(int); // 隐藏基类的虚函数 f0
    void f1(int); // 隐藏基类的普通成员函数 f1
};

Son s;
s.f0(); // 错误
s.Dad::f0();

s.f1(); // 错误
s.Dad::f1(); 
```

对于 f0 的调用，先在 Son 的类作用域查找 f0，有，**立即停止搜索**

因此，```s.f0();``` 实际调用的是 ```Son::f0(int);```，由于需要一个 int 类型的实参，因此，调用错误

对于 f1 的调用也是同样的道理

```cpp
Son s;
Dad &rd = s;
rd.f0(); // 正确，调用 Dad::f0()，因为 Son 没有覆盖 Dad 的虚函数 f0
```

### 通过基类调用隐藏的虚函数

primer 550 页

### 覆盖重载的函数

派生类可以覆盖基类重载虚函数的 0 个或多个实例，**如果派生类希望所有重载版本对它的用户都可见，那么它必须覆盖所有的重载版本，或者一个都不覆盖**

```cpp
struct Dad
{
    virtual void func(void)
    {
        std::cout << "Dad::func(void)" << std::endl;
    }

    virtual void func(int)
    {
        std::cout << "Dad::func(int)" << std::endl;
    }

    virtual void func(int, int)
    {
        std::cout << "Dad::func(int, int)" << std::endl;
    }

    virtual void func(int, int, std::string)
    {
        std::cout << "Dad::func(int, int, std::string)" << std::endl;
    }
};

struct Son : Dad
{
    
};

void test(void)
{
    Son s;
    s.func();
    s.func(1);
    s.func(1, 1);
    s.func(1, 1, "");
}
```

test 中的调用都是正确的，因为 Son 类没有定义任何基类的虚函数，因此，调用的都是基类的版本

实际上，派生类仅仅定义其中一些重载的基类虚函数是十分常见的行为：

```cpp
struct Son : Dad
{
    void func(int) override
    {
        std::cout << "Son::func(int) override" << std::endl;
    }
};
```

但如果仅定义一些重载函数，基类部分的其它重载函数对用户就不可见了：

```cpp
void test(void)
{
    Son s;
    s.func(); // 错误
    s.func(1); // 正确
    s.func(1, 1); // 错误
    s.func(1, 1, ""); // 正确
}
```

我们当然可以通过将所有实例全部再定义一遍来解决这个问题，然而，这十分麻烦

这时，可以使用 using 声明解决这个问题：

```cpp
struct Son : Dad
{
    using Dad::func();

    void func(int) override
    {
        std::cout << "Son::func(int) override" << std::endl;
    }
};
```

这样，test 中的所有调用均正常，输出如下：

```
Dad::func(void)
Son::func(int) override
Dad::func(int, int)
Dad::func(int, int, std::string)
```

**注意：** 

* using 声明语句指定一个名字，**不指定形参列表**
* 基类函数的每一个实例 **必须** 对派生类 **可见**

## 15.7

### 为什么在基类定义了虚析构函数，派生类的析构函数就能正确执行？

我们知道：和其它虚函数一样，析构函数的虚属性也会被继承

结合之前 C++ 对于函数名字查找的过程可知：

如果基类 **没有** 定义虚析构函数，那么，删除指向一个派生类的基类指针，过程如下：

* 在 Base 的类作用域查找 虚构函数，找到
* 进行类型检查，通过
* 确定是否为虚函数，**不是**，在编译时确定函数调用结果，即调用 ```Base::~Base```

因此，子类的虚构函数就将无法调用，因为它根本就不会执行

然而，如果基类 **定义了** 虚析构函数，那么，删除指向一个派生类的基类指针，过程如下：

* 在 Base 的类作用域查找 虚构函数，找到
* 进行类型检查，通过
* 确定是否为虚函数，**是**，在 **运行时** 确定函数调用结果

运行时，发现该基类指针实际上是指向的一个派生类对象（假设为 D），那么，析构函数的调用将在运行时被确定为 ```D::~D()```，析构 D 时，先执行析构函数的函数体，再析构它的 *直接基类*

举个例子：

```cpp
struct Base
{
    virtual ~Base()
    {
        std::cout << "Calling Base::~Base()" << std::endl;
    }
};

struct D : Base
{
    ~D()
    {
        std::cout << "Calling D::~D()" << std::endl;
    }
};

struct DD : D
{
    ~DD()
    {
        std::cout << "Calling DD::~DD()" << std::endl;
    }
};

void test(void)
{
    Base *d0 = new DD;
    delete d0;
    std::cout << "-------------------" << std::endl;
    D *d1 = new DD;
    delete d1;
    std::cout << "-------------------" << std::endl;
    DD *d2 = new DD;
    delete d2;
}
```

**输出：**

```
Calling DD::~DD()
Calling D::~D()
Calling Base::~Base()
-------------------
Calling DD::~DD()
Calling D::~D()
Calling Base::~Base()
-------------------
Calling DD::~DD()
Calling D::~D()
Calling Base::~Base()
```

可以发现，不管删除哪个类型的指针，析构函数都能正确执行

那么，如果将 Base 的虚构函数改为 **普通** 析构函数，并将 D 的析构函数声明为 virtual 的，执行结果是什么捏？

```
Calling Base::~Base()
-------------------
Calling DD::~DD()
Calling D::~D()
Calling Base::~Base()
-------------------
Calling DD::~DD()
Calling D::~D()
Calling Base::~Base()
```

可以发现，删除 Base 类型的指针，析构函数并没有正确执行；而删除 D 和 DD 类型的指针，析构函数可以正确执行

请结合最开始分析的析构函数的调用过程来弄清楚为什么结果是这样

### 分割线    

1. 如果基类的默认构造函数、默认拷贝构造函数、拷贝赋值运算符、析构函数是删除的或者不可访问的，那么，派生类对应的成员将是删除的，因为派生类无法构造、拷贝、析构其基类部分
2. 由于基类通常需要定义虚析构函数，因此，基类通常不包含默认的移动操作，自然，派生类也不包含默认的移动操作。

    如果确实需要移动操作，应该在基类先定义移动操作（可以是 default 的），这样只要派生类没有排斥移动的成员，它将获得合成的移动操作


3. 基类的构造函数负责初始化基类部分，因此，如果我们要拷贝（移动）一个派生类对象，必须在派生类的拷贝（移动）构造函数的初始值列表 **显式使用基类的拷贝（移动）构造函数**
4. 同样地，派生类的拷贝（移动）赋值也应该遵循上述操作：

    ```cpp
    D& operator=(const D &d)
    {
        Base::operator=(d); // 拷贝基类部分

        if(this->data != d.data) // 处理派生类成员自赋值
        {
            delete this->data;
            this->data = new int(*d.data);
        }
        
        return *this;
    }
    ```

### 在构造函数和析构函数中调用虚函数

如我们所知，在实例化一个派生类对象时，先调用基类的构造函数（初始化基类部分），再执行派生类的构造函数以及函数体部分

因此，在调用基类的构造函数时，派生类对象（D）处于一种 *未完成* 状态

如果此时基类的构造函数「间接调用」的虚函数是派生类版本的话，那就可能访问派生类部分的成员，而这部分成员此时是没有被初始化的，因此，十分危险！

为了正确处理这种未完成状态，**编译器会认为对象的类型仿佛在构造（析构）的过程发生了改变一样**，也就是说，当我们创建一个对象时，需要 **将对象的类型和此时构造函数的类型看成同一个**

这种规则不仅对直接调用虚函数有效，对「间接调用」也是有效的，这里的「间接调用」指在构造函数和析构函数中调用另一个函数

看下面的一个例子：

```cpp
struct Base
{
    Base()
    {
        print();
    }

    virtual void print(void)
    {
        std::cout << "Calling void Base::print()" << std::endl;
    }
    virtual ~Base(){}
};

struct D : Base
{
    int *data = nullptr;
    D()
        :Base(), data(new int(114514))
    {
        print();
    }
    void print() override
    {
        std::cout << "Calling void D::print()" << std::endl;
        std::cout << *data << std::endl;
    }
    ~D();
};
```

当我们创建一个 D 类对象时，输出结果如下：

```
Calling void Base::print()
Calling void D::print()
114514
```

也就是说，在执行基类的构造函数部分时，编译器认为 D 类的类型仿佛变成了 Base 类型，调用的是基类版本的 print，这种间接调用是安全的

对于析构函数，也是一个道理

### 继承的构造函数

1. 使用 using 声明来继承基类的构造函数
2. using 声明不包含 explicit、constexpr 关键字
3. explicit、constexpr 属性 **会** 被继承
4. 两个例外：

    * 派生类可以继承一部分构造函数，而为其它构造函数定义自己的版本，但自己定义的构造函数版本的参数列表 **必须** 与基类相同
    * 默认构造函数、拷贝构造函数、移动构造函数 **不会** 被继承

```cpp
struct Base
{
    Base()
    {
        std::cout << "Calling Base::Base()" << std::endl;
    }
    explicit Base(int)
    {
        std::cout << "Calling Base::Base(int)" << std::endl;
    }
    Base(std::string s0, std::string s1 = "default")
    {
        std::cout << "Calling Base::Base(std::string, std::string)" << std::endl;
    }
    Base(int, int)
    {
        std::cout << "Calling Base::Base(int, int)" << std::endl;
    }
};

struct D : Base
{
    int val = 1;

    // using explicit Base::Base; // 错误，using 声明不能指定 explicit 和 constexpr
    using Base::Base;

    // D() = default; 默认构造函数不会被继承
    // D(const D &) = default; 拷贝构造函数不会被继承
    // D(D &&) = default; 移动构造函数不会被继承

    // 继承的构造函数等价于：
    // D(int v0, int v1)
    //     :Base(v0, v1){}

    // 如果派生类定义的构造函数与基类有相同的参数列表，该构造函数将不会继承
    D(int, int)
    {
        std::cout << "Calling D::D(int, int)" << std::endl;
    }

};

void test(void)
{
    D d;
    std::cout << "---------------------" << std::endl;

    // D d1 = 1; // 错误，继承下来的构造函数保留 explicit 和 constexpr 的属性
    D d1(1);
    std::cout << "---------------------" << std::endl;

    D d2("parm0"); // D::D(std::string)
    D d3("parm0", "parm1"); // D::D(std::string, std::string)
    std::cout << "---------------------" << std::endl;

    D d4(1, 1); // Calling D::D(int, int)
    std::cout << "---------------------" << std::endl;


}
```

## 15.8

### 容器与继承

1. 当派生类对象被赋值给基类对象，其派生类部分被切掉，因此，**容器与存在继承关系的类型无法兼容**
2. 在容器中存放（智能）指针，而非对象：

    ```cpp
    void test5(void)
    {
        std::vector<std::shared_ptr<Quote>> data;
        data.push_back(std::make_shared<Quote>());
        data.push_back(std::make_shared<Bulk_quote>("114514", 101, 2, 0.7));
        std::for_each(data.begin(), data.end(), [&](std::shared_ptr<Quote> q)
        {
            q->debug();
        });
    }
    ```
 

 