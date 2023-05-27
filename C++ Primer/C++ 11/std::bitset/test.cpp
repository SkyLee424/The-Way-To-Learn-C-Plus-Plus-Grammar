#include <iostream>
#include <vector>

template <typename T>
std::ostream &operator<<(std::ostream &cout, const std::vector<T> &arr)
{
    for (const T &num : arr)
        std::cout << num << " ";
    return cout;
}

// 使用 unsigned 值初始化 bitset
// std::bitset<size_t> b(val);

void test0(void)
{
    unsigned long long a = 18;    // 10010
    std::bitset<3> bin0(a);       // 010 ，多余删去
    std::bitset<10> bin1(a);      // 00000 10010 ，超过补 0
    std::bitset<128> bin2(~0ull); // 前 64 位是 0，后 64 位是 1
    std::cout << bin0 << std::endl;
    std::cout << bin1 << std::endl;
    std::cout << bin2 << std::endl;
}

// std::bitset<size_t> bin(std::string, begin = 0, len = std::npos, zero = '0', one = '1')

void test1(void)
{
    std::string s = "10111";
    std::bitset<16> bin0(s);               // 0000 0000 0001 0111
    std::bitset<4> bin1(s);                // 1011
    std::bitset<32> bin2(s, 1, 4);         // 0000 .... 0111（使用 s[1] ~ s[4]）
    std::bitset<32> bin3(s, s.size() - 2); // 0000 .... 0011（使用最后两个字符）
    std::cout << bin0 << std::endl;
    std::cout << bin1 << std::endl;
    std::cout << bin2 << std::endl;
    std::cout << bin3 << std::endl;
}

// bitset 操作
// 注意：二进制的位与我们的习惯是相反的
// 例如：0011
// 第 0 位为 1

void test2(void)
{
    std::string s = "001001";
    std::bitset<16> binvec(s); // 0000 0000 0000 1001
    int sz = binvec.size();    // 一个 constexpr 函数，这里返回 16
    int cnt = binvec.count();  // 含 1 的个数，这里是 2
    std::cout << "size = " << sz << ", count = " << cnt << std::endl;
    std::vector<bool> vec;
    vec.push_back(binvec.all());   // false 因为有 0
    vec.push_back(binvec.none());  // false 因为有 1
    vec.push_back(binvec.any());   // true 因为有 1
    vec.push_back(binvec.test(0)); // true，最后一位为 1
    // 下标运算符有两个版本
    // 1. const 版本：返回 bool 值
    // 2. 非 const 版本：返回一个特殊类型，可以修改指定位置的值
    vec.push_back(static_cast<bool>(binvec[1])); // false
    std::cout << vec << std::endl;               // 0 0 1 1 0
}

// bitset 操作
// 这里的操作大多有重载版本
// 其中，无参的会改变整个 bitset
// 有参的只改变指定位置的值

void test3(void)
{
    std::string s = "1001";
    std::bitset<4> binvec(s); // 1001

    // set(pos, val = true)
    binvec.set(0, false);             // 将第 0 位置为 0
    std::cout << binvec << std::endl; // 1000
    // set() => 全部置 1
    binvec.set();                     // 全 1
    std::cout << binvec << std::endl; // 1111

    // reset(pos) => 将 pos 位置 0
    binvec.reset(0);
    std::cout << binvec << std::endl; // 1110
    // reset() => 全部置 0
    binvec.reset();
    std::cout << binvec << std::endl; // 0000

    // filp(pos) => 翻转 pos 的值
    binvec.flip(0);
    std::cout << binvec << std::endl; // 0001
    // filp() => 全部翻转
    binvec.flip();
    std::cout << binvec << std::endl; // 1110

    binvec[1] = false;
    std::cout << binvec << std::endl; // 1100
}

// 提取 bitset 的值

void test4(void)
{
    std::string s = "1010";
    std::bitset<64> binvec0(s);
    std::bitset<256> binvec1(std::string(256, '1'));
    unsigned long num0 = binvec0.to_ulong(); // 正确
    // 错误，256位 超过了 ull 可以存储的值
    // std::overflow_error: bitset to_ullong overflow error
    // unsigned long num1 = binvec1.to_ullong();
}

// I/O

void test5(void)
{
    std::bitset<8> bit;
    // 读入 bit 时，先保存到临时 std::string 对象
    // 读到 8 个字符，或者读到不是 0 或者 1 的字符时停止
    // 如果读取的字符数 < sz，高位置 0
    // 01012 => 0000 0101
    std::cin >> bit;
    std::cout << bit << std::endl;
}

// 使用 bitset 十分方便
void test6(void)
{
    // 评分程序
    unsigned long students = 0;
    students |= (1ul << 20);             // 表示第 20 位学生通过
    bool check = students & (1ul << 20); // 如果第 20 位学生通过，那么 check == true
    students &= ~(1ul << 20);            // 表示第 20 位学生未通过

    // 使用 bitset
    std::bitset<32> _students(0ul);
    _students.set(20, true); // 表示第 20 位学生通过
    bool _check = _students.test(20);
    _students.set(20, false);
}

void test(void)
{
    unsigned int val = ((1 << 1) | (1 << 2) | (1 << 3) | (1 << 5) |
                        (1 << 8) | (1 << 13) | (1 << 21));
    std::bitset<32> bitvec0(val);
    std::vector<int> stream = {1, 2, 3, 5, 8, 13, 21};
    std::bitset<32> bitvec1;
    for (const int pos : stream)
    {
        bitvec1.set(pos, 1);
    }
    std::cout << bitvec0.to_ulong() << std::endl;
    std::cout << bitvec1.to_ulong() << std::endl;
}

template <std::size_t questionNum>
class StudentJudgeSolution
{
    std::bitset<questionNum> questions;

public:
    StudentJudgeSolution() = default;
    StudentJudgeSolution(const std::string &answer, char zero = '0', char one = '1')
    {
        check(answer, zero, one);
        for (std::size_t i = 0; i < questionNum; ++i)
            modify(i, answer[i] != zero);
    }

    void modify(std::size_t pos, bool val)
    {
        questions.set(pos, val);
    }

    double getScore(const std::string &answer, char zero = '0', char one = '1')
    {
        check(answer, zero, one);
        int cnt = 0;
        for (int i = 0; i < questionNum; ++i)
        {
            if (questions.test(i) == (answer[i] == one))
                ++cnt;
        }
        return (cnt / static_cast<double>(questionNum)) * 100ul;
    }

private:
    void check(const std::string &answer, char zero = '0', char one = '1')
    {
        if (answer.size() != questionNum)
            throw std::runtime_error("学生做答数与题目数不相等");
        std::string temp;
        temp.push_back(zero);
        temp.push_back(one);
        if (answer.find_first_not_of(temp) != std::string::npos)
            throw std::runtime_error("无效参数");
    }
};

int main(void)
{
    StudentJudgeSolution<6> s("010111");
    std::string ans = "101011";
    std::cout << s.getScore(ans) << std::endl;
}