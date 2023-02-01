# C++对象模型和 this 指针
<!-- ~~~``` -->
## 一、成员函数和成员属性分开存储

我们先来看看**空类**的空间占用
~~~C++
class Test
{
    
};
int main(void)
{
    Test t;
    std::cout << sizeof(t) << std::endl;
}
~~~

执行结果为
~~~
1
~~~

为什么**空类的空间占用是一个字节** ？因为为了区分每一个对象，需要确保每一个对象在内存地址上的唯一性，因此，最少需要占用一个字节的空间才能保证唯一性

现在，我们往 Test 类添加一些属性
~~~C++
class Test
{
    int val;
};
~~~

此时，Test 类的空间占用为 ``` 4 ``` 

为什么不是 5 ？

因为只要类中有成员属性，就能确保对象之间的唯一性，因此，占用 4 个字节已经足以区分，并且满足一个整形数据所必须占用的空间

现在，我们添加一个成员函数
~~~C++
class Test
{
    int val;
    void func(void){}
};
~~~

此时，Test 类的空间占用仍为 ``` 4 ``` 

由此我们可以看出：**成员函数与成员属性分开存储**

并且，所有对象共享同一个成员函数，也就是说，在内存中，**一个非静态成员函数只有一个拷贝**

那成员函数依靠什么来区分不同的对象呢？

```this``` 指针！

## 二、this 指针

### 1. this 指针的定义

>this指针是类的一个**自动生成**、**自动隐藏**的**私有成员**，它存在于类的**非静态成员函数**中，指向被调用函数所在的对象。全局仅有一个 this 指针，当一个对象被创建时，this 指针就**存放指向对象数据的首地址**。

### 2. this 指针的用途

从定义可以看出：**this 指针指向被调用的成员函数的所属对象**

因此，this 指针可以帮助成员函数区分不同的对象！

此外，this 指针还有以下两个重要用途

#### **1. 当形参与成员属性同名时，可以用 this 指针区分**

举个例子：

~~~C++
class Test
{
    int val;
public:
    Test(int val)
    {
        this->val = val;
    }
};
~~~
在构造函数中，形参与成员属性发生重名，此时，利用 this 指针，帮助区分

**注意：** 不能写 ```val = val``` ，这句话并不会给成员属性赋值

#### **2. 在类的非静态成员函数中，返回对象本身**

举个例子：
~~~C++
class Test
{
public:
    int val;
    Test(int val)
    {
        this->val = val;
    }
    Test & addVal(const Test &t1)
    {
        val += t1.val;
        return *this;
    }
};
~~~

```addVal``` 函数返回值是对象本身，利用这个特性，我们就能实现**链式编程**的思想

例如：
~~~C++
int main(void)
{
    Test t0(10);
    Test t1(10);
    t0.addVal(t1).addVal(t1).addVal(t1);
    std::cout << t0.val << std::endl;
}
~~~

执行结果为 ```40``` 

值得注意的是：实现 ```addVal``` 函数时，是以引用的形式返回的

为什么？

事实上，如果以值传递的形式返回，编译器**会拷贝一个新的对象返回**，这样，下一次累加的对象就不是原来的 t0 ，而是一个新的对象

我们还是结合代码理解
~~~C++
Test & addVal(const Test &t1)
{
    std::cout << (int*)this << std::endl;
    val += t1.val;
    return *this;
}
~~~

加了引用，执行结果：
~~~
0x618fdff9ac
0x618fdff9ac
0x618fdff9ac
40
~~~

不加引用，即，```Test addVal(const Test &t1)```，执行结果：
~~~
0x1009bff824
0x1009bff82c
0x1009bff828
20
~~~

可以看到，不加引用，采用值传递的形式返回，每一次调用 ```addVal``` 函数，this 的值都不一样，也就是累加的对象不一样，自然不能实现多次累加

## 三、空指针访问成员

当对象为指针类型，且为空指针（nullptr），仍然可以访问成员。然而，需要特别注意：会不会涉及到 this 指针的访问

举个例子

~~~C++
class Test
{
    int val;
public:
    Test(int val)
    {
        this->val = val;
    }
    void Print(void)
    {
        std::cout << "Calling 'print' function" << std::endl;
    }
    int getVal(void)
    {
        return val;
    }
};
int main(void)
{
    Test *t0 = nullptr;
    t0->Print();
}
~~~

上述代码可以正确执行，因为没有涉及到 this 指针的访问

接下来，我们来尝试访问 this 指针，看看会怎样

~~~C++
int main(void)
{
    Test *t0 = nullptr;
    t0->getVal();
}
~~~

自然报错

![](屏幕截图%202023-01-19%20105324.png)

因为，此时的 this 指针为空，我们访问 val，实际上访问的是 this->val

为了保证代码的**健壮性**，需要在可能访问空指针的地方加一个判断
~~~C++
int getVal(void)
{
    if(this) return val;
    std::cout << "'this' is a null pointer!" << std::endl;
    return -1;
}
~~~

## Const 修饰成员函数

### 常函数

在成员函数后面加上一个 "const" ，这样的成员函数称为常函数

* 常函数内**不能修改**成员属性
* 如果确实要在常函数内修改成员属性，可以在成员属性前加上 ```mutable``` 关键字

举个例子，例如，我们不希望在 ```print``` 函数里修改成员属性，就可以在  ```print``` 函数后加上  ```const``` 关键字

~~~C++
void Print(void) const
{
    if(this)
        std::cout << "Her age is " << age << "." << std::endl;
}
~~~

当然，如果我们确实又想修改成员属性，可以在成员属性前加上 ```mutable``` 关键字比如，一个女生不想让别人知道她的真实年龄

~~~C++
class Test
{
    mutable int age;
public:
    Test()
    :age(100){}
    Test(int age)
    :age(age){}
    void Print(void) const
    {
        if(this)
        {
            age = 18;
            std::cout << "Her age is " << age << "." << std::endl;
        }
    }
};
int main(void)
{
    Test t0(35);
    t0.Print();
}
~~~

这样，就可以神不知鬼不觉地修改她的年龄为 18 岁了

### 常对象

在实例化对象时，加上 ```const``` 关键字，就可以实例化一个常对象

~~~C++
const Test t0(18);
~~~

需要注意的是，**常对象只能调用常函数**，因为**只有常函数才能够保证不会修改成员属性**，其他成员函数并不能保证不会修改成员属性
