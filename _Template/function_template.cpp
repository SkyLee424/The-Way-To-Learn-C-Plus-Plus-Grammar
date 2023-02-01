#include<iostream>
#include<vector>
#include<queue>

template<typename T>
void mySwap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

template<typename T>
void func(void)
{
    std::cout << "Calling func." << std::endl;
}

// 升序排序
template<typename T>
void mySort(std::vector<T> &arr, int L, int R)
{
    if(L >= R) return;
    T pivot = arr[L];
    int l = L, r = R;
    while (l < r)
    {
        while (l < r && arr[r] >= pivot) --r;
        if(l < r) arr[l] = arr[r];
        
        while (l < r && arr[l] <= pivot) ++l;
        if(l < r) arr[r] = arr[l];
    }
    arr[l] = pivot;
    mySort(arr, L, r - 1);
    mySort(arr, r + 1, R);
}

// 打印数组
template<typename T>
std::ostream& operator<<(std::ostream& cout, const std::vector<T> &arr)
{
    for(const T &num : arr)
        std::cout << num << " ";
    std::cout << std::endl;
    return cout;
}

// 普通函数
int add0(const int &a, const int &b)
{
    return a + b;
}

// 模板函数
template<typename T>
T add1(const T &a, const T &b)
{
    return a + b;
}

// 普通函数
void func1(const int &a, const int &b)
{
    std::cout << "Calling normal func." << std::endl;
}

// 模板函数
template<typename T>
void func1(const T &a, const T &b)
{
    std::cout << "Calling template func." << std::endl;
}

// 重载的模板函数
template<typename T>
void func1(const T &a, const T &b, const T &c)
{
    std::cout << "Calling overload template func." << std::endl;
}

class Test
{
public:

    // 基本语法
    void test0(void)
    {
        int a = 1, b = 2;
        double c = 1.1, d = 2.2;
        mySwap(a, b); // 自动类型推导
        mySwap<double>(c, d); // 显式指定
        std::cout << a << " " << b << std::endl;
        std::cout << c << " " << d << std::endl;
    }

    // 使用函数模板的注意事项
    void test1(void)
    {
        // 1. 自动类型推导，必须推导出一致的数据类型

        int a = 1;
        char b = 'b';
        // mySwap(a, b); // 错误，数据类型不一致

        // 2. 模板必须要确定了 T 的数据类型才能使用

        // func(); // 错误，未指定参数类型
        func<int>(); // 正确，指定参数类型
    }

    // 利用模板，实现多种类型数组的排序
    void test2(void)
    {
        int n;
        std::cin >> n;
        std::vector<int> arr0(n);
        std::vector<double> arr1(n);
        for(int i = 0; i < n; ++i)
        {
            std::cin >> arr0[i];
        }
        for(int i = 0; i < n; ++i)
        {
            std::cin >> arr1[i];
        }
        mySort(arr0, 0, n - 1);
        mySort(arr1, 0, n - 1);
        std::cout << arr0 << arr1;
    }

    // 普通函数与模板函数的区别
    // 1. 普通函数可以发生隐式类型转换
    // 2. 模板函数，在使用自动类型推导时，不会发生隐式类型转换
    // 3. 模板函数，指定参数类型后，可以发生隐式类型转换
    // 因此，建议使用模板函数时，自己指定参数类型，尽量不要使用自动类型推导
    void test3(void)
    {
        int a = 10;
        char b = 'a';
        // 发生隐式类型转换：char -> int
        std::cout << add0(a, b) << std::endl;
        // 无法发生隐式类型转换，报错
        // std::cout << add1(a, b) << std::endl;
        // 指定参数后，可以发生隐式类型转换
        std::cout << add1<int>(a, b) << std::endl; // char -> int
        std::cout << add1<char>(a, b) << std::endl; // int -> char
    }

    // 普通函数与模板函数的调用规则
    // 1. 如果普通函数与模板函数均能实现，优先调用普通函数
    // 2. 添加空模板参数列表实现强制调用模板函数
    // 3. 函数模板可以发生重载
    // 4. 如果函数模板可以更好的匹配，则调用函数模板
    // 总结：在实际开发中，提供了函数模板，就不要再提供相同名称的普通函数，容易引发二义性
    void test4(void)
    {
        int a = 10, b = 10, c = 10;
        func1(a, b); // 1. 优先调用普通函数
        func1<>(a, b); // 2. 强制调用模板函数
        func1(a, b, c); // 3. 函数模板可以发生重载
        char d = 'd', e = 'e';
        func1(d, e); // 4. 虽然普通函数也可调用（发生隐式类型转换），但模板函数更好匹配，调用模板函数
    }

};
int main(void)
{
    Test test;
    int opinion;
    std::cin >> opinion;
    switch (opinion)
    {
    case 0:
        test.test0();
        break;
    case 1:
        test.test1();
        break;
    case 2:
        test.test2();
        break;
    case 3:
        test.test3();
        break;
    case 4:
        test.test4();
        break;
    default:
        break;
    }
}