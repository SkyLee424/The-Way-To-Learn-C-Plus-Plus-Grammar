#include<iostream>
class CPU
{
public:
    // 继承层次关系的父类应该定义虚函数性质的析构函数，即使这个析构函数不做任何的事情。
    virtual ~CPU(){}
    // 虚函数，由子类实现内部功能
    virtual void info(void) = 0;
};

class GraphicsCard
{
public:
    virtual ~GraphicsCard(){}
    virtual void info(void) = 0;
};

class Memory
{
public:
    virtual ~Memory(){}
    virtual void info(void) = 0;
};

// 子类

class I5_12400F : public CPU
{
public:
    void info(void)
    {
        std::cout << "I5_12400F" << std::endl;
    }
};

class Rx470 : public GraphicsCard
{
public:
    void info(void)
    {
        std::cout << "AMD Rx470" << std::endl;
    }
};

class TForce_Memory : public Memory
{
public:
    void info(void) 
    {
        std::cout << "TForce 3200MHz Memory" << std::endl;
    }
};

class R5_2600 : public CPU
{
public:
    void info(void)
    {
        std::cout << "R5_2600" << std::endl;
    }
};

class Rx560 : public GraphicsCard
{
public:
    void info(void)
    {
        std::cout << "Rx560" << std::endl;
    }
};

// 主板类
class MotherBoard
{
    CPU *cpu;
    GraphicsCard *graphicsCard;
    Memory *memory;
public:
    // 以指针的方式传递
    MotherBoard(CPU *cpu, GraphicsCard *graphicsCard, Memory *memory)
    {
        this->cpu = cpu;
        this->graphicsCard = graphicsCard;
        this->memory = memory;
    }
    // 析构函数
    ~MotherBoard()
    {
        std::cout << "The system is about to shut down..." << std::endl;
        delete cpu;
        delete graphicsCard;
        delete memory;
        cpu = nullptr;
        graphicsCard = nullptr;
        memory = nullptr;
    }
    // 输出硬件信息
    void LayOutInfo(void)
    {
        std::cout << "--------------------------------" << std::endl;
        std::cout << "System Hardware Infomation:" << std::endl;
        std::cout << "CPU: ";
        cpu->info();
        std::cout << "GraphicsCard: ";
        graphicsCard->info();
        std::cout << "Memory: ";
        memory->info();
        std::cout << "--------------------------------" << std::endl;
    }
};

void assembleComputer0(void)
{
    CPU *cpu = new I5_12400F;
    GraphicsCard *graphicsCard = new Rx470;
    Memory *memory = new TForce_Memory;
    MotherBoard motherBoard(cpu, graphicsCard, memory); 
    motherBoard.LayOutInfo();
}

void assembleComputer1(void)
{
    CPU *cpu = new R5_2600;
    GraphicsCard *graphicsCard = new Rx560;
    Memory *memory = new TForce_Memory;
    MotherBoard motherBoard(cpu, graphicsCard, memory); 
    motherBoard.LayOutInfo();
}
int main(void)
{
    assembleComputer0();
    // assembleComputer1();
    system("pause");
}
