# 友元

为了让一个类或者一个函数能够**访问另一个类的私有成员**，这时，需要用到友元

## 使用场景

一般来说，友元有三种使用场景

* 全局函数做友元
* 类做友元
* 成员函数做友元

## 全局函数做友元

先给出阿伟类的定义

~~~C++
class Wei
{
public:
    std::string clothes;
private:
    std::string pants; // 阿伟不希望每个人都能访问他的胖次
    std::string schoolAddress; // 阿伟不希望陌生人能知道他的学校在哪
    int length; // 阿伟不希望每个人都能访问他的长度
public:
    Wei()
        :clothes("阿伟的衣服"), pants("阿伟的胖次"), schoolAddress("???"), length(0){}
};

~~~

现在，杰哥想要访问阿伟
~~~C++
void letMeSeeSee(const Wei &wei)
{
    std::cout << "杰哥正在访问" << wei.clothes << std::endl;
    std::cout << "杰哥正在访问" << wei.pants << std::endl;
}
~~~

此时，杰哥发现他无法访问阿伟的胖次，于是，他使用了某种技能，强制让 ```void letMeSeeSee(const Wei &wei)``` 可以访问阿伟的胖次

杰哥是怎么做的呢？

他在阿伟类中，让 ```void letMeSeeSee(const Wei &wei)``` 这个全局函数作为友元，就像这样

~~~C++
class Wei
{
    friend void letMeSeeSee(const Wei &wei); // 全局函数做友元
public:
    ......
};
~~~

也就是说，全局函数做友元，只需要**在需要访问的类中添加全局函数的声明，并加上 ```friend``` 关键字即可**

现在，杰哥可以愉快的访问阿伟的胖次了

## 类做友元

仅仅让全局函数访问阿伟的胖次，已经不能满足杰哥了，杰哥现在想让他的所有成员函数都能访问阿伟的胖次（比如，```登dua郎函数``` 需要得知阿伟是否发育正常），他该怎么做呢？

这就需要类做友元的技术了

同样地，给出杰哥类

~~~C++
class JieGe
{
public:
    void letMeSeeSee(const Wei &wei); // “让我看看” 的函数声明，类外实现
    void dengDuaLang(const Wei &wei); // “登dua郎” 的函数声明，类外实现
};
~~~

再给出上述函数的实现

~~~C++
void JieGe::letMeSeeSee(const Wei &wei)
{
    std::cout << "杰哥正在访问" << wei.clothes << std::endl;
    std::cout << "杰哥正在访问" << wei.pants << std::endl;

}
void JieGe::dengDuaLang(const Wei &wei)
{
    std::cout << "杰哥正在访问阿伟的长度" << std::endl;
    std::cout << "阿伟的长度是：" << wei.length << std::endl;
}
~~~

为了让上述两个成员函数都能访问阿伟的私有成员，杰哥需要让自己成为阿伟的“好朋友”

就像这样：

~~~C++
class Wei
{
    friend class JieGe; // 杰哥强制让其变成阿伟的好朋友
public:
    ...
};
~~~

也就是说：**类做友元，只需要在被访问的类中添加类的声明，并加上 ```friend``` 关键字即可**

## 成员函数做友元

经过一夜，杰哥累了，现在，他不想访问阿伟的胖次，也不想教阿伟登dua郎了，他**只想知道阿伟的学校在哪里**，他该怎么做呢？

利用成员函数做友元的技术

与全局函数做友元类似，成员函数做友元**只需要在被访问的类中加入成员函数的声明，并加上 ```friend``` 关键字即可**

就像这样

~~~C++
class Wei
{
    friend void JieGe::getSchoolAddress(const Wei &wei);
public:
    ......
};
~~~

**注意：** 与前面两种方式不同，**成员函数做友元需保证在被访问类（阿伟类）的前面，需要有该类（杰哥类）的实现**

即：

~~~C++
class Wei;
// 杰哥类
class JieGe
{
public:
    // void letMeSeeSee(const Wei &wei); 
    // void dengDuaLang(const Wei &wei); 
    void getSchoolAddress(const Wei &wei); // “获得学校地址” 的函数声明，类外实现
}; 
// 阿伟类，前面有杰哥类的定义
class Wei
{
    friend void JieGe::getSchoolAddress(const Wei &wei);
public:
    ......
};
~~~

否则，成员函数仍无法访问被访问类的私有成员

## 最终实现代码

### 全局函数做友元

~~~C++
class Wei
{
    friend void letMeSeeSee(const Wei &wei); // 全局函数做友元
public:
    std::string clothes;
private:
    std::string pants; // 阿伟不希望每个人都能访问他的胖次
    std::string schoolAddress; // 阿伟不希望陌生人能知道他的学校在哪
    int length; // 阿伟不希望每个人都能访问他的长度
public:
    Wei()
        :clothes("阿伟的衣服"), pants("阿伟的胖次"), schoolAddress("???"), length(0){}
};
void letMeSeeSee(const Wei &wei)
{
    std::cout << "杰哥正在访问" << wei.clothes << std::endl;
    std::cout << "杰哥正在访问" << wei.pants << std::endl;
}
int main(void)
{
    Wei wei;
    letMeSeeSee(wei);
}
~~~

### 类做友元

~~~C++
class Wei
{
    friend class JieGe; // 全局函数做友元
public:
    ......
};
class JieGe
{
public:
    void letMeSeeSee(const Wei &wei); // “让我看看” 的函数声明，类外实现
    void dengDuaLang(const Wei &wei); // “登dua郎” 的函数声明，类外实现
};
void JieGe::letMeSeeSee(const Wei &wei)
{
    std::cout << "杰哥正在访问" << wei.clothes << std::endl;
    std::cout << "杰哥正在访问" << wei.pants << std::endl;
}
void JieGe::dengDuaLang(const Wei &wei)
{
    std::cout << "杰哥正在访问阿伟的长度" << std::endl;
    std::cout << "阿伟的长度是：" << wei.length << std::endl;
}
int main(void)
{
    Wei wei;
    JieGe jie;
    jie.letMeSeeSee(wei);
    jie.dengDuaLang(wei);
}
~~~

### 成员函数做友元

~~~C++
class Wei;
// 杰哥类
class JieGe
{
public:
    // void letMeSeeSee(const Wei &wei); 
    // void dengDuaLang(const Wei &wei); 
    void getSchoolAddress(const Wei &wei); // “获得学校地址” 的函数声明，类外实现
}; 
// 阿伟类，前面有杰哥类的定义
class Wei
{
    friend void JieGe::getSchoolAddress(const Wei &wei);
public:
    ......
};

void JieGe::getSchoolAddress(const Wei &wei)
{
    std::cout << "阿伟的学校在：" << wei.schoolAddress <<std::endl;
}

int main(void)
{
    Wei wei;
    JieGe jie;
    jie.getSchoolAddress(wei);
}
~~~

## That's All