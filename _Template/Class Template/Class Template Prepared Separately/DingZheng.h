#pragma once
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