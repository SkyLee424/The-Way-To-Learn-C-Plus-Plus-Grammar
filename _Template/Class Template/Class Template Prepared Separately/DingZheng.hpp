#pragma once
#include<iostream>

// 4
template<class nameType, class ageType>
class DingZheng;

// 3
template <typename nameType, class ageType>
void showPerson1(const DingZheng<nameType, ageType> &p);

template<class nameType, class ageType = int>
class DingZheng
{
    nameType name;
    ageType age;
    friend void showPerson1<>(const DingZheng<nameType, ageType> &p); // 2
public:
    DingZheng(nameType name, ageType age);
    void show(void);
};

// 声明与实现均在 hpp 文件中

template<class nameType, class ageType>
DingZheng<nameType, ageType>::DingZheng(nameType name, ageType age)
{
    this->name = name;
    this->age = age;
}

template<class nameType, class ageType>
void DingZheng<nameType, ageType>::show(void)
{
    std::cout << "name: " << name << ", age: " << age << std::endl;
    std::cout << "like RuiKe V" << std::endl;
}

// 1
template <typename nameType, class ageType>
void showPerson1(const DingZheng<nameType, ageType> &p)
{
    std::cout << "Out of class_______________name: " << p.name << ", age: " << p.age << std::endl; 
}