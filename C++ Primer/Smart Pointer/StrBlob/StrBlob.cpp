#include "StrBlob.h"
#include "StrBlobPtr.h"

StrBlob::StrBlob()
{
    data = std::make_shared<std::vector<std::string>>();
}

StrBlob::StrBlob(std::initializer_list<std::string> _list)
{
    data = std::make_shared<std::vector<std::string>>(_list);
}

void StrBlob::push_back(const std::string &s)
{
    data->push_back(s);
}

void StrBlob::pop_back()
{
    check(0, "pop_back on a empty StrBlob");
    data->pop_back();
}

std::string StrBlob::front()
{
    check(0, "Get front from a empty StrBlob");
    return data->front();
}

std::string StrBlob::front() const
{
    check(0, "Get front from a empty StrBlob");
    return data->front();
}

std::string StrBlob::back()
{
    check(0, "Get back from a empty StrBlob");
    return data->back();
}

std::string StrBlob::back() const
{
    check(0, "Get back from a empty StrBlob");
    return data->back();
}

StrBlobPtr StrBlob::begin()
{
    StrBlobPtr res(*this);
    return res;
}

StrBlobPtr StrBlob::end()
{
    StrBlobPtr res(*this, data->size());
    return res;
}

void StrBlob::check(size_t pos, const char *msg)
{
    if (pos >= data->size())
        throw std::out_of_range(msg);
}

void StrBlob::check(size_t pos, const char *msg) const
{
    if (pos >= data->size())
        throw std::out_of_range(msg);
}