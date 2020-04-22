// ChineseCode.h: interface for the CChineseCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHINESECODE_H__E9206178_CA96_4B8A_AB2A_AD94C9CD12B1__INCLUDED_)
#define AFX_CHINESECODE_H__E9206178_CA96_4B8A_AB2A_AD94C9CD12B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

class ChineseCode
{
public:
	static void UTF_8ToUnicode(wchar_t* pOut, char* pText);  // ��UTF-8ת����Unicode
	static void UnicodeToUTF_8(char* pOut, wchar_t* pText);  //Unicode ת����UTF-8
	static void UnicodeToGB2312(char* pOut, wchar_t uData);  // ��Unicode ת���� GB2312 
	static void Gb2312ToUnicode(wchar_t* pOut, char* gbBuffer);// GB2312 ת���ɡ�Unicode
	static void GB2312ToUTF_8(string& pOut, char* pText, int pLen);//GB2312 תΪ UTF-8
	static void UTF_8ToGB2312(string& pOut, char* pText, int pLen);//UTF-8 תΪ GB2312
};

class strCoding
{
public:
	strCoding(void);
	~strCoding(void);
	//utf_8תΪgb2312
	void UTF_8ToGB2312(string& pOut, char* pText, int pLen);
	//gb2312 תutf_8
	void GB2312ToUTF_8(string& pOut, char* pText, int pLen); 
	//urlgb2312����
	string UrlGB2312(char* str);                           
	//urlutf8 ����
	string UrlUTF8(char* str);                             
	//urlutf8����
	string UrlUTF8Decode(string str);                  
	//urlgb2312����
	string UrlGB2312Decode(string str);                

private:
	void Gb2312ToUnicode(WCHAR* pOut, char* gbBuffer);
	void UTF_8ToUnicode(WCHAR* pOut, char* pText);
	void UnicodeToUTF_8(char* pOut, WCHAR* pText);
	void UnicodeToGB2312(char* pOut, WCHAR uData);
	char CharToInt(char ch);
	char StrToBin(char* str);

};
#endif // !defined(AFX_CHINESECODE_H__E9206178_CA96_4B8A_AB2A_AD94C9CD12B1__INCLUDED_)
