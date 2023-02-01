#include<iostream>
// 实现两个数的运算

// 抽象类
class Calculator
{
protected:
    double num0;
    double num1;

public:
    Calculator()
    : num0(0.0), num1(0.0) {}
    // 纯虚函数
    // 只要有纯虚函数，就是抽象类
    // 抽象类无法实例化对象
    virtual double calculate(void) = 0; // 告知编译器，在父类中，我无法给出 calculate 的具体实现
};

class Add : public Calculator
{
public:
    Add(double num0, double num1)
    {
        this->num0 = num0;
        this->num1 = num1;
    }
    // 重写父类中的纯虚函数，否则子类仍然是抽象类
    double calculate(void)
    {
        return num0 + num1;
    }
};

class Subtract : public Calculator
{
public:
    Subtract(double num0, double num1)
    {
        this->num0 = num0;
        this->num1 = num1;
    }
    double calculate(void)
    {
        return num0 - num1;
    }
};

class Multiply : public Calculator
{
public:
    Multiply(double num0, double num1)
    {
        this->num0 = num0;
        this->num1 = num1;
    }
    double calculate(void)
    {
        return num0 * num1;
    }
};

// 多态的优点
// 1. 结构清晰
// 2. 可读性高
// 3. 扩展性强、易于维护（想要扩展父类的功能，只需要再编写一个子类，无需修改之前的代码）

// 扩展一个除法类

class Divide : public Calculator
{
public:
    Divide(double num0, double num1)
    {
        this->num0 = num0;
        this->num1 = num1;
    }
    double calculate(void)
    {
        return num0 / num1;
    }
};

void calculating(Calculator *cal)
{
    // 一个接口，有多种形态，可以实现多种功能
    std::cout << cal->calculate() << std::endl;
    delete cal;
    cal = nullptr;
}

int main(void)
{
    double num0, num1;
    char op;
    std::cin >> num0 >> op >> num1;
    switch (op)
    {
    case '+':
    {
        calculating(new Add(num0, num1));
        break;
    }
    case '-':
    {
        calculating(new Subtract(num0, num1));
        break;
    }
    case '*':
    {
        calculating(new Multiply(num0, num1));
        break;
    }
    case '/':
    {
        calculating(new Divide(num0, num1));
        break;
    }
    default:
        std::cout << "Check Your Input!" << std::endl;
        break;
    }
}