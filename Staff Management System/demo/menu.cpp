#include<iostream>
#include"CoreOperations.h"
#include"status.h"

Menu::Menu()
{
    coreOperations.load();
}

void Menu::displayMenu(void)
{
    system("cls");
    std::cout << " Welcome to the staff management system" << std::endl;
    std::cout << "Please select the action you want to take:" << std::endl;
    for (int i = 0; i < 43; ++i)
        std::cout << "-";
    std::cout << "\n";
    std::cout << "(0).Add employee information" << std::endl;
    std::cout << "(1).Display employee information" << std::endl;
    std::cout << "(2).Delete resigned employee" << std::endl;
    std::cout << "(3).Modify employee information" << std::endl;
    std::cout << "(4).Find employee information" << std::endl;
    std::cout << "(5).Sort employee information" << std::endl;
    std::cout << "(6).Delete all employee information" << std::endl;
    std::cout << "(7).Exit system" << std::endl;
}
int Menu::getAction(void)
{
    int action;
    std::cin >> action;
    getchar();
    return action;
}
int Menu::userInteraction(void)
{
    system("cls");
    displayMenu();
    int action = getAction();
    system("cls");
    switch (action)
    {
        case 0:
        
            CoreOperations::printLine("", 23);
            std::cout << "Add employee information";
            CoreOperations::printLine("", 23);
            std::cout << "\n" << std::endl;
            coreOperations.addWorkers();
            std::cout << "\nAll employees have been added." << std::endl;
            break;
        case 1:
            CoreOperations::printLine("", 29);
            std::cout << "Display employee information";
            CoreOperations::printLine("", 29);
            std::cout << "\n" << std::endl;
            if(!coreOperations.display())
            {
                for(int i = 0; i < 24; ++i) std::cout << " ";
                std::cout << "There are no employees in the database" << std::endl;
            }
            break;
        case 2:
        {
            CoreOperations::printLine("", 28);
            std::cout << "Delete resigned employee";
            CoreOperations::printLine("", 28);
            std::cout << "\n" << std::endl;
            int opinion;
            std::cout << "Please select the deletion method(0. Delete by name, 1. Delete by employee ID): ";
            std::cin >> opinion;
            getchar();
            // 保存姓名或 ID
            std::string s;
            if(opinion == 0)
            {
                std::cout << "\nPlease enter the name of the employee to be deleted: ";
            }
            else if(opinion == 1)
            {
                std::cout << "\nPlease enter the ID of the employee to be deleted: ";
            }
            else
            {
                std::cout << "\nInput error, please re-enter!\n";
                break;
            }
            std::getline(std::cin, s);
            if(coreOperations.deleteWorker(opinion, s))
            {
                std::cout << "\nSuccessfully deleted the employee in the database" << std::endl;
            }
            else std::cout << "\nFailed to delete, this employee does not exist in the database." << std::endl;
            break;
        }
        case 3:
        {
            CoreOperations::printLine("", 23);
            std::cout << "Modify employee information";
            CoreOperations::printLine("", 23);
            std::cout << "\n" << std::endl;
            int opinion;
            std::cout << "Please select the modification method(0. Modify by name, 1. Modify by employee ID): ";
            std::cin >> opinion;
            getchar();
            // 保存姓名或 ID
            std::string s;
            if(opinion == 0)
            {
                std::cout << "\nPlease enter the name of the employee to be modified: ";
            }
            else if(opinion == 1)
            {
                std::cout << "\nPlease enter the ID of the employee to be modified: ";
            }
            else
            {
                std::cout << "\nInput error, please re-enter!" << std::endl;
                break;
            }
            std::getline(std::cin, s);
            std::cout << "\n";
            int status = coreOperations.modifyWorker(opinion, s);
            if(status == SUCCESS)
            {
                std::cout << "\nSuccessfully modified the employee in the database." << std::endl;
            }
            else if(status == ERROR0)
            {
                std::cout << "Modification failed, database is empty." << std::endl;
            }
            else if(status == ERROR1)
            {
                std::cout << "\nInput error, please re-enter!" << std::endl;
            }
            break;
        }
        case 4:
        {
            CoreOperations::printLine("", 27);
            std::cout << "Find employee information";
            CoreOperations::printLine("", 27);
            std::cout << "\n" << std::endl;
            int opinion;
            std::cout << "Please select the search method(0. Search by name, 1. Search by employee ID): ";
            std::cin >> opinion;
            getchar();
            // 保存姓名或 ID
            std::string s;
            if(opinion == 0)
            {
                std::cout << "\nPlease enter the name of the employee to search: ";
            }
            else if(opinion == 1)
            {
                std::cout << "\nPlease enter the ID of the employee to search: ";
            }
            else
            {
                std::cout << "\nInput error, please re-enter!" << std::endl;
                break;
            }
            std::getline(std::cin, s);
            std::cout << "\n";
            auto it = coreOperations.findWorker(opinion, s);
            if(it != coreOperations.DataBase.end())
            {
                (*it)->showInfo();
                CoreOperations::printLine((*it)->getDeptName());
            }
            else
            {
                std::cout << "Lookup failed. The database is empty or there is no such member." << std::endl;
            }
            break;
        }
        case 5:
        {
            CoreOperations::printLine("", 27);
            std::cout << "Sort employee information";
            CoreOperations::printLine("", 27);
            std::cout << "\n" << std::endl;
            int opinion;
            std::cout << "(0). Sort by ID in ascending order\n";
            std::cout << "(1). Sort by ID in descending order\n";
            std::cout << "(2). Sort by name in ascending order\n";
            std::cout << "(3). Sort by name in descending order\n";
            std::cout << "\nPlease select the sort method: ";
            std::cin >> opinion;
            getchar();
            int status = coreOperations.sort(opinion);
            if(status == SUCCESS)
            {
                std::cout << "\nSuccessfully sorted the employee in the database." << std::endl;
            }
            else if(status == ERROR0)
            {
                std::cout << "Sort failed, database is empty!" << std::endl;
            }
            else if(status == ERROR1)
            {
                std::cout << "\nInput error, please re-enter!" << std::endl;
            }
            break;
        }
        case 6:
        {
            CoreOperations::printLine("", 27);
            std::cout << "Delete all employee information";
            CoreOperations::printLine("", 27);
            std::cout << "\n" << std::endl;
            char opinion;
            std::cout << "Are you sure you want to empty the database(Y/N)?: ";
            std::cin >> opinion;
            getchar();
            if(opinion == 'Y')
            {
                coreOperations.deleteAllEmployeeInfo();
                std::cout << "\nSuccessfully emptied the database." << std::endl;
            }
            else if(opinion != 'N')
            {
                std::cout << "\nInput error, please re-enter!" << std::endl;
            }
            break;
        }
        case 7:
            std::cout << "About to exit the system..." << std::endl;
            return EXIT;
        default:
            std::cout << "\nInput error, please re-enter!";
            break;
    }
    std::cout << std::endl;
    system("pause");
    return CONTINUE;
}