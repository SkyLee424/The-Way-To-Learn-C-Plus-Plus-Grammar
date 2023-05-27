#include "A.h"

class Token;

std::ostream& operator<<(std::ostream &os, const Token &t);

class Token
{
    friend std::ostream& operator<<(std::ostream &os, const Token &t);
    union
    {
        int ival;
        char cval;
        std::string sval;
        A aval;
    }; // 匿名联合体

    enum {INT, CHAR, STRING, _A} tok; // 判别式（利用不限定作用域、匿名的枚举类型）

public:
    Token(int ival = 0)
        : tok(INT), ival(ival) {}
    Token(char cval)
        : tok(CHAR), cval(cval) {}
    Token(const std::string &sval)
        : tok(STRING), sval(sval) {}
    Token(const A &aval)
        : tok(_A), aval(aval) {}
    Token(const Token &t);
    Token &operator=(const Token& t);

    Token& operator=(int ival);
    Token& operator=(char cval);
    Token& operator=(const std::string &sval);
    Token& operator=(const A &aval);

    ~Token();

private:
    void copyUnion(const Token &t);
};

Token::Token(const Token &t)
    :tok(t.tok)
{
    copyUnion(t);
}

Token& Token::operator=(const Token& t)
{
    if(tok == STRING)
    {
        if(t.tok == STRING)
        {
            sval = t.sval; // 可以直接赋值
            return *this;
        }
        else sval.~basic_string(); // 需析构左侧对象联合体成员
    }
    else if(tok == _A)
    {
        if(t.tok == _A)
        {
            aval = t.aval;
            return *this;
        }
        else aval.~A();
    }
    copyUnion(t);
    tok = t.tok;
    return *this;
    
}

Token& Token::operator=(int ival)
{
    if(tok == STRING)
        sval.~basic_string();
    else if(tok == _A)
        aval.~A();
    this->ival = ival;
    tok = INT;
    return *this;
}

Token& Token::operator=(char cval)
{
    if(tok == STRING)
        sval.~basic_string();
    else if(tok == _A)
        aval.~A();
    this->cval = cval;
    tok = CHAR;
    return *this;
}

Token& Token::operator=(const std::string &sval)
{
    if(tok == _A)
        aval.~A();
    if(tok == STRING)
        this->sval = sval;
    else new(&(this->sval)) std::string(sval); // 定位 new 表达式（不分配内存，而是在指定的位置 construct 一个对象）
    tok = STRING;
    return *this;
}

Token& Token::operator=(const A &aval)
{
    if(tok == STRING)
        sval.~basic_string();
    if(tok == _A)
        this->aval = aval;
    else new (&(this->aval)) A(aval);
    tok = _A;
    return *this;
}

Token::~Token()
{
    if(tok == STRING)
        sval.~basic_string();
    else if(tok == _A)
        aval.~A();
}

// 注意，copyUnion 假定左侧运算对象已被正确析构
void Token::copyUnion(const Token &t)
{
    switch (t.tok)
    {
    case INT:
        ival = t.ival;
        break;
    case CHAR:
        cval = t.cval;
        break;
    case STRING:
        new(&sval) std::string(t.sval);
        break;
    case _A:
        new(&aval) A(t.aval);
        break;
    default:
        break;
    }
}

std::ostream& operator<<(std::ostream &os, const Token &t)
{
    os << "Token: " << (int)t.tok << " ,val: ";
    switch (t.tok)
    {
    case Token::INT:
        os << t.ival;
        break;
    case Token::CHAR:
        os << t.cval;
        break;
    case Token::STRING:
        os << t.sval;
        break;
    case Token::_A:
        os << t.aval;
        break;
    default:
        break;
    }

    return os;
}

void test0(void)
{
    A a(114, "514");
    std::cout << "---------------------" << std::endl;
    
    Token t0;
    Token t1 = 1;
    Token t2 = 'a';
    Token t3("烟distance");
    Token t4 = a;

    std::cout << t0 << std::endl;
    std::cout << t1 << std::endl;
    std::cout << t2 << std::endl;
    std::cout << t3 << std::endl;
    std::cout << t4 << std::endl;

    std::cout << "---------------------" << std::endl;
}

void test1(void)
{
    A a(114, "514");
    std::cout << "---------------------" << std::endl;
    
    Token t0;
    Token t1 = 1;
    Token t2 = 'a';
    Token t3("烟distance");
    Token t4 = a;
    std::cout << "---------------------" << std::endl;

    t0 = t1;
    t1 = t2;
    t2 = t3;
    t3 = t4;
    t4 = t0;
    std::cout << "---------------------" << std::endl;

    std::cout << t0 << std::endl;
    std::cout << t1 << std::endl;
    std::cout << t2 << std::endl;
    std::cout << t3 << std::endl;
    std::cout << t4 << std::endl;

    std::cout << "---------------------" << std::endl;
}

int main(void)
{
    test1();    
}