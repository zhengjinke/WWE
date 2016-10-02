#pragma once
#include "Common.h"
using namespace std;

//char* convert2Char_needfree(LPCTSTR str);
//char* convert2Char_needfree(LPWSTR str);
//char* convert2Char_needfree(std::string str);
//LPCTSTR convert2LPCTSTR_needfree(char* str);
//LPCTSTR convert2LPCTSTR_needfree(std::string str);
void messagebox(char* str);
void messagebox(char* str, char* title);
void messagebox(LPCTSTR str);
void messagebox(LPCTSTR str, LPCTSTR title);
float Angle2Radian(float ang);
float Radian2Angle(float rad);
