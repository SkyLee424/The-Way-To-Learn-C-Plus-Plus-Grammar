#include <iostream>
#include <fstream>
#include <string>
class DingZheng
{
public:
    std::string name;
    // char name[64];
    int age;
};

std::ostream& operator<<(std::ostream &cout, const DingZheng &d)
{
    std::cout << "我是" << d.name << "，今年 " << d.age << " 岁";
    return cout;
}

void write(void)
{
    DingZheng d = {"丁真", 18};
    d.age = 18;
    std::ofstream file("D:\\C++\\Grammar\\File_operation\\dingZheng.txt", std::ios::out | std::ios::binary);
    file.write((char *)&d, sizeof(d));
    file.close();
}

void read(void)
{
    DingZheng d;
    std::ifstream file("D:\\C++\\Grammar\\File_operation\\dingZheng.txt", std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Can't open the file!" << std::endl;
    }
    file.read((char *)&d, sizeof(d));
    std::cout << d << std::endl;
    file.close();
}

int main(void)
{
    write();
    read();
}