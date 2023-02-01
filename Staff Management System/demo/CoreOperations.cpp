#include"CoreOperations.h"

// 不用加 static 关键字了
void CoreOperations::printLine(std::string dept, int numOfLine)
{
    if(dept == "Staff") numOfLine = 47;
    else if(dept == "Manager") numOfLine = 86;
    else if(dept == "Boss") numOfLine = 53;
    for(int i = 0; i < numOfLine; ++i)
    {
        std::cout << "-";
    }
    if(numOfLine == 47 || numOfLine == 86 || numOfLine == 53) 
        std::cout << '\n';
}

void CoreOperations::load(void)
{
    workersInfo.open("D:\\Staff Management System\\demo\\workersInfo.txt", std::ios::in);
    if(!workersInfo.is_open()) return ;
    std::string name, ID, departmentId;
    char sex;
    int age;
    std::string row = "";
    while (workersInfo >> name && workersInfo >> sex && workersInfo >> age && workersInfo >> ID && workersInfo >> departmentId)
    {
        if(departmentId == "0")
        {
            DataBase.push_back(new Staff(name, sex, age, ID));
        }
        else if(departmentId == "1")
        {
            DataBase.push_back(new Manager(name, sex, age, ID));
        }
        else if(departmentId == "2")
        {
            DataBase.push_back(new Boss(name, sex, age, ID));
        }
    }
    
    workersInfo.close();
}

void CoreOperations::autoSave(void)
{
    workersInfo.open("D:\\Staff Management System\\demo\\workersInfo.txt", std::ios::out | std::ios::trunc);
    for(auto it = DataBase.begin(); it != DataBase.end(); ++it)
    {
        workersInfo << (*it)->name << " " << (*it)->sex << " ";
        workersInfo << (*it)->age << " " << (*it)->id << " " << (*it)->departmentId << std::endl;
    }
    workersInfo.close();
}

void CoreOperations::addWorkers(void)
{
    while (1)
    {
        std::string name, ID;
        char sex;
        int age;
        int opinion;
        std::cout << "\nPlease enter the name of the employee (Enter - 1 to end): ";
        while (1)
        {
            std::getline(std::cin, name);
            if(name.size() > 0) break;
        }
        if(name == "-1")
            break;
        std::cout << "\nPlease enter the gender of the employee(M/F): ";
        std::cin >> sex;
        getchar();
        if(sex != 'M' && sex != 'F')
        {
            std::cout << "\nInput error, please re-enter!\n";
            continue;
        }
        std::cout << "\nPlease enter the age of the employee: ";
        std::cin >> age;
        getchar();
        std::cout << "\nPlease enter the employee's ID: ";
        while (1)
        {
            std::getline(std::cin, ID);
            if(ID.size() > 0) break;
        }
        if(findWorker(1, ID) != DataBase.end())
        {
            std::cout << "\nEmployee with this number already exists. Please re-enter" << std::endl;
            continue;
        }
        std::cout << "\nPlease enter the employee's position(0. Staff, 1. Manager, 2. Boss): ";
        std::cin >> opinion;
        getchar();
        switch (opinion)
        {
            case 0:
            {
                Staff *s = new Staff(name, sex, age, ID);
                DataBase.push_back(s);
                break;
            }
            case 1:
            {
                Manager *m = new Manager(name, sex, age, ID);
                DataBase.push_back(m);
                break;
            }
            case 2:
            {
                Boss *b = new Boss(name, sex, age, ID);
                DataBase.push_back(b);
                break;
            }
            default:
                std::cout << "\nInput error, please re-enter!\n";
                // system("pause");
                break;
        }
    }
    autoSave();
}

int CoreOperations::display(void)
{
    if(DataBase.size() == 0) return ERROR0;
    for(auto it = DataBase.begin(); it != DataBase.end(); ++it)
    {
        (*it)->showInfo();
        printLine((*it)->getDeptName());
    }
    return SUCCESS;
}

std::deque<Workers *>::iterator CoreOperations::findWorker(int opinion, const std::string &s)
{
    auto it = DataBase.begin();
    for(; it != DataBase.end(); ++it)
    {
        if((opinion == 0 && (*it)->name == s) || (opinion == 1 && (*it)->id == s))
        { 
            break;  
        }
    }
    return it;
}

bool CoreOperations::deleteWorker(int opinion, const std::string &s)
{
    auto it = findWorker(opinion, s);
    if(it == DataBase.end()) return false;
    auto worker = (*it);
    DataBase.erase(it);
    delete worker;
    worker = nullptr;
    autoSave();
    return true;
}

int CoreOperations::modifyWorker(int opinion, const std::string &s)
{
    auto it = findWorker(opinion, s);
    if(it == DataBase.end()) 
        return DataBase.size() == 0 ? ERROR0 : ERROR1;
    (*it)->showInfo();
    printLine((*it)->getDeptName());
    std::string name, ID;
    int age;
    char sex;
    std::cout << "\nChange the employee's name to: ";
    while (1)
    {
        std::getline(std::cin, name);
        if(name.size() > 0) break;
    }
    std::cout << "\nChange the employee's gender to: ";
    std::cin >> sex; getchar();
    if(sex != 'M' && sex != 'F') return ERROR1;
    std::cout << "\nChange the employee's age to: ";
    std::cin >> age; getchar();
    std::cout << "\nChange the employee's ID to: ";
    while (1)
    {
        std::getline(std::cin, ID);
        if(ID.size() > 0) break;
    }
    std::cout << "\nNeed to change the position of employees(Y/N)? ";
    char choice;
    std::cin >> choice; getchar(); 
    if(choice == 'Y')
    {
        int pos = -1;
        std::cout << "\nChange the employee's position to(0. Staff, 1. Manager, 2. Boss): ";
        std::cin >> pos; getchar();
        auto temp = *it;
        switch (pos)
        {
        case 0:
        {
            Staff *s = new Staff(name, sex, age, ID);
            *it = s;
            break;
        }
        case 1:
        {
            Manager *m = new Manager(name, sex, age, ID);
            *it = m;
            break;
        }
        case 2:
        {
            Boss *b = new Boss(name, sex, age, ID);
            *it = b;
            break;
        }
        // 输入有误
        default:
            return ERROR1;
        }
    }
    else if(choice == 'N')
    {
        (*it)->name = name;
        (*it)->sex = sex;
        (*it)->age = age;
        (*it)->id = ID;
    }
    // 输入有误
    else return ERROR1;
    autoSave();
    return SUCCESS;
}

int CoreOperations::sort(int opinion)
{
    if(DataBase.size() == 0) return ERROR0;
    switch (opinion)
    {
    // 按编号升序
    case 0:
        std::sort(DataBase.begin(), DataBase.end(), [&](const Workers* w0, const Workers* w1)
        {
            return std::stol(w0->id) < std::stol(w1->id);
        });
        break;
    // 按编号降序
    case 1:
        std::sort(DataBase.begin(), DataBase.end(), [&](const Workers* w0, const Workers* w1)
        {
            return std::stol(w0->id) > std::stol(w1->id);
        });
        break;
    // 按姓名升序
    case 2:
        std::sort(DataBase.begin(), DataBase.end(), [&](const Workers* w0, const Workers* w1)
        {
            return w0->name < w1->name;
        });
        break;
    // 按姓名降序
    case 3:
        std::sort(DataBase.begin(), DataBase.end(), [&](const Workers* w0, const Workers* w1)
        {
            return w0->name > w1->name;
        });
        break;
    // 输入有误
    default:
        return ERROR1;
    }
    autoSave();
    return SUCCESS;
}

void CoreOperations::deleteAllEmployeeInfo(void)
{
    for(auto it = DataBase.begin(); it != DataBase.end(); ++it)
    {
        Workers* temp = (*it);
        delete temp;
        temp = nullptr;
    }
    DataBase.clear();
    workersInfo.open("D:\\Staff Management System\\demo\\workersInfo.txt", std::ios::out);
    workersInfo.clear();
    workersInfo.close();
}