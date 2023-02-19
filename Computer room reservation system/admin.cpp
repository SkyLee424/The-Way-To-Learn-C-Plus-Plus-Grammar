#include "identity.h"

_Admin::~_Admin(void)
{
}

Status _Admin::login(void)
{
    std::cout << "请输入您的管理员账号" << std::endl;
    std::getline(std::cin, id);
    basicLogin();
    return Kernel::verifyPassword(Admin, this->name, id, password);
}

void _Admin::showMenu(void)
{
    while (1)
    {
        std::cout << "欢迎，管理员" << this->name << std::endl;
        Menu::printLine(20);
        std::cout << "请选择要执行的操作：" << std::endl;
        std::cout << "0. 添加账号" << std::endl;
        std::cout << "1. 查看账号" << std::endl;
        std::cout << "2. 查看机房" << std::endl;
        std::cout << "3. 清空预约" << std::endl;
        std::cout << "4. 注销登录" << std::endl;
        char opinion;
        std::cin >> opinion;
        opinion -= '0';
        getchar();
        switch (opinion)
        {
        case 0:
        {
            Menu::printLine(20);
            std::cout << "      添加账号" << std::endl;
            Menu::printLine(20);
            std::cout << "请选择添加账号的方式" << std::endl;
            std::cout << "0. 添加学生账号" << std::endl;
            std::cout << "1. 添加教师账号" << std::endl;
            char choose ;
            std::cin >> choose;
            choose -= '0';
            getchar();
            if(choose != Student && choose != Teacher) 
                Menu::throwExpection(InputError);
            else 
            {
                int cnt = addAccount(choose);
                if(cnt != Failed)
                {
                    std::cout << "已成功添加 " << cnt << " 个账号！" << std::endl;
                }
                else std::cout << "添加账号失败！" << std::endl;
            }
            break;
        }
        case 1:
        {
            Menu::printLine(20);
            std::cout << "      查看账号" << std::endl;
            Menu::printLine(20);
            int cnt;
            while (1)
            {
                std::cout << "请选择查看账号的方式" << std::endl;
                std::cout << "0. 查看学生账号" << std::endl;
                std::cout << "1. 查看教师账号" << std::endl;
                char choose;
                std::cin >> choose;
                choose -= '0';
                getchar();
                if(choose == Student)
                {
                    Menu::printLine(50);
                    std::cout << "                   学生账号信息" << std::endl;
                    Menu::printLine(50);
                    cnt = viewAccount(Student);
                    Menu::printLine(50);
                }
                else if(choose == Teacher)
                {
                    Menu::printLine(50);
                    std::cout << "                   教师账号信息" << std::endl;
                    Menu::printLine(50);
                    cnt = viewAccount(Teacher);
                    Menu::printLine(50);

                }
                else 
                {
                    Menu::throwExpection(InputError);
                    continue;;
                }
                break;
            }
            std::cout << "查询完毕，共查询到 " << cnt << " 条相关信息" << std::endl;
            break; 
        }
        case 2:
        {
            Menu::printLine(50);
            std::cout << "                          查看机房" << std::endl;
            std::vector<ComputerRoom> rooms;
            if(Kernel::getComputerRoom(rooms) == Failed)
                std::cout << "查询失败！" << std::endl;
            else 
            {
                Menu::showComputerRoom(rooms);
            }
            break;
        }
        case 3:
        {
            Menu::printLine(30);
            std::cout << "           清空预约" << std::endl;
            Menu::printLine(30);
            while (1)
            {
                std::cout << "你确定要清空所有预约吗？(Y/N)" << std::endl;
                char choose;
                std::cin >> choose;
                getchar();
                if(choose == 'Y') 
                {
                    if(emptyOrders() == Success)
                    {
                        std::cout << "清空成功！" << std::endl;
                    }
                    else std::cout << "清空失败！" << std::endl;
                    break;
                }
                else if(choose == 'N') break;
                else 
                {
                    Menu::throwExpection(InputError);
                    Menu::wait();
                }
            }
            break;
        }
        case 4:
            this->logout();
            return;
        default:
            Menu::throwExpection(InputError);
            break;
        }
        Menu::wait();
    }
}

int _Admin::addAccount(int identity)
{
    std::string fileName;
    if(identity == Student) fileName = Student_File;
    else if(identity == Teacher) fileName = Teacher_File;
    else return Failed;
    std::ifstream ifs;
    ifs.open(fileName);
    if(!ifs.is_open())
    {
        Menu::throwExpection(NoFile);
        return Failed;
    }
    std::set<std::string> alreadyExist;
    std::string row;
    while (std::getline(ifs, row))
    {
        auto id = row.substr(0, row.find(' '));
        alreadyExist.insert(id);
    }
    ifs.close();
    std::ofstream ofs;
    ofs.open(fileName, std::ios::app);
    
    std::string name, id, password;
    int cnt = 0;
    while (1)
    {
        while (1)
        {
            if(identity == Student) 
                std::cout << "请输入学生的学号： ";
            else std::cout << "请输入教师的职工号： ";
            std::getline(std::cin, id);
            if(alreadyExist.count(id))
            {
                if(identity == Student) 
                    std::cout << "该学号已存在，";   
                else std::cout << "该职工号已存在，";
                while(1)
                {
                    std::cout << "是否继续添加其他账号(Y/N)" << std::endl;
                    char op;
                    std::cin >> op;
                    getchar();
                    if(op == 'Y') break;
                    else if(op == 'N') return cnt;
                    else 
                    {
                        Menu::throwExpection(InputError);
                    }
                }
            }
            else break;
        }

        if(identity == Student) 
            std::cout << "请输入学生的姓名： ";
        else std::cout << "请输入教师的姓名：" ;
        std::getline(std::cin, name);
        std::cout << "请输入该账号的登录密码：";
        std::getline(std::cin, password);
        ofs << id << " " << name << " " << password << " " << std::endl;
        alreadyExist.insert(id);
        ++cnt;
        std::cout << "添加成功！是否继续添加？(Y/N)" << std::endl;
        char opinion;
        std::cin >> opinion;
        getchar();
        if(opinion == 'Y') continue;
        else if(opinion == 'N') break;
        else
        {
            while (1)
            {
                Menu::throwExpection(InputError);
                std::cout << "是否继续添加？(Y/N)" << std::endl;
                std::cin >> opinion;
                getchar();
                if(opinion == 'Y') break;
                else if(opinion == 'N') 
                {
                    ofs.close();
                    return cnt;
                }
            }   
        }
    }
    ofs.close();
    return cnt;
}

Status _Admin::viewAccount(int identity)
{
    std::string fileName;
    if(identity == Student) fileName = Student_File;
    else if(identity == Teacher) fileName = Teacher_File;
    else return Failed;
    std::ifstream ifs;
    ifs.open(fileName);
    if(!ifs.is_open())
    {
        Menu::throwExpection(NoFile);
        return Failed;
    }
    std::string id, name, password;
    int cnt = 0;
    while (ifs >> id >> name >> password)
    {
        std::cout << "账号：" << id << "，姓名：" << name << "，密码：" << password << std::endl;
        ++cnt;
    }
    ifs.close();
    return cnt;
}

Status _Admin::emptyOrders(void)
{
    std::ofstream ofs;
    ofs.open(Order_File);
    if(!ofs.is_open())
    {
        Menu::throwExpection(NoFile);
        return Failed;
    }
    ofs.clear();
    ofs.close();
    auto &database = Kernel::getDataBase();
    database.clear();
    return Success;
}