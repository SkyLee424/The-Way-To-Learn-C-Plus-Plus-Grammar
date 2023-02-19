#include "kernel.h"
#include "menu.h"
#include "identity.h"

void Menu::printLine(int n)
{
    std::string line(n, '-');
    std::cout << line << std::endl;
}

void Menu::throwExpection(Status status)
{
    switch (status)
    {
    case InputError:
        std::cout << "输入有误，请检查你的输入！" << std::endl;
        break;
    case NoFile:
        std::cout << "文件不存在！请检查程序是否损坏" << std::endl;
        break;
    case NoSuchUser:
        std::cout << "该用户不存在！请检查你的用户名是否正确" << std::endl;
        break;
    case WrongPassword:
        std::cout << "密码错误！请检查你的密码是否正确" << std::endl;
    default:
        break;
    }
}

void Menu::wait(void)
{
    std::cout << "按‘Enter’以继续操作" << std::endl;
    std::cin.get();
}

void Menu::showBasicMenu(void)
{
    while (1)
    {
        Identity *identity = nullptr;
        printLine(30);
        std::cout << "欢迎使用机房预约系统！" << std::endl;
        std::cout << "请选择你的登录方式：" << std::endl;
        std::cout << "0. 学生登录" << std::endl;
        std::cout << "1. 教师登录" << std::endl;
        std::cout << "2. 管理员登录" << std::endl;
        std::cout << "3. 退出系统" << std::endl;
        printLine(30);
        char opinion;
        std::cin >> opinion;
        opinion -= '0';
        getchar();
        switch (opinion)
        {
        case Student:
        {
            identity = new _Student();
            if (identity->login() != Success)
            {
                break;
            }
            std::cout << "登录成功！" << std::endl;
            identity->showMenu();
            break;
        }
        case Teacher:
            identity = new _Teacher();
            if (identity->login() != Success)
            {
                break;
            }
            std::cout << "登录成功！" << std::endl;
            identity->showMenu();
            break;
        case Admin:
            identity = new _Admin();
            if (identity->login() != Success)
            {
                break;
            }
            std::cout << "登录成功！" << std::endl;
            identity->showMenu();
            break;
        case ExitSystem:
            std::cout << "即将退出系统...\n欢迎下次使用" << std::endl;
            wait();
            return;
        default:
            throwExpection(InputError);
            break;
        }
        if (identity)
        {
            delete identity;
        }
        wait();
    }
}

void Menu::showComputerRoom(std::vector<ComputerRoom> &rooms)
{
    int cnt = 0;
    for(auto computerRoom : rooms)
    {
        Menu::printLine(50);
        std::cout << "机房编号：" << cnt << std::endl;
        std::cout << "机房名称：" << computerRoom.roomName << std::endl;
        std::cout << "容量：" << computerRoom.capacity << std::endl;
        std::cout << "剩余容量：" << computerRoom.remainCapacity << std::endl;
        ++cnt;
    }
    Menu::printLine(50);
}

Status Menu::showOrderInfo(const std::vector<SingleOrderInfo> &infos, int index)
{
    if(infos.size() == 0) 
    {
        return NoOrder;
    }
    for(auto singleInfo : infos)
    {
        std::cout << "预约编号：" << index << std::endl;
        int date = singleInfo.date;
        std::string s_date;
        if(date == 1) s_date = "周一";
        else if(date == 2) s_date = "周二";
        else if(date == 3) s_date = "周三";
        else if(date == 4) s_date = "周四";
        else if(date == 5) s_date = "周五";
        std::cout << "预约日期：" << s_date << std::endl;
        std::string noonTime = singleInfo.noonTime == 0 ? "上午" : "下午";
        std::cout << "预约时段：" << noonTime << std::endl;
        std::cout << "机房编号：" << singleInfo.roomId << std::endl;
        std::string status;
        if(singleInfo.status == Approved) status = "审核通过";
        else if(singleInfo.status == AuditFailed) status = "审核失败";
        else status = "审核中";
        std::cout << "预约状态：" << status << std::endl;
        Menu::printLine(50);
        ++index;
    }
    return Success;
}

Status Menu::showAllOrderInfo(void)
{
    int index = 0;
    auto &database = Kernel::getDataBase();
    if(database.size() == 0)
    {
        std::cout << "当前没有任何预约信息！" << std::endl;
        return NoOrder;
    }
    for(auto &info : database)
    {
        std::cout << "账号：" << info.first << std::endl;
        std::cout << "姓名：" << (*info.second.begin()).stuName << std::endl;
        Menu::printLine(50);
        showOrderInfo(info.second, index);
        index += info.second.size();
    }
    return Success;
}