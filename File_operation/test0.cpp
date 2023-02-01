#include<iostream>
#include<fstream>
#include<string>
void write(void)
{
    std::ofstream writeFile;
    writeFile.open("D:\\C++\\test\\test.txt", std::ios::out | std::ios::trunc);
    writeFile << "int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {" << std::endl;
    writeFile << "    std::vector<std::vector<std::pair<int, int>>> map(n);" << std::endl;
    writeFile << "    for(auto &flight : flights)" << std::endl;
    writeFile << "    {" << std::endl;
    writeFile << "        int i = flight[0], j = flight[1], k = flight[2];" << std::endl;
    writeFile << "        map[flight[0]].push_back({flight[1], flight[2]});" << std::endl;
    writeFile << "    }" << std::endl;
    writeFile << "    return dijkstra(map, src, dst, k);" << std::endl;
    writeFile << "}" << std::endl;
    writeFile.close();
}
void read(void)
{
    std::ifstream readFile;
    readFile.open("D:\\C++\\test\\test.txt", std::ios::in);
    if (!readFile.is_open())
    {
        std::cout << "Can not open the file" << std::endl;
    }
    // // 方式 1

    // char s0[1024] = {0};
    // while (readFile >> s0)
    // {
    //     std::cout << s0 << std::endl;
    // }

    // // 方式 2

    // char s1[1024] = {0};
    // while (readFile.getline(s1, sizeof(s1)))
    // {
    //     std::cout << s1 << std::endl;
    // }

    // 方式 3

    std::string s2 = "";
    while (std::getline(readFile, s2))
    {
        std::cout << s2 << std::endl;
    }

    // // 方式 4

    // char ch;
    // while ((ch = readFile.get()) != EOF)
    // {
    //     std::cout << ch;
    // }

    readFile.close();
}
int main(void)
{
    write();
    read();
}