#include <iostream>
// 虚析构与纯虚析构
// 继承层次关系的父类应该定义虚函数性质的析构函数，即使这个析构函数不做任何的事情。
class Animal
{
public:
    Animal()
    {
        std::cout << "Calling Animal's constructor!" << std::endl;
    }
    // ~Animal()
    // {
    //     std::cout << "Calling Animal's destructor!" << std::endl;
    // }
    // 如果父类不提供虚析构函数或者纯虚析构函数
    // 就无法调用子类的虚构函数
    // 此时，若子类含有在堆区开辟的成员
    // 就会造成内存泄露

    // 虚析构函数
    virtual ~Animal()
    {
        std::cout << "Calling Animal's virtual destructor!" << std::endl;
    }

    // 纯虚析构函数
    // virtual ~Animal() = 0;

    virtual void speak(void) = 0;
};

// 类外给出具体实现（可以是空实现）
// Animal::~Animal()
// {
//     std::cout << "Calling Animal's virtual destructor!" << std::endl;
// }

// 子类含有在堆区开辟的成员
class Cat : public Animal
{
    std::string *name;

public:
    Cat(std::string name)
    {
        std::cout << "Calling Cat's constructor!" << std::endl;
        this->name = new std::string(name);
    }
    ~Cat()
    {
        std::cout << "Calling Cat's destructor!" << std::endl;
        delete name;
        name = nullptr;
    }
    void speak(void)
    {
        std::cout << *name << " cat is talking right now!" << std::endl;
    }
};

void test(void)
{
    Animal *animal = new Cat("Tom");
    animal->speak();
    delete animal;
    animal = nullptr;
}

int main(void)
{
    test();
    // 父类不提供虚析构函数的输出：
    // Calling Animal's constructor!
    // Calling Cat's constructor!
    // Tom cat is talking right now!
    // Calling Animal's destructor!
    // 可以发现：没有调用 Cat 的析构函数
}