#include "lib.h"
//
//char* convert2Char_needfree(LPCTSTR str) {
//	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
//	char *ptxtTemp = new char[len + 1];
//	WideCharToMultiByte(CP_ACP, 0, str, -1, ptxtTemp, len, NULL, NULL);
//	return ptxtTemp;
//}
//char* convert2Char_needfree(LPWSTR str) {
//	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
//	char *ptxtTemp = new char[len + 1];
//	WideCharToMultiByte(CP_ACP, 0, str, -1, ptxtTemp, len, NULL, NULL);
//	return ptxtTemp;
//}
//char* convert2Char_needfree(std::string str) {
//	int len = str.size();
//	char *ptxtTemp = new char[len + 1];
//	for (int i = 0; i < len; i++) ptxtTemp[i] = str[i];
//	ptxtTemp[len] = '\0';
//	return ptxtTemp;
//}
//LPCTSTR convert2LPCTSTR_needfree(char *str) {
//	int num = MultiByteToWideChar(0, 0, str, -1, NULL, 0);
//	wchar_t *wide = new wchar_t[num];
//	MultiByteToWideChar(0, 0, str, -1, wide, num);
//	return wide;
//}
//LPCTSTR convert2LPCTSTR_needfree(std::string str) {
//	char *tmp = convert2Char_needfree(str);
//	LPCTSTR res = convert2LPCTSTR_needfree(tmp);
//	delete tmp;
//	return res;
//}
void messagebox(LPCTSTR str) {
	MessageBox(NULL, str, "", 0);
}
void messagebox(LPCTSTR str, LPCTSTR title) {
	MessageBox(NULL, str, title, 0);
}
void messagebox(char *str) {
	//LPCTSTR lpTxt = convert2LPCTSTR_needfree(str);
	MessageBox(NULL, str, "", 0);
	//delete lpTxt;
}
void messagebox(char* str,char* title) {
	//LPCTSTR lpTxt = convert2LPCTSTR_needfree(str);
	//LPCTSTR lpTit = convert2LPCTSTR_needfree(title);
	MessageBox(NULL, str, title, 0);
	//delete lpTxt;
	//delete lpTit;
}
float Angle2Radian(float ang) {
	return ang*(2.0f*D3DX_PI) / 360.0f;
}
float Radian2Angle(float rad) {
	return rad / ((2.0f*D3DX_PI) / 360.0f);
}