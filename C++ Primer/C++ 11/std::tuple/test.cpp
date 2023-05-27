#include <iostream>
#include <string>
#include <vector>
#include <list>

// 定义和初始化 tuple
void test0(void)
{
    // 执行默认初始化
    std::tuple<int, std::string, std::vector<int>, std::list<int>> obj;
    // 可以给予初始值
    std::tuple<int, std::string> people(18, "DingZhen");
    // tuple 的构造函数是 explicit 的，因此，必须显式的初始化
    // 但是，C++ 17 引入了类模版推导，
    // 可以在很多情况下自动推导出模板参数类型，
    // 因此在一些情况下可以省略显式构造函数调用。
    // 但是，为了代码的可读性和可维护性，
    // 建议在使用 std::tuple 时仍然显式地调用构造函数。
    std::tuple<int, int, int> arr = {1,2,3}; // 不建议
    auto t = std::make_tuple(18, "DingZhen");
    auto t1 = std::make_tuple<int, std::string>(18, "DingZhen"); // 显式指定模版参数类型
}

// 访问 tuple 的成员
// get<i>(t)
// 返回 t 的第 i 个成员，如果 t 的第 i 个成员是左值，返回左值引用，否则，返回右值引用
void test1(void)
{
    auto obj = std::make_tuple<std::string, int, char>("DingZhen", 18, 'M');
    auto name = std::get<0>(obj);
    auto age = std::get<1>(obj);
    auto sex = std::get<2>(obj);
    std::get<0>(obj) = "MaBaoGuo";
    std::cout << std::get<0>(obj) << std::endl;
}

// 如果不知道 tuple 成员的具体信息，可以利用两个模版类来访问
// std::tuple_size<T>
// T 为 tuple 的类型，可以用 decltype 方便获取
// std::tuple_element<size_t _Ip, typename _Tp>
// _Ip 为元素的在 tuple 中的位置，_Tp 为 tuple 的类型

void test2(void)
{
    auto obj = std::make_tuple<std::string, int, char>("DingZhen", 18, 'M');
    std::size_t sz = std::tuple_size<decltype(obj)>::value; // 返回 3
    std::tuple_element<0, decltype(obj)>::type name = std::get<0>(obj);
    std::cout << sz << std::endl;
    std::cout << name << std::endl;
}

// 关系和相等运算符
// 为了使用关系运算符，需保证两个 tuple 的 size 相同，并且，每对成员之间支持 '<' 运算符
// 按照字典序比较
// 为了使用相等运算符，需保证两个 tuple 的 size 相同，并且，每对成员之间支持 '==' 运算符
// 使用每对成员的 '==' 运算符，如果发现不相等，停止比较
void test3(void)
{
    auto obj0 = std::make_tuple(1, 2, 3);
    auto obj1 = std::make_tuple(2, 3);
    auto obj2 = std::make_tuple("1", "2", "3");
    auto obj3 = std::make_tuple("2", "3", "4");
    bool b;
    // b = (obj0 == obj1); // 错误，size 不相等
    // b = (obj0 < obj1); // 错误，size 不相等
    b = (obj2 == obj3); // fasle
    b = (obj2 < obj3); // true
}

std::tuple<std::vector<int>::size_type, 
std::vector<int>::const_iterator,
std::vector<int>::const_iterator> func(void)
{
    static std::vector<int> nums = {1,2,2,2,3};
    auto p = std::equal_range(nums.begin(), nums.end(), 2);
    std::tuple<std::vector<int>::size_type, 
    std::vector<int>::const_iterator,
    std::vector<int>::const_iterator> ret;
    ret = std::make_tuple(p.first - nums.begin(), p.first, p.second);
    return ret;
}

void test4(void)
{
    auto t = func();
    std::cout << "pos = " << std::get<0>(t) << std::endl;
    for(auto it = std::get<1>(t); it != std::get<2>(t); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int main(void)
{
    test4();   
}