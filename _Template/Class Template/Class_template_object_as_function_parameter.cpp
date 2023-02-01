#include <iostream>
#include <string>
/**
 *  @brief 模板类 Person 
 *
 *  @ingroup sequences
 *
 *  @tparam nameType  Type of name
 *  @tparam ageType   Type of age, default to int
 */
template <class nameType, class ageType = int>
class Person
{
    nameType name;
    ageType age;
public:
    Person(nameType name, ageType age)
        : name(name), age(age) {}
    void showPerson(void) const
    {
        std::cout << "name: " << name << ", age: " << age << std::endl;
    }
};

// 类模板作为函数参数

// 方式 1
// 指定传入类型
void func0(const Person<std::string, int> &p)
{
    p.showPerson();
}

// 方式 2
// 参数模板化
template <class nameType, class ageType>
void func1(const Person<nameType, ageType> &p)
{
    p.showPerson();
}

// 方式 3
// 整个类模板化
template <class className>
void func2(const className &p)
{
    p.showPerson();
}

int main(void)
{
    Person<std::string> p("DingZheng", 18);
    func0(p);
    func1(p);
    func2(p);
}