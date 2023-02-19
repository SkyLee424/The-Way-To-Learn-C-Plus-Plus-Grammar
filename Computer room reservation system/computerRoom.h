#pragma once
#include<string>

struct ComputerRoom
{
    std::string roomName;
    int capacity;
    int remainCapacity;

    ComputerRoom(std::string name, int c, int rc)
    {
        roomName = name;
        capacity = c;
        remainCapacity = rc;    
    }

};
