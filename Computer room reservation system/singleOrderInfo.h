#pragma once
#include<string>

struct SingleOrderInfo
{
    std::string stuName;
    int date;
    int noonTime;
    int roomId;
    int status;

    SingleOrderInfo() = default;

    SingleOrderInfo(std::string stuName, int date, int noonTime, int roomId, int status)
        :stuName(stuName), date(date), noonTime(noonTime), roomId(roomId), status(status) {}
};
