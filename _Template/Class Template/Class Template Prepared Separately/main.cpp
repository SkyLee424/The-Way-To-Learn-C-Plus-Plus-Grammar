// 由于类模板成员函数创建时机是调用时
// 因此，在编译的链接阶段，不会被创建
// 分文件编写时，如果调用类模板成员函数，就会出现问题
// 解决方式：
// 1. 直接包含源文件
// #include"DingZheng.cpp"
// 2. 将声明与实现写在同一文件，并将文件后缀名命名为.hpp
#include"DingZheng.hpp"
int main(void)
{
    DingZheng<const char*> d("DingZheng", 18);
    d.show();
    showPerson1(d);
}