#include<iostream>
#include<string>
#include<fstream>
class File
{
    std::fstream myFile;
public:
    
    void write(void)
    {
        system("cls");
        myFile.open("D:\\C++\\Grammar\\File_operation\\myFile.txt", std::ios::out | std::ios::trunc);
        std::string s = "------------------------------------------------------";
        myFile << s << std::endl;
        std::cout << "Enter what you want to save, ";
        std::cout << "double-click 'Enter' to end: " << std::endl;
        while (std::getline(std::cin, s))
        {   
            if(s.size() == 0) break;
            myFile << s << std::endl;
        }
        s = "------------------------------------------------------";
        myFile << s << std::endl;
        std::cout << "Your file has been saved." << std::endl;
        myFile.close();
    }

    void read(void)
    {
        system("cls");
        myFile.open("D:\\C++\\Grammar\\File_operation\\myFile.txt", std::ios::in);
        if(!myFile.is_open())
        {
            std::cout << "Can not read file!" << std::endl;
            return ;
        }
        std::cout << "Reading file..." << std::endl;
        std::cout << '\n';
        std::string s;
        while (std::getline(myFile, s))
        {
            std::cout << s << std::endl;
        }
        std::cout << '\n';
        std::cout << "File read completed." << std::endl;
        myFile.close();
    }
};

int main(void)
{
    File f;
    while (1)
    {
        std::cout << "Please select the action you want to take." << std::endl;
        std::cout << "(0). Write File" << std::endl;
        std::cout << "(1). Read File" << std::endl;
        std::cout << "(2). Exit the system" << std::endl;
        std::cout << "(3). Refresh screen" << std::endl;
        int action = 2;
        std::cin >> action;
        getchar();
        switch (action)
        {
        case 0:
            f.write();
            break;
        case 1:
            f.read();
            break;
        case 2:
            std::cout << "About to exit the system..." << std::endl;
            system("pause");
            return 0;
        case 3:
            system("cls");
            break;
        default:
            std::cout << "Input error, check your input!" << std::endl;
            break;
        }
    }
}