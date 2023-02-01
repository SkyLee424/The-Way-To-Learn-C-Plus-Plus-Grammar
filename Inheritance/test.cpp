#include<iostream>
// // 父类
// class Dad
// {
// public:
//     int a;

// protected:
//     int b;

// private:
//     int c;

// public:
//     Dad()
//     {
//         std::cout << "Calling Dad's constructor function." << std::endl;
//     }
//     ~Dad()
//     {
//         std::cout << "Calling Dad's destructor function." << std::endl;
//     }
// };

// // 公共继承，不改变 Dad 属性的权限
// // 但无法访问 Dad 的私有成员
// class Son0 : public Dad
// {
// public:
//     Son0()
//     {
//         std::cout << "Calling Son0's constructor function." << std::endl;
//     }
//     ~Son0()
//     {
//         std::cout << "Calling Son0's destructor function." << std::endl;
//     }
//     void test(void)
//     {   
//         a = 10; // 仍为 public
//         b = 10; // 仍为 protected
//         // c = 10; // 无法访问 Dad 的私有成员
//     }
// };

// // 保护继承，继承后的属性均为 protected
// // 也不能访问 Dad 的私有成员
// class Son1 : protected Dad
// {
// public:
//     void test(void)
//     {
//         a = 10; // 变为 protected
//         b = 10; // 仍为 protected
//         // c = 10; // 无法访问 Dad 的私有成员
//     }
// };

// // 私有继承，继承后的属性均为 private
// // 也不能访问 Dad 的私有成员
// class Son2 : private Dad
// {
// public:
//     void test(void)
//     {
//         a = 10; // 变为 private
//         b = 10; // 仍为 private
//         // c = 10; // 无法访问 Dad 的私有成员
//     }
// };


// // 进一步验证了，私有继承，继承后的属性均为 private
// // 即使是公有继承 Son2，也无法访问 Son2 的成员属性
// // 因为 Son2 的成员属性均从 Dad 私有继承而来，权限均为私有
// class GrandSonOfSon2 : public Son2
// {
// public:
//     void test(void)
//     {
//         // a = 10; // 无法访问 Son2 的私有成员
//         // b = 10; // 无法访问 Son2 的私有成员
//         // c = 10; // 无法访问 Dad 的私有成员
//     }
// };


// class Verify
// {
// public:
//     // 验证公共继承
//     void test0(void)
//     {
//         Son0 s0;
//         s0.a = 0; // s0.a 在 Son0 中为公共成员属性
//         // s0.b = 0; // s0.b 在 Son0 中为保护成员属性，类外不可访问
//     }

//     // 验证保护继承
//     void test1(void)
//     {
//         Son1 s1;
//         // s1.a = 0; // s1.a 在 Son1 中为保护成员属性，类外不可访问
//         // s1.b = 0; // s1.b 在 Son1 中为保护成员属性，类外不可访问
//     }

//     // 虽然无法访问父类私有成员
//     // 但父类的私有成员也是被继承下来了的
//     // 只是编译器隐藏了，我们无法访问
//     void test2(void)
//     {
//         // 输出结果为 12（三个整形数据）
//         std::cout << "Sizeof Son0 = " << sizeof(Son0) << std::endl;
//     }

//     // 验证构造函数、析构函数的调用时机：
//     // 先调用父类的构造函数，再调用子类的构造函数（先有父亲再有儿子）
//     // 先调用子类的析构函数，再调用父类的析构函数（白发人送黑发人）
//     // 栈的 FILO 性质？
//     void test3(void)
//     {
//         Son0 s0;
//     }
// };



// int main(void)
// {
//     Verify verifiy;
//     int opinion;
//     std::cin >> opinion;
//     switch (opinion)
//     {
//     case 0:
//         verifiy.test0();
//         break;
//     case 1:
//         verifiy.test1();
//         break;
//     case 2:
//         verifiy.test2();
//         break;
//     case 3:
//         verifiy.test3();
//         break;
//     }
// }

/*验证父类子类同名成员的处理*/
class Dad
{
public:
    Dad()
        :a(114514){}
    int a;
    static int b;
    void func(void)
    {
        std::cout << "Calling Dad's void func(void)." << std::endl;
    }
    void func(int a)
    {
        std::cout << "Calling Dad's void func(int a)." << std::endl;
    }
    static void static_func(void)
    {
        std::cout << "Calling Dad's static void static_func(void)." << std::endl;
    }
    static void static_func(int a)
    {
        std::cout << "Calling Dad's static void static_func(int a)." << std::endl;
    }
};

int Dad::b = 114514; // 静态变量初始化

class Son : public Dad
{
public:
    Son()
        :a(2023){}
    int a;
    static int b;
    void func(void)
    {
        std::cout << "Calling Son's func(void)." << std::endl;
    }
    static void static_func(void)
    {
        std::cout << "Calling Son's static void static_func(void)." << std::endl;
    }
};

int Son::b = 2023;

// 验证 父类子类 非静态同名成员 的访问与调用
void test0(void)
{
    Son s;
    // 直接访问，访问的是子类中的 a
    std::cout << s.a << std::endl; 
    // 想要访问父类中的 a，需要加上作用域
    std::cout << s.Dad::a << std::endl;

    // 成员函数的调用也是一样的
    // 直接调用，调用的是子类中的 func
    s.func(); 
    // s.func(100); // 直接调用，就算有父类同名函数参数与子类不同，编译器也不认
    // 还得加上作用域
    s.Dad::func();
    s.Dad::func(100);
}

// 验证 父类子类 静态同名成员 的访问与调用
void test1(void)
{
    Son s;
    /*1. 通过对象访问*/
    // 直接访问，访问的是子类中的 b
    std::cout << s.b << std::endl;
    // 想要访问父类中的 b，需要加上作用域
    std::cout << s.Dad::b << std::endl;
    // 直接调用，调用的是子类中的 static_func
    s.static_func();
    // s.static_func(1); // 直接调用，就算有父类同名函数参数与子类不同，编译器也不认
    // 想要调用父类中的 static_func，需要加上作用域
    s.Dad::static_func();
    s.Dad::static_func(1);
    /*---------------------------------------------------------------------------*/
    /*2. 通过类名访问*/
    // 直接访问，访问的是子类中的 b
    std::cout << Son::b << std::endl;
    // 想要访问父类中的 b，需要加上作用域
    std::cout << Son::Dad::b << std::endl;
    // 直接调用，调用的是子类中的 static_func
    Son::static_func();
    // 想要调用父类中的 static_func，需要加上作用域
    Son::Dad::static_func();
    Son::Dad::static_func(1);
}

int main(void)
{
    // test0();
    // test1();
    std::cout << sizeof(int*) << std::endl;
}