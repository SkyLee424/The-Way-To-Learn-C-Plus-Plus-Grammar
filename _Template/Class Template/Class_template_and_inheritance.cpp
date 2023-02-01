#include <iostream>
#include <string>
template<class Type>
class Dad 
{
public:
    Type name;
};

// 当父类为模板类时，子类继承时
// 需指定父类的模板参数

// class Son0 : public Dad // 错误，未指定父类的模板参数

// 方式 1，子类直接指定父类的模板参数
class Son0 : public Dad <const char*>  
{

};

// 方式 2：子类也为模板类
template <class TypeOfSon, class TypeOfDad>
class Son1 : public Dad<TypeOfDad>
{
public:
    TypeOfSon name;
};

int main(void)
{
    Son1<const char*, std::string> s1;
    std::cout << typeid(s1.Dad::name).name() << std::endl;
    std::cout << typeid(s1.name).name() << std::endl;
}