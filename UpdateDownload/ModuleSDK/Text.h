#pragma once
#ifndef Text_H
#define Text_H
#include <windows.h>
#include <time.h>

class Mod_Text {
public:
	Mod_Text();
	~Mod_Text();
	//ȡ���ı����(�ַ���������,ȡ������);
	char *GetTextLeft(const char *TextBuff, UINT TakeLen);
	//ȡ���ı����(�ַ���������,ȡ������);
	wchar_t *GetTextLeft(const wchar_t *TextBuff, UINT TakeLen);
	//ȡ���ı��ұ�(�ַ���������,ȡ������);
	char *GetTextRight(const char *TextBuff, UINT TakeLen);
	//ȡ���ı��ұ�(�ַ���������,ȡ������);
	wchar_t *GetTextRight(const wchar_t *TextBuff, UINT TakeLen);
	//ȡ���ı��м�����(�ַ���������,����ı�,�ұ��ı�);
	char *GetTextMiddle(const char *TextBuff, const char *TextLeft, const char *TextRight);
	//ȡ���ı��м�����(�ַ���������,����ı�,�ұ��ı�);
	wchar_t *GetTextMiddle(const wchar_t *TextBuff, const wchar_t *TextLeft, const wchar_t *TextRight);
	//�ı��滻(�ַ���������,�����ı�,Ԥ�滻�ı�);
	char *TextReplace(const char *TextBuff, const char *SearchText, const char *Replacestrbuf);
	//�ı��滻(�ַ���������,�����ı�,Ԥ�滻�ı�);
	wchar_t *TextReplace(const wchar_t *TextBuff, const wchar_t *SearchText, const wchar_t *Replacestrbuf);
	//�ı�ת��д(�ַ���������);
	void TextToUpper(char *TextBuff);
	//�ı�ת��д(�ַ���������);
	void TextToUpper(wchar_t *TextBuff);
	//�ı�תСд(�ַ���������);
	void TextToLower(char *TextBuff);
	//�ı�תСд(�ַ���������);
	void TextToLower(wchar_t *TextBuff);
	//����ַ���(��Ҫ���ַ���Ŀ);
	char *RandText(UINT RandNumber);
	//����ַ���(��Ҫ���ַ���Ŀ);
	wchar_t *RandTextW(UINT RandNumber);
	//��������ַ���(��Ҫ�ĺ�����Ŀ)
	char *RandChineseText(UINT RandNumber);
	//��������ַ���(��Ҫ�ĺ�����Ŀ)
	wchar_t *RandChineseTextW(UINT RandNumber);
private:
	void *TmpAlloc = nullptr;
};
#endif
