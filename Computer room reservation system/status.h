#pragma once
#define Status int
constexpr int InputError = 1;
constexpr int ExitSystem = 3;
constexpr int Success = 4;
constexpr int Failed = -1;
constexpr int NoFile = 6;
constexpr int NoSuchUser = 7;
constexpr int WrongPassword = 8;
constexpr int UnderReview = 9;
constexpr int Approved = 10;
constexpr int AuditFailed = 11;
constexpr int CancelOrder = 12;
// 某个用户没有预约
constexpr int NoOrder = 13; 
constexpr int CancelReview = 14;
constexpr int FullRoom = 15;
