#include"namespace.h"
// 未命名的 namespace
namespace
{
    int Static = 1; // 相当于静态全局变量
}
int main(void)
{
    test::print();
    /*...............................................................*/
    // 导入 test 局部 namespace
    using test::a;
    using test::print1;
    // int a = 100; 二义性
    std::cout << a << std::endl;
    print1();
    /*................................................................*/
    {
        // 导入 test 整个 namespace，将using指令局限在很小的作用域中是一个好习惯
        using namespace test;
        std::cout << b << std::endl;
    }
    {
        using namespace test1;
        std::cout << b << std::endl;
    }
    /*................................................................*/
    test1::test2::print2(); // 嵌套的namespace
    // using namespace test1;
    // test2::print2();
    /*................................................................*/
    {
        namespace imTestToo = test; // 起别名，不要在头文件起别名，易冲突
        imTestToo::print2();
    }
    /*................................................................*/
    Static = 2;
    std::cout << Static << std::endl;
}