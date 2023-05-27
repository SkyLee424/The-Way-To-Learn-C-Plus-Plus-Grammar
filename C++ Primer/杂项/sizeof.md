# Sizeof 运算符

* 对引用对象执行 sizeof，返回被引用对象的大小
* 对指针执行sizeof，返回**指针本身所占空间大小**
    ```cpp
    int arr[4] = {0};
    int *p = arr;
    std::cout << sizeof(int*) << std::endl;
    std::cout << sizeof(arr) / sizeof(*arr) << std::endl; // 4
    std::cout << sizeof(p) / sizeof(*p) << std::endl; // 8 / 4 == 2
    ```
* 对解引用指针执行 sizeof，指针不需要有效（即没有初始化）
    ```cpp
    int *a = nullptr;
    std::cout << (sizeof *a) << std::endl; // 4
    ```

* 对数组执行 sizeof，等价于将数组中所有元素各执行一次 sizeof，求和

    ```cpp
    int arr[4] = {0};
    std::cout << (sizeof arr) << std::endl; // 16
    ```

* 对一个类执行 sizeof，sizeof **不会** 计算对象中的元素占了多少空间

    ```cpp
    struct test
    {
        int *a;
        test()
            :a(nullptr){}
    };
    int main(void)
    {
        test t;
        std::cout << (sizeof t) << std::endl; // 8
        t.a = new int(sizeof(int));
        std::cout << (sizeof t) << std::endl; // 8
    }
    ```
