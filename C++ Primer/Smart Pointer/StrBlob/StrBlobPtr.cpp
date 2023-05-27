#include "StrBlobPtr.h"

StrBlobPtr::StrBlobPtr()
{
    curPos = 0;
    // 隐式的将 wp 初始化为空的 weak_ptr   
}

StrBlobPtr::StrBlobPtr(StrBlob &sb, std::size_t sz)
{
    curPos = sz;
    wp = sb.data;
}

StrBlobPtr& StrBlobPtr::operator++(void)
{
    check(curPos, "Increment past end of StrBlobPtr"); // 不能递增一个在尾后的 curPos
    ++curPos;
    return *this;
}

std::string& StrBlobPtr::operator*(void)
{
    auto p = check(curPos, "Try to get reference of an invaild index");
    return (*p)[curPos];
}

bool StrBlobPtr::operator!=(const StrBlobPtr &sb)
{
    return sb.wp.lock() != this->wp.lock() || sb.curPos != this->curPos;
}

std::shared_ptr<std::vector<std::string>> StrBlobPtr::check(std::size_t sz, const char* msg)
{
    auto p = wp.lock();
    // wp 没有绑定到任何 vector 上
    if(!p)
    {
        throw std::runtime_error("Derefence past end");
    }
    if(sz >= p->size())
    {
        throw std::out_of_range(msg);
    }
    return p;
}