#include <iostream>
#pragma once

template <class T>
class shared_ptr;

template <typename T, typename ...Args>
shared_ptr<T> &make_shared(Args&& ...args);

template <class T>
class shared_ptr
{
    template <typename X, typename ...Args> // 注意这里的友元声明，X，而不是直接使用 T
    friend shared_ptr<X> &make_shared(Args&& ...args);

    int *_count;
    T *data;

    shared_ptr(T &&val) // 只给 make_shared 函数使用
        : _count(new int(0)), data(new T(std::move(val))) {}

public:
    shared_ptr()
        : _count(new int(0)), data(nullptr) {}
    shared_ptr(const shared_ptr &s)
        : _count(s._count), data(s.data)
    {
        ++*_count; // 递增被拷贝对象的引用计数
    }
    ~shared_ptr();
    shared_ptr &operator=(const shared_ptr &s);
    T &operator*(void);
    int use_count(void);
    T *get(void);
};

// 如果要验证 SIGBAT ，使用这个版本的 make_shared（记得改友元声明）
// template <typename T>
// shared_ptr<T> &make_shared(T &&val)
// {
//     shared_ptr<T> *res = new shared_ptr<T>(std::forward<T>(val));
//     return *res;
// }

template <typename T, typename ...Args>
shared_ptr<T> &make_shared(Args&& ...args)
{
    shared_ptr<T> *res = new shared_ptr<T>(T(std::forward<T>(args)...));
    return *res;
}

template <typename T>
shared_ptr<T>::~shared_ptr()
{
    if(--*_count <= 0)
    {
        delete _count;
        delete data;
        data = nullptr;
        _count = nullptr;
    }
}

template <typename T>
shared_ptr<T> &shared_ptr<T>::operator=(const shared_ptr<T> &s)
{
    ++*s._count;                // 先递增右侧对象的引用计数，如果发生自赋值也是安全的
    if (--*(this->_count) <= 0) // 递减引用计数，如果没有其他用户
    {
        delete this->_count; // 释放本对象的成员
        delete this->data;
        this->data = nullptr;
        this->_count = nullptr;
    }
    this->data = s.data;
    this->_count = s._count;
    return *this;
}

template <typename T>
inline T &shared_ptr<T>::operator*(void)
{
    return *data;
}

template <typename T>
inline int shared_ptr<T>::use_count(void)
{
    return *_count;
}

template <typename T>
inline T *shared_ptr<T>::get(void)
{
    return data;
}