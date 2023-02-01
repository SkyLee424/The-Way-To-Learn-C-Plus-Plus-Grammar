#include<iostream>
template<class nameType, class ageType = int>
class DingZheng
{
    nameType name;
    ageType age;
public:
    DingZheng(nameType name, ageType age);
    void show(void);
};

// 类模板成员函数类外实现

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

int main(void)
{
    DingZheng<const char*> d("DingZheng", 18);
    d.show();
}