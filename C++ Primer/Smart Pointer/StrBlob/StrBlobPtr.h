#pragma once
#include "StrBlob.h"

class StrBlobPtr
{
    std::size_t curPos;
    std::weak_ptr<std::vector<std::string>> wp;
public:
    StrBlobPtr();

    StrBlobPtr(StrBlob &sb, std::size_t sz = 0);

    /**
     * @brief 前置递增
     * 
     * @return StrBlobPtr& 
     */
    StrBlobPtr& operator++(void);

    /**
     * @brief 解引用操作符
     * 
     * @return std::string& 
     */
    std::string& operator*(void);

    bool operator!=(const StrBlobPtr &sb);

private:
    std::shared_ptr<std::vector<std::string>> check(std::size_t sz, const char* msg);
};