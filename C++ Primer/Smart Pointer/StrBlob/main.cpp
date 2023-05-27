#include <iostream>
#include "StrBlob.h"
#include "StrBlobPtr.h"

int main(void)
{
    StrBlob b0;
    StrBlob b1 = {"1", "1,2", "1,2,3"};
    b1.push_back("haha");
    for(auto i = b1.begin(); i != b1.end(); ++i)
    {
        std::cout << *i << std::endl;
    }
}