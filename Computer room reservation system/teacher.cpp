#include "identity.h"

_Teacher::~_Teacher(void)
{
}

Status _Teacher::login(void)
{
    std::cout << "请输入您的职工号" << std::endl;
    std::getline(std::cin, id);
    basicLogin();
    return Kernel::verifyPassword(Teacher, this->name, id, password);
}

void _Teacher::showMenu(void)
{
    while (1)
    {
        std::cout << "欢迎，老师" << this->name << std::endl;
        Menu::printLine(20);
        std::cout << "请选择要执行的操作：" << std::endl;
        std::cout << "0. 查看所有预约" << std::endl;
        std::cout << "1. 审核预约" << std::endl;
        std::cout << "2. 注销登录" << std::endl;
        char opinion;
        std::cin >> opinion;
        opinion -= '0';
        getchar();
        switch (opinion)
        {
        case 0:
            Menu::printLine(50);
            std::cout << "                  查看所有预约" << std::endl;
            Menu::printLine(50);
            Menu::showAllOrderInfo();
            break;
        case 1:
        {
            Menu::printLine(50);
            std::cout << "                   审核预约" << std::endl;
            int status = reviewAppointment();
            if(status == Success)
            {
                std::cout << "审核完成！您的记录已保存" << std::endl;
            }
            else if(status == CancelReview)
            {
                std::cout << "用户主动取消了审核" << std::endl;
            }
            else if(status == NoFile)
            {
                Menu::throwExpection(NoFile);
            }

            Menu::printLine(50);
            break;
        }
        case 2:
            this->logout();
            return;
        default:
            Menu::throwExpection(InputError);
            break;
        }
        Menu::wait();
    }
}

Status _Teacher::reviewAppointment()
{
    // 标记某个记录是否可以审核
    bool flag = true;
    while (1)
    {
        if(Menu::showAllOrderInfo() == NoOrder)
        {
            return NoOrder;
        }
        std::cout << "您想审核哪一个学生的预约？（输入其账号即可）" << std::endl;
        std::string idToBeReviewed;
        std::getline(std::cin, idToBeReviewed);
        auto &infosToBeReviewed = Kernel::getOrderInfo(idToBeReviewed);
        if(infosToBeReviewed.size() == 0)
        {
            std::cout << "该学生还没有任何预约记录" << std::endl;
            while (1)
            {
                std::cout << "是否需要继续审核？（Y/N）" << std::endl;
                char choose;
                std::cin >> choose;
                getchar();
                if(choose == 'Y') break;
                else if(choose == 'N') return CancelReview;
                else
                {
                    Menu::throwExpection(InputError);
                    Menu::wait();
                }
            }
            continue;
        }

        while (1)
        {
            Menu::showOrderInfo(infosToBeReviewed);
            std::cout << "您想审核哪条预约？输入编号即可" << std::endl;
            int choose;
            std::cin >> choose;
            getchar();
            if(choose < 0 || choose >= infosToBeReviewed.size())
            {
                Menu::throwExpection(InputError);
                Menu::wait();
            }
            // 已经被审核过
            else if(infosToBeReviewed[choose].status != UnderReview)
            {
                while (1)
                {
                    std::cout << "该记录已经被审核过，您是想修改其审核状态吗？(Y/N)" << std::endl;
                    char op;
                    std::cin >> op;
                    getchar();
                    if(op == 'Y') 
                    {
                        while (1)
                        {
                            int _choose;
                            std::cout << "将状态修改为：" << std::endl;
                            std::cout << "0. 审核通过" << std::endl;
                            std::cout << "1. 审核失败" << std::endl;
                            std::cin >> _choose;
                            getchar();
                            int status;
                            int roomId = infosToBeReviewed[choose].roomId;
                            if(_choose == 0) 
                            {
                                if(infosToBeReviewed[choose].status == Approved)
                                {
                                    std::cout << "该预约已经审核通过，无须再次审核" << std::endl;
                                    flag = false;
                                }
                                else
                                {
                                    status = Kernel::modifyRoomCapacity(roomId, -1);
                                    if(status == Success)
                                    {
                                        infosToBeReviewed[choose].status = Approved;
                                    }
                                    else if(status == NoFile)
                                    {
                                        return NoFile;
                                    }
                                    else if(status == FullRoom)
                                    {
                                        std::cout << "该机房剩余容量不足，审核无法通过！" << std::endl; 
                                        flag = false;
                                    }
                                }
                            }
                            else if(_choose == 1) 
                            {
                                if(infosToBeReviewed[choose].status == AuditFailed)
                                {
                                    std::cout << "该预约已经审核未通过，无须再次审核" << std::endl;
                                    flag = false;
                                }
                                else
                                {
                                    status = Kernel::modifyRoomCapacity(roomId, 1);
                                    if(status == NoFile) return NoFile;
                                    infosToBeReviewed[choose].status = AuditFailed;
                                }
                            }
                            else
                            {
                                Menu::throwExpection(InputError);
                                Menu::wait();
                                continue;
                            }
                            break;
                        }
                        break;
                    }
                    else if(op == 'N'){break;}
                    else
                    {
                        Menu::throwExpection(InputError);
                        Menu::wait();
                    }
                }
                break;
            }
            // 正常情况
            else
            {
                while (1)
                {
                    char _choose;
                    std::cout << "将状态修改为：" << std::endl;
                    std::cout << "0. 审核通过" << std::endl;
                    std::cout << "1. 审核失败" << std::endl;
                    std::cin >> _choose;
                    _choose -= '0';
                    getchar();
                    int status;
                    int roomId = infosToBeReviewed[choose].roomId;
                    if(_choose == 0) 
                    {
                        status = Kernel::modifyRoomCapacity(roomId, -1);
                        if(status == Success)
                        {
                            infosToBeReviewed[choose].status = Approved;
                        }
                        else if(status == NoFile)
                        {
                            return NoFile;
                        }
                        else if(status == FullRoom)
                        {
                            std::cout << "该机房剩余容量不足，审核无法通过！" << std::endl; 
                            flag = false;
                        }
                    }
                    else if(_choose == 1) 
                    {
                        infosToBeReviewed[choose].status = AuditFailed;
                    }
                    else
                    {
                        Menu::throwExpection(InputError);
                        Menu::wait();
                        continue;
                    }
                    break;
                }
                break;
            }
        }

        while (1)
        {
            if(flag) std::cout << "该审核记录已生效！是否继续审核其他记录？(Y/N)" << std::endl;
            else 
            {
                std::cout << "是否继续审核其他记录？(Y/N)" << std::endl;
                flag = true;
            }
            char choose;
            std::cin >> choose;
            getchar();
            if(choose == 'Y') break;
            else if(choose == 'N')
            {
                Kernel::saveFile();
                return Success;
            }
            else
            {
                Menu::throwExpection(InputError);
                Menu::wait();
            }
        }
    }
    Kernel::saveFile();
    return Success;
}