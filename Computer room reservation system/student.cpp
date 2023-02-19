#include "identity.h"

_Student::~_Student(void)
{
}

Status _Student::login(void)
{
    std::cout << "请输入您的学号" << std::endl;
    std::getline(std::cin, id);
    basicLogin();
    return Kernel::verifyPassword(Student, this->name, id, password);
}

void _Student::showMenu(void)
{
    while (1)
    {
        std::cout << "欢迎，学生" << this->name << std::endl;
        Menu::printLine(20);
        std::cout << "请选择要执行的操作：" << std::endl;
        std::cout << "0. 申请预约" << std::endl;
        std::cout << "1. 查看我的所有预约" << std::endl;
        std::cout << "2. 查看所有预约" << std::endl;
        std::cout << "3. 取消预约" << std::endl;
        std::cout << "4. 注销登录" << std::endl;
        char opinion;
        std::cin >> opinion;
        opinion -= '0';
        getchar();
        switch (opinion)
        {
        case 0:
        {
            Menu::printLine(50);
            std::cout << "                 申请预约" << std::endl;
            Menu::printLine(50);
            int applyStatus = applyForAppointment();
            if(applyStatus == Success)
            {
                std::cout << "预约成功！" << std::endl;
            }
            else if(applyStatus == CancelOrder)
            {
                std::cout << "用户主动取消了预约" << std::endl;
            }
            else std::cout << "预约失败！" << std::endl;
            std::cout << "即将退出预约..." << std::endl;
            Menu::printLine(50);
            break;
        }
        case 1:
        {
            Menu::printLine(50);
            std::cout << "                 查看我的所有预约" << std::endl;
            Menu::printLine(50);
            int status = Menu::showOrderInfo(Kernel::getOrderInfo(this->id));
            if(status == NoOrder)
            {
                std::cout << "你还没有预约，快去预约一个机房吧！" << std::endl;
            }
            break;
        }
        case 2:
            Menu::printLine(50);
            std::cout << "                  查看所有预约" << std::endl;
            Menu::printLine(50);
            Menu::showAllOrderInfo();
            break;
        case 3:
        {
            Menu::printLine(50);
            std::cout << "                     取消预约" << std::endl;
            Menu::printLine(50);
            int status = cancelOrder();
            if(status == Success)
            {
                std::cout << "取消预约成功！" << std::endl;
            }
            else if(status == CancelOrder)
            {
                std::cout << "用户主动放弃取消预约" << std::endl;
            }
            else if(status == NoOrder)
            {
                std::cout << "你还没有任何预约，无法取消预约捏" << std::endl;
            }
            else if(status == Failed)
            {
                std::cout << "取消预约失败！" << std::endl;
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

Status _Student::applyForAppointment(void)
{
    int dayTime;
    while (1)
    {
        char choose;
        std::cout << "机房开放时间为周一至周五" << std::endl;
        std::cout << "请选择预约时间:" << std::endl;
        std::cout << "1. 周一" << std::endl;
        std::cout << "2. 周二" << std::endl;
        std::cout << "3. 周三" << std::endl;
        std::cout << "4. 周四" << std::endl;
        std::cout << "5. 周五" << std::endl;
        std::cin >> choose;
        getchar();
        dayTime = choose - '0';
        if(dayTime >= 1 && dayTime <= 5) break;
        Menu::throwExpection(InputError);
        Menu::wait();
    }

    int noonTime;
    while (1)
    {
        char choose;
        std::cout << "请选择预约时间段" << std::endl;
        std::cout << "0. 上午" << std::endl;
        std::cout << "1. 下午" << std::endl;
        std::cin >> choose;
        getchar();
        noonTime = choose - '0';
        if(noonTime == 0 || noonTime == 1) break;
        Menu::throwExpection(InputError);
        Menu::wait();
    }
    
    std::vector<ComputerRoom> rooms;
    if(Kernel::getComputerRoom(rooms) == Failed)
        return Failed;
    std::ofstream ofs;
    ofs.open(Order_File, std::ios::app);
    if(!ofs.is_open())
    {
        Menu::throwExpection(NoFile);
        return Failed;
    }

    int roomID;
    while (1)
    {
        char choose;
        std::cout << "请选择机房，输入编号即可" << std::endl;
        Menu::showComputerRoom(rooms);
        std::cin >> choose;
        roomID = choose - '0'; 
        getchar();
        if(roomID < 0 || roomID >= rooms.size()) 
        {
            Menu::throwExpection(InputError);
            Menu::wait();
        }
        else break;
    }

    while (1)
    {
        std::cout << "请检查你的预约信息，是否确定预约？(Y/N)" << std::endl;
        char choose;
        std::cin >> choose;
        getchar();
        if(choose == 'N')
        {
            ofs.close();
            return CancelOrder;
        }
        else if(choose != 'Y')
        {
            Menu::throwExpection(InputError);
            Menu::wait();
        }
        else break;
    }
    
    ofs << "Date: " << dayTime << " ";
    ofs << "NoonTime: " << noonTime << " ";
    ofs << "StuId: " << this->id << " ";
    ofs << "StuName: " << this->name << " ";
    ofs << "RoomId: " << roomID << " ";
    ofs << "Status: " << UnderReview << std::endl;

    SingleOrderInfo info(this->name, dayTime, noonTime, roomID, UnderReview);

    Kernel::addOrderInfo(id, info);

    ofs.close();
    return Success;
}

Status _Student::cancelOrder(void)
{
    auto &infos = Kernel::getOrderInfo(this->id);
    if(infos.size() == 0)
    {
        return NoOrder;
    }
    std::cout << "您的预约信息如下" << std::endl;
    Menu::showOrderInfo(infos);
    char choose;
    while (1)
    {
        std::cout << "仅限取消审核中或者审核通过的预约" << std::endl;
        std::cout << "您想取消哪条预约？" << std::endl;
        std::cin >> choose;
        choose -= '0';
        getchar();
        if(choose < 0 || choose >= infos.size())
        {
            Menu::throwExpection(InputError);
            Menu::wait();   
        }
        else if(infos[choose].status == AuditFailed)
        {
            while (1)
            {
                std::cout << "当前选中的预约不可取消！" << std::endl;
                std::cout << "是否取消其他预约？(Y/N)" << std::endl;
                char op;
                std::cin >> op;
                getchar();
                if(op == 'Y') break;
                else if(op == 'N') return CancelOrder;
                else Menu::throwExpection(InputError);
                Menu::wait();
            }
        }
        else break;
    }

    while (1)
    {
        std::cout << "您确定要取消这条预约吗？(Y/N)" << std::endl;
        char op;
        std::cin >> op;
        getchar();
        if(op == 'Y') break;
        else if(op == 'N') return CancelOrder;
        else 
        {
            Menu::throwExpection(InputError);
            Menu::wait();
        }
    }

    auto it = infos.begin() + choose;
    Kernel::modifyRoomCapacity(it->roomId, 1);
    infos.erase(it);
    Kernel::saveFile();
    return Success;
}