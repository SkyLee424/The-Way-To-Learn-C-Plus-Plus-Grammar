#pragma once
#include<iostream>
#include<fstream>
#include<unordered_map>
#include<set>
#include<vector>
#include"menu.h"
#include"status.h"
#include"identity.h"
#include"fileName.h"
#include"computerRoom.h"
#include"singleOrderInfo.h"

static std::unordered_map<std::string, std::vector<SingleOrderInfo>> DataBase;

class Kernel
{

public:

    static void initDatabase(void);

    // 为了让其他cpp文件也可以得到我们这里的database
    static std::unordered_map<std::string, std::vector<SingleOrderInfo>>& getDataBase(void);

    static Status verifyPassword(int mode, std::string &name, std::string id, std::string password);

    static Status getComputerRoom(std::vector<ComputerRoom> &rooms);

    // 添加预约信息
    static void addOrderInfo(std::string id, const SingleOrderInfo &info);    

    // 查询某人的预约信息
    static std::vector<SingleOrderInfo>& getOrderInfo(std::string id);

    static Status modifyRoomCapacity(int roomId, int diffrence);

    static void saveFile(void);

};

