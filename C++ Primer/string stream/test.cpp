#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Person
{
    std::string name;
    std::vector<std::string> phoneNums;
};

std::ostream& operator<<(std::ostream &os, const Person &p)
{
    os << p.name;
    for(const auto &phoneNum : p.phoneNums)
    {
        os << " " << phoneNum;
    }
    return os;
}

bool isVaild(const std::string &phoneNum)
{
    int n = phoneNum.size();
    if(n != 11) return false;
    std::string check = "0123456789";
    if(phoneNum.find_first_not_of(check, 0) != std::string::npos)
        return false;
    return true;
}

std::string format(const std::string &phoneNum)
{
    std::string res = phoneNum.substr(0, 3);
    res.push_back(' ');
    res.append(phoneNum.substr(3, 4));
    res.push_back(' ');
    res.append(phoneNum.substr(7, 4));
    return res;
}

// istringstream 实例
std::vector<Person> getPersonInfos(void)
{
    std::fstream info;
    info.open("info.txt");
    std::vector<Person> infos;
    std::string line, phoneNum;
    while (std::getline(info, line))
    {
        // 创建 istringstream 对象
        std::istringstream iss(line);
        Person p;
        iss >> p.name;
        while (iss >> phoneNum)
        {
            p.phoneNums.push_back(phoneNum);
        }
        infos.push_back(p);
    }
    info.close();
    return infos;

}

// ostringstream 实例
void showPersonInfos(const std::vector<Person> &infos)
{
    for(const auto &info : infos)
    {
        // formatted 存储已经格式化的电话号码
        // badNum 存储违法的电话号码
        std::ostringstream formatted, badNums;
        for(auto &phoneNum : info.phoneNums)
        {
            if(isVaild(phoneNum))
            {
                formatted << format(phoneNum) << "\n";
            }
            else badNums << phoneNum << "\n";
        }
        if(badNums.str().size() == 0)
        {
            std::cout << "Name: " << info.name << ", phoneNumber(s): " << std::endl;
            std::cout << formatted.str() << std::endl;
        }
        else
        {
            std::cerr << "Name: " << info.name << ", invaild phoneNumber(s): " << std::endl;
            std::cerr << badNums.str() << std::endl;
        }
    }
}


int main(void)
{
    showPersonInfos(getPersonInfos());
}
