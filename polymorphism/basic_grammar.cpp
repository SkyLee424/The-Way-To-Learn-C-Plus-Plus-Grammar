#include<iostream>
class Animal
{
public:
    // 父类函数为虚函数
    virtual void speak(void)
    {
        std::cout << "动物在说话" << std::endl;
    }
};

class XueBao : public Animal
{
public:
    // 子类需重写父类的虚函数
    // 重写：返回值、函数名、形参列表完全相同
    // 例外：返回的是对象本身
    void speak(void)
    {
        std::cout << "雪豹在说话" << std::endl;
    }
};

class DingZheng : public Animal
{
public:
    void speak(void)
    {
        std::cout << "丁真在说藏话" << std::endl;
    }
};


// 传引用或指针
void letAnimalSpeak(Animal &animal)
{
    animal.speak();
}

void test(void)
{
    XueBao xueBao;
    letAnimalSpeak(xueBao);
    DingZheng dingZheng;
    letAnimalSpeak(dingZheng);
}

int main(void)
{
    test();
}