#include <iostream>

struct destination
{
    int *pos;
    destination()
    {
        pos = new int[INT32_MAX];
    }
    // 没有析构函数（准确的说，是有一个空的析构函数，它不做任何事情）
};

struct connection
{
    destination *dest;
    connection(destination *dest)
    {
        this->dest = dest;
    }
    // 没有析构函数（准确的说，是有一个空的析构函数，它不做任何事情）
};

void disconnect(connection c)
{
    if(c.dest->pos)
    {
        delete []c.dest->pos;
        c.dest->pos = nullptr;
    }
}

void test0(void)
{
    destination *dest;
    connection c(dest);
    // do something...

    // 如果此处抛出异常，并且在 test 中没有被捕获
    // 那么 c.dest->pos 就无法得到释放了（connection 没有析构函数）
    throw std::exception();
    disconnect(c);
}

// 自定义删除操作
void endConnection(connection *c)
{
    disconnect(*c);
}

// 解决方法：使用智能指针
void test1(void)
{
    destination *dest = new destination();
    connection c(dest);
    std::shared_ptr<connection> p(&c, endConnection);
    // do something...

    throw std::exception();
    // 结束 test1
    // 就算在结束前发生异常，c.dest->pos 也能正确释放
}

int main(void)
{
    test1();
}