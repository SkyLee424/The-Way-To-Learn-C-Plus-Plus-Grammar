#pragma once
#include <iostream>
#include <vector>

class StrBlobPtr; // StrBlob 的伴随类

class StrBlob
{
    friend class StrBlobPtr;
    std::shared_ptr<std::vector<std::string>> data;
    using size_t = std::vector<std::string>::size_type;
public:
    StrBlob();

    StrBlob(std::initializer_list<std::string> _list);

    // 使用默认拷贝、赋值、析构
    
    void push_back(const std::string &s);

    void pop_back(void);

    std::string front(void);

    std::string front(void) const;

    std::string back(void);

    std::string back(void) const;
    
    StrBlobPtr begin();

    StrBlobPtr end();

private:
    void check(size_t pos, const char *msg);

    void check(size_t pos, const char *msg) const;
};