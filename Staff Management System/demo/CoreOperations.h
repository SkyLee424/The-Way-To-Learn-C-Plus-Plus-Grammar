#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<queue>
#include<algorithm>
#include"Workers.h"
#include"status.h"
class CoreOperations
{
    friend class Menu;
    std::deque<Workers*> DataBase;
    std::fstream workersInfo;
    // 打印分割线
    static void printLine(std::string dept, int numOfLine = 0);

    // 读取
    void load(void);

    // 自动保存
    void autoSave(void);
public:
    // 添加职员
    void addWorkers(void);

    // 打印所有职员信息
    int display(void);

    // 查找某一个职员
    // opinion：0. 按姓名查找，1. 按编号查找
    std::deque<Workers *>::iterator findWorker(int opinion, const std::string &s);

    // 删除某一个职员
    // opinion：0. 按姓名删除，1. 按编号删除
    bool deleteWorker(int opinion, const std::string &s);

    // 修改某一个职员
    // opinion：0. 指定姓名修改，1. 指定编号修改
    int modifyWorker(int opinion, const std::string &s);

    // 排序
    // 0. 按编号升序，1. 按编号降序，2. 按姓名升序，3.按姓名降序
    int sort(int opinion);

    // 清空数据库
    void deleteAllEmployeeInfo(void);
};
class Menu
{
public:
    Menu();

    CoreOperations coreOperations;
    // 展示菜单
    void displayMenu(void);
    
private:
    // 得到用户输入的操作
    int getAction(void);

public:
    // 展示对应提示信息
    int userInteraction();
};
