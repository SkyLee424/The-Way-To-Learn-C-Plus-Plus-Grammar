#pragma once
#include <iostream>
#include "status.h"

namespace Nstd
{
    template <class T>
    class MyArray;

    template <typename T>
    std::ostream &operator<<(std::ostream &cout, MyArray<T> &arr);

    template <typename T>
    std::ostream &operator<<(std::ostream &cout, const std::vector<T> &arr);

    /**
     *  @brief A Non-standard container which offers fixed time access to
     *  individual elements in any order.
     *
     *  @ingroup sequences
     *
     *  @tparam T  Type of element.
     */
    template <class T>
    class MyArray
    {
        friend std::ostream &operator<< <>(std::ostream &cout, MyArray<T> &arr);

    private:
        T *array;
        int _size;
        int _capacity;
        int curIndex;

    public:
        /**
         * @brief Construct a new MyArray object with no elements.
         *
         */
        MyArray(void);

        /**
         * @brief Construct a new MyArray object
         *
         * @param size The number of elements to initially create.
         */

        MyArray(int size);

        /**
         *  @brief  Creates a %MyArray with default constructed elements.
         *  @param  size  The number of elements to initially create.
         *  @param  val  The value of elements.
         *
         *  This constructor fills the %MyArray with @a val default
         *  constructed elements.
         */
        MyArray(int size, T val);

        ~MyArray();

        MyArray(const MyArray &arr);

        MyArray<T> &operator=(const MyArray &arr);

        // T operator[](int index) const;

        T &operator[](int index);

        int size(void) const;

        int capacity(void) const;

        void push_back(T val);

        void pop_back(void);
    };
}

using namespace Nstd;

template <class T>
MyArray<T>::MyArray()
{
    this->_size = DEFAULT_SIZE;
    this->_capacity = DEFAULT_CAPACITY;
    curIndex = 0;
    this->array = new T[_capacity];
}

template <class T>
MyArray<T>::MyArray(int size)
{
    this->_size = size;
    this->_capacity = size;
    curIndex = size; // 一开始没考虑到
    this->array = new T[_capacity];
}

template <class T>
MyArray<T>::MyArray(int size, T val)
{
    this->_size = size;
    this->_capacity = size;
    curIndex = size; // 一开始没考虑到
    this->array = new T[_capacity];
    for (int i = 0; i < _capacity; ++i)
    {
        this->array[i] = val;
    }
}

template <class T>
MyArray<T>::~MyArray()
{
    if (array)
    {
        
        delete []array;
        array = nullptr; // 防止野指针
    }
}

template <class T>
MyArray<T>::MyArray(const MyArray &arr)
{
    this->_size = arr._size;
    this->_capacity = arr._capacity;
    this->curIndex = arr.curIndex;
    this->array = new T[_capacity];
    for (int i = 0; i < arr.curIndex; ++i)
    {
        this->array[i] = arr.array[i];
    }
}

// 一开始没考虑到
// 为了实现连等，返回引用
template <class T>
MyArray<T> &MyArray<T>::operator=(const MyArray &arr)
{
    // 一开始没考虑到
    // 如果原来有数据，先释放
    if (this->array)
    {
        delete []this->array;
        this->array = nullptr;
    }
    this->_size = arr._size;
    this->_capacity = arr._capacity;
    this->curIndex = arr.curIndex;
    this->array = new T[_capacity];
    for (int i = 0; i < arr.curIndex; ++i)
    {
        this->array[i] = arr.array[i];
    }
    return *this;
}

template <typename T>
T &MyArray<T>::operator[](int index)
{
    // // 越界
    // if(index < 0 || index > _capacity) return OUT_OF_RANGE;
    // // 未定义
    // if(index >= curIndex) return UNDEFINED;
    return array[index];
}

template <typename T>
int MyArray<T>::size(void) const
{
    return _size;
}

template <typename T>
int MyArray<T>::capacity(void) const
{
    return _capacity;
}

template <typename T>
void MyArray<T>::push_back(T val)
{
    if (curIndex >= _capacity)
    {
        T *temp = new T[_capacity * 2];
        int i = 0;
        for (; i < _capacity; ++i)
        {
            temp[i] = array[i];
        }
        temp[i] = val;
        delete[] array;
        array = temp;
        _capacity *= 2;
    }
    else
    {
        array[curIndex] = val;
    }
    ++curIndex;
    ++_size;
}

template <typename T>
void MyArray<T>::pop_back(void)
{
    curIndex = std::max(0, curIndex - 1);
    _size = std::max(0, _size - 1);
}

// 重载左移运算符，用于 Nstd::MyArray 的输出
// 注意：仅支持基本数据类型
template <typename T>
std::ostream &Nstd::operator<<(std::ostream &cout, MyArray<T> &arr)
{
    for (int i = 0; i < arr.curIndex; ++i)
        std::cout << arr[i] << " ";
    return cout;
}

// 重载左移运算符，用于 std::vector 的输出
// 注意：仅支持基本数据类型
template <typename T>
std::ostream &Nstd::operator<<(std::ostream &cout, const std::vector<T> &arr)
{
    for (auto a : arr)
        std::cout << a << " ";
    return cout;
}