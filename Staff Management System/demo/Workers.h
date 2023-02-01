#pragma once
#include<iostream>
// 抽象职工类
class Workers
{
protected:
    // 展示职工基本信息
    void showBasicInfo(void);
public:
    // 职工姓名
    std::string name;
    // 职工编号
    std::string id;
    // 性别
    char sex;
    // 年龄
    int age;
    // 部门编号
    // 0. 员工 1. 经理 2. 老板
    std::string departmentId;
    // 虚析构函数
    virtual ~Workers(){}
    // 展示职工信息
    virtual void showInfo(void) = 0;
    // 获取职工部门
    virtual std::string getDeptName(void) = 0;
};

// 员工类
class Staff : public Workers
{
public:
    Staff(const std::string &name, char sex, int age, const std::string &id);
    // 展示员工信息
    void showInfo(void);
    
    std::string getDeptName(void);
};

// 经理类
class Manager : public Workers
{
public:
    Manager(const std::string &name, char sex, int age, const std::string &id);
    // 展示经理信息
    void showInfo(void);

    std::string getDeptName(void);
};

// 老板类
class Boss : public Workers
{
public:
    Boss(const std::string &name, char sex, int age, const std::string &id);
    // 展示老板信息
    void showInfo(void);
   
    std::string getDeptName(void);
};