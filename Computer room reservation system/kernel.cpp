#include"kernel.h"
#include"menu.h"
#include"computerRoom.h"

void Kernel::initDatabase(void)
{
    std::ifstream ifs;
    ifs.open(Order_File);
    if(!ifs.is_open())
    {
        Menu::throwExpection(NoFile);
        return;
    }
    std::string row;
    int date, noonTime, roomId, status;
    std::string stuId, stuName;
    while (std::getline(ifs, row))
    {
        int pos = 0;
        pos = row.find("Date: ", pos);
        pos += 6;
        date = std::stoi(row.substr(pos, 1));
        pos = row.find("NoonTime: ", pos);
        pos += 10;
        noonTime = std::stoi(row.substr(pos, 1));
        int l = row.find("StuId: ", pos) + 7;
        int r = row.find("StuName: ", l);
        stuId = row.substr(l, r - l - 1);
        l = r + 9;
        r = row.find("RoomId: ", l);
        stuName = row.substr(l, r - l - 1);
        l = r + 8;
        r = row.find("Status: ", l);
        roomId = std::stoi(row.substr(l, r - l - 1));
        l = r + 8;
        status = std::stoi(row.substr(l));
        DataBase[stuId].emplace_back(stuName, date, noonTime, roomId, status);
    }
    ifs.close();
}

std::unordered_map<std::string, std::vector<SingleOrderInfo>>& Kernel::getDataBase(void)
{
    return DataBase;
}


Status Kernel::verifyPassword(int mode, std::string &name, std::string id, std::string password)
{
    std::ifstream ifs;
    std::string fileName = "";
    if(mode == Student) fileName = Student_File;
    else if(mode == Teacher) fileName = Teacher_File;
    else if(mode == Admin) fileName = Admin_File;
    ifs.open(fileName);
    if(!ifs.is_open())
    {
        Menu::throwExpection(NoFile);
        return Failed;
    }
    std::string _id, _password, _name;
    std::unordered_map<std::string, std::pair<std::string, std::string>> mapping;
    while (ifs >> _id >> _name >> _password)
    {
        mapping[_id].first = _password;
        mapping[_id].second = _name;
    }
    auto it = mapping.find(id);
    if(it == mapping.end())
    {
        Menu::throwExpection(NoSuchUser);
        ifs.close();
        return Failed;
    }
    else if(it->second.first != password)
    {
        Menu::throwExpection(WrongPassword);
        ifs.close();
        return Failed;
    }
    name = it->second.second;

    ifs.close();
    return Success;
}    

Status Kernel::getComputerRoom(std::vector<ComputerRoom> &rooms)
{
    std::ifstream ifs;
    ifs.open(Computer_Room_File);
    if(!ifs.is_open())
    {
        Menu::throwExpection(NoFile);
        return Failed;
    }
    std::string row = "";
    std::string roomName;
    int capacity, remainCapacity;
    while (std::getline(ifs, row))
    {
        int l = 0, r = row.find(',', l);
        roomName = row.substr(l, r - l);
        l = r + 1, r = row.find(',', l);
        capacity = std::stoi(row.substr(l, r - l));
        l = r + 1, r = row.find(',', l);
        remainCapacity = std::stoi(row.substr(l, r - l));
        ComputerRoom computerRoom(roomName, capacity, remainCapacity);
        rooms.push_back(computerRoom);
    }
    ifs.close();
    return Success;
}

void Kernel::addOrderInfo(std::string id, const SingleOrderInfo &info)
{
    DataBase[id].push_back(info);
}

// 记得返回引用
// 静态变量在不同的 cpp 文件里，值不同，也就是说，此database非彼database
std::vector<SingleOrderInfo>& Kernel::getOrderInfo(std::string id)
{
    auto it = DataBase.find(id);
    if(it == DataBase.end())
    {
        static std::vector<SingleOrderInfo> emptyRet;
        return emptyRet;
    }
    return it->second;
}

Status Kernel::modifyRoomCapacity(int roomId, int diffrence)
{
    std::ifstream ifile;
    ifile.open(Computer_Room_File);
    if(!ifile.is_open())
    {
        return NoFile;
    }
    int curRoomId = 0;
    std::vector<std::string> fileData;
    std::string row;
    while (std::getline(ifile, row))
    {
        if(curRoomId == roomId)
        {
            int pos0 = row.find(',');
            int pos1 = row.find(',', pos0 + 1);
            int capacity = std::stoi(row.substr(pos0 + 1, pos1 - pos0 - 1));
            int remainCapacity = std::stoi(row.substr(pos1 + 1));
            remainCapacity += diffrence;
            if(remainCapacity < 0)
            {
                ifile.close();
                return FullRoom;
            }
            else if(remainCapacity > capacity)
            {
                ifile.close();
                return Success;
            }
            while (row.back() != ',')
            {
                row.pop_back();
            }
            row += std::to_string(remainCapacity);
        }
        fileData.push_back(row);
        ++curRoomId;   
    }
    int s = fileData.size();
    ifile.close();
    std::ofstream ofile;
    ofile.open(Computer_Room_File, std::ios::trunc);
    for(auto &row : fileData)
    {
        ofile << row << std::endl;
    }
    ofile.close();
    return Success;
}

void Kernel::saveFile(void)
{
    std::ofstream ofs;
    ofs.open(Order_File, std::ios::trunc);

    for(auto &info : DataBase)
    {
        std::string id = info.first;

        for(auto &singleInfo : info.second)
        {
            ofs << "Date: " << singleInfo.date << " ";
            ofs << "NoonTime: " << singleInfo.noonTime << " ";
            ofs << "StuId: " << id << " ";
            ofs << "StuName: " << singleInfo.stuName << " ";
            ofs << "RoomId: " << singleInfo.roomId << " ";
            ofs << "Status: " << singleInfo.status << std::endl;
        }
    }

    ofs.close();
}


