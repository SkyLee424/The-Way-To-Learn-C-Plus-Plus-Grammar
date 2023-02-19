#include "kernel.h"
#include "identity.h"
Identity::Identity(void)
{
    this->name = "?";
    this->id = "?";
    this->password = "?";
}

Identity::Identity(std::string name, std::string id)
{
    this->name = name;
    this->id = id;
}

Identity::~Identity()
{
}

void Identity::basicLogin(void)
{
    std::cout << "请输入您的密码" << std::endl;
    std::getline(std::cin, password);
}

void Identity::logout(void)
{
    std::cout << "即将注销你的登录..." << std::endl;
}