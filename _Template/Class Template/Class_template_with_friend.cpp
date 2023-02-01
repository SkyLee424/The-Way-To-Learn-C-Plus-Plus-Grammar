#include<iostream>
#include<string>

// 3. 由于在 showPerson1 中使用了 Person，因此，需要在前面加上 Person 的声明
template <class nameType, class ageType>
class Person;

// 2. 在模板类前加上该函数的声明，否则编译器仍找不到 showPerson1

template <typename nameType, class ageType>
void showPerson1(const Person<nameType, ageType> &p);

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
    // 方式 1：全局函数类内实现
    friend void showPerson0(const Person<nameType, ageType> &p)
    {
        std::cout << "name: " << p.name << ", age: " << p.age << std::endl; 
    }
    // 方式 2：全局函数类外实现
    // friend void showPerson1(const Person<nameType, ageType> &p); // 这样写，showPerson1 相当于普通函数，不是模板函数了
    // 1. 要在函数名后面加上空模板参数列表，以表明其为模板函数
    friend void showPerson1<>(const Person<nameType, ageType> &p);
public:
    Person(nameType name, ageType age)
        : name(name), age(age) {}

private:
    nameType name;
    ageType age;
};

template <typename nameType, class ageType>
void showPerson1(const Person<nameType, ageType> &p)
{
    std::cout << "Out of class_______________name: " << p.name << ", age: " << p.age << std::endl; 
}

int main(void)
{
    Person<std::string, int> p0("DingZheng", 18);
    showPerson0(p0);
    showPerson1(p0);
}
