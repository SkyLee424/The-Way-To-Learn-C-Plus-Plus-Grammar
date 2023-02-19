#pragma once
#include "status.h"
#include "computerRoom.h"
#include "singleOrderInfo.h"

class Menu
{
    friend class Kernel;
    friend class Identity;
    friend class _Admin;
    friend class _Teacher;
    friend class _Student;

    static void printLine(int n);

    static void throwExpection(Status status);

    static void showComputerRoom(std::vector<ComputerRoom> &rooms);

    // 展示全部预约信息
    static Status showAllOrderInfo(void);

    // 展示某人的预约信息
    static Status showOrderInfo(const std::vector<SingleOrderInfo> &infos, int index = 0);

    static void wait(void);

public:
    static void showBasicMenu(void);
};