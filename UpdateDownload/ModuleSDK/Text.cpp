#include "Text.h"
#pragma warning(disable:4996)

Mod_Text::Mod_Text() {
	srand((UINT)GetTickCount64());
}
Mod_Text::~Mod_Text() {
	if (TmpAlloc)free(TmpAlloc);
}
//ȡ���ı����(�ַ���������,ȡ������);
char *Mod_Text::GetTextLeft(const char *TextBuff, UINT TakeLen) {
	if (!TextBuff)return nullptr;
	void *pTmpp = nullptr;
	if (TmpAlloc) { 
		UINT NewTmpLen = strlen(TextBuff) * sizeof(char);
		pTmpp = malloc(NewTmpLen);
		if (!pTmpp)return nullptr;
		memset(pTmpp, NULL, NewTmpLen);
		CopyMemory(pTmpp, TextBuff, NewTmpLen);
		free(TmpAlloc); 
		TmpAlloc = nullptr;
	}
	else pTmpp = (void *)TextBuff;
	UINT sSize = TakeLen * sizeof(char);
	TmpAlloc = malloc(sSize + sizeof(char));
	if (!TmpAlloc)return nullptr;
	ZeroMemory(TmpAlloc, sSize + sizeof(char));
	CopyMemory(TmpAlloc, pTmpp, sSize);
	return (char*)TmpAlloc;
}
//ȡ���ı����(�ַ���������,ȡ������);
wchar_t *Mod_Text::GetTextLeft(const wchar_t *TextBuff, UINT TakeLen) {
	if (!TextBuff)return nullptr;
	void *pTmpp = nullptr;
	if (TmpAlloc) {
		UINT NewTmpLen = wcslen(TextBuff) * sizeof(wchar_t);
		pTmpp = malloc(NewTmpLen);
		if (!pTmpp)return nullptr;
		ZeroMemory(pTmpp, NewTmpLen);
		CopyMemory(pTmpp, TextBuff, NewTmpLen);
		free(TmpAlloc);
		TmpAlloc = nullptr;
	}
	else pTmpp = (void *)TextBuff;
	UINT sSize = TakeLen * sizeof(wchar_t);
	TmpAlloc = malloc(sSize + sizeof(wchar_t));
	if (!TmpAlloc)return nullptr;
	ZeroMemory(TmpAlloc, sSize + sizeof(wchar_t));
	CopyMemory(TmpAlloc, pTmpp, sSize);
	return (wchar_t*)TmpAlloc;
}
//ȡ���ı��ұ�(�ַ���������,ȡ������);
char *Mod_Text::GetTextRight(const char *TextBuff, UINT TakeLen) {
	if (!TextBuff)return nullptr;
	void *pTmpp = nullptr;
	if (TmpAlloc) {
		UINT NewTmpLen = strlen(TextBuff) * sizeof(char);
		pTmpp = malloc(NewTmpLen);
		if (!pTmpp)return nullptr;
		ZeroMemory(pTmpp, NewTmpLen);
		CopyMemory(pTmpp, TextBuff, NewTmpLen);
		free(TmpAlloc);
		TmpAlloc = nullptr;
	}
	else pTmpp = (void *)TextBuff;
	UINT slen = strlen((char*)pTmpp) * sizeof(char), sSize = TakeLen * sizeof(char);
	TmpAlloc = malloc(sSize + sizeof(char));
	if (!TmpAlloc)return nullptr;
	ZeroMemory(TmpAlloc, sSize + sizeof(char));
	char *pTmp = (char*)pTmpp + (slen - sSize);
	CopyMemory(TmpAlloc, pTmp, sSize);
	return (char*)TmpAlloc;
}
//ȡ���ı��ұ�(�ַ���������,ȡ������);
wchar_t *Mod_Text::GetTextRight(const wchar_t *TextBuff, UINT TakeLen) {
	if (!TextBuff)return nullptr;
	void *pTmpp = nullptr;
	if (TmpAlloc) {
		UINT NewTmpLen = wcslen(TextBuff) * sizeof(wchar_t);
		pTmpp = malloc(NewTmpLen);
		if (!pTmpp)return nullptr;
		ZeroMemory(pTmpp, NewTmpLen);
		CopyMemory(pTmpp, TextBuff, NewTmpLen);
		free(TmpAlloc);
		TmpAlloc = nullptr;
	}
	else pTmpp = (void *)TextBuff;
	UINT slen = wcslen((wchar_t*)pTmpp) * sizeof(wchar_t), sSize = TakeLen * sizeof(wchar_t);
	TmpAlloc = malloc(sSize + sizeof(wchar_t));
	if (!TmpAlloc)return nullptr;
	ZeroMemory(TmpAlloc, sSize + sizeof(wchar_t));
	char *pTmp = (char*)pTmpp + (slen - sSize);
	CopyMemory(TmpAlloc, pTmp, sSize);
	return (wchar_t*)TmpAlloc;
}
//ȡ���ı��м�����(�ַ���������,����ı�,�ұ��ı�);
char *Mod_Text::GetTextMiddle(const char *TextBuff, const char *TextLeft, const char *TextRight) {
	if (!TextBuff)return nullptr;
	char *pTmpTextBuff = nullptr;
	UINT sLen = 0;//��¼�ҵ����ı�����
	BOOL IsTmpAlloc = FALSE;//�Ƿ��������ʱ�ڴ�
	//�ж�֮ǰ�Ƿ��Ѿ�������ڴ�,�������������ʱ�洢Ȼ���ͷ�;
	if (TmpAlloc) {
		UINT NewTmpLen = strlen(TextBuff) * sizeof(char) + sizeof(char);
		pTmpTextBuff = (char*)malloc(NewTmpLen);
		if (!pTmpTextBuff)return nullptr;
		ZeroMemory(pTmpTextBuff, NewTmpLen);
		CopyMemory(pTmpTextBuff, TextBuff, NewTmpLen);
		IsTmpAlloc = TRUE;
		free(TmpAlloc);
		TmpAlloc = nullptr;
	}
	else pTmpTextBuff = (char*)TextBuff;
	//Ѱ������ı�λ��;
	char *pTmpl = strstr(pTmpTextBuff, TextLeft);
	if (!pTmpl)return nullptr;
	//����ҵ���¼�ı�λ��,���ƶ�������ı�����;
	char *pTmplpos = (char*)pTmpl + (strlen(TextLeft) * sizeof(char));
	//Ѱ���ұ��ı�λ��;
	char *pTmpr = strstr(pTmplpos, TextRight);
	//�ж��Ƿ��ҵ��ұ��ı�,����ҵ����Ҳ�������ı�λ���ظ�,��ȡ����;
	if (pTmpr != pTmplpos && pTmpr != NULL)sLen = (UINT)pTmpr - (UINT)pTmplpos;
	else sLen = strlen(pTmplpos) * sizeof(char);
	//����洢ȡ���ı��Ļ�����;
	TmpAlloc = malloc(sLen + sizeof(char));
	//�������ʧ��,�ж����������ڴ�,�����ͷ�;
	if (!TmpAlloc) { if (IsTmpAlloc)free(pTmpTextBuff); return nullptr; }
	ZeroMemory(TmpAlloc, sLen + sizeof(char));
	//������Ҫȡ�����ı�����ʱ�洢��;
	CopyMemory(TmpAlloc, pTmplpos, sLen);
	if (IsTmpAlloc)free(pTmpTextBuff);
	return (char*)TmpAlloc;
}
//ȡ���ı��м�����(�ַ���������,����ı�,�ұ��ı�);
wchar_t *Mod_Text::GetTextMiddle(const wchar_t *TextBuff, const wchar_t *TextLeft, const wchar_t *TextRight) {
	if (!TextBuff)return nullptr;
	wchar_t *pTmpTextBuff = nullptr;
	UINT sLen = 0;//��¼�ҵ����ı�����
	BOOL IsTmpAlloc = FALSE;//�Ƿ��������ʱ�ڴ�
	//�ж�֮ǰ�Ƿ��Ѿ�������ڴ�,�������������ʱ�洢Ȼ���ͷ�;
	if (TmpAlloc) {
		UINT NewTmpLen = wcslen(TextBuff) * sizeof(wchar_t) + sizeof(wchar_t);
		pTmpTextBuff = (wchar_t*)malloc(NewTmpLen);
		if (!pTmpTextBuff)return nullptr;
		ZeroMemory(pTmpTextBuff, NewTmpLen);
		CopyMemory(pTmpTextBuff, TextBuff, NewTmpLen);
		IsTmpAlloc = TRUE;
		free(TmpAlloc);
		TmpAlloc = nullptr;
	}
	else pTmpTextBuff = (wchar_t*)TextBuff;
	//Ѱ������ı�λ��;
	wchar_t *pTmpl = wcsstr(pTmpTextBuff, TextLeft);
	if (!pTmpl)return nullptr;
	//����ҵ���¼�ı�λ��,���ƶ�������ı�����;
	wchar_t *pTmplpos = (wchar_t*)((char*)pTmpl + (wcslen(TextLeft) * sizeof(wchar_t)));
	//Ѱ���ұ��ı�λ��;
	wchar_t *pTmpr = wcsstr(pTmplpos, TextRight);
	//�ж��Ƿ��ҵ��ұ��ı�,����ҵ����Ҳ�������ı�λ���ظ�,��ȡ����;
	if (pTmpr != pTmplpos && pTmpr != NULL)sLen = (UINT)pTmpr - (UINT)pTmplpos;
	else sLen = wcslen(pTmplpos) * sizeof(wchar_t);
	//����洢ȡ���ı��Ļ�����;
	TmpAlloc = malloc(sLen + sizeof(wchar_t));
	//�������ʧ��,�ж����������ڴ�,�����ͷ�;
	if (!TmpAlloc) { if (IsTmpAlloc)free(pTmpTextBuff); return nullptr; }
	ZeroMemory(TmpAlloc, sLen + sizeof(wchar_t));
	//������Ҫȡ�����ı�����ʱ�洢��;
	CopyMemory(TmpAlloc, pTmplpos, sLen);
	if (IsTmpAlloc)free(pTmpTextBuff);
	return (wchar_t*)TmpAlloc;
}
//�ı��滻(�ַ���������,�����ı�,Ԥ�滻�ı�);
char *Mod_Text::TextReplace(const char *TextBuff, const char *SearchText, const char *Replacestrbuf) {
	if (!TextBuff)return false;
	void *pTmpp = nullptr;
	if (TmpAlloc) {
		UINT NewTmpLen = (strlen(TextBuff) * sizeof(char)) + sizeof(char);
		pTmpp = malloc(NewTmpLen);
		if (!pTmpp)return nullptr;
		ZeroMemory(pTmpp, NewTmpLen);
		CopyMemory(pTmpp, TextBuff, NewTmpLen);
		free(TmpAlloc);
		TmpAlloc = nullptr;
	}
	else pTmpp = (void *)TextBuff;
	UINT SrcTxtLen = strlen((char*)pTmpp), ReplaceTxtlen = strlen(Replacestrbuf);
	UINT SrcSize = SrcTxtLen * sizeof(char), ReplaceSize = ReplaceTxtlen * sizeof(char);
	char *pcSearch = strstr((char*)pTmpp, SearchText);
	if (!pcSearch)return (char*)pTmpp;
	UINT Lstrpos = (UINT)pcSearch - (UINT)pTmpp;
	TmpAlloc = malloc(SrcSize + ReplaceSize + sizeof(char));
	if (!TmpAlloc)return nullptr;
	void *Tmppp = TmpAlloc;
	ZeroMemory(Tmppp, SrcSize + ReplaceSize + sizeof(char));
	CopyMemory(Tmppp, pTmpp, Lstrpos);
	Tmppp = (char*)Tmppp + Lstrpos;
	CopyMemory(Tmppp, Replacestrbuf, ReplaceSize);
	Tmppp = (char*)Tmppp + ReplaceSize;
	char *pch = (char*)pcSearch + (strlen(SearchText) * sizeof(char));
	UINT CopyLen = strlen(pch) * sizeof(char);
	CopyMemory(Tmppp, pch, CopyLen);
	Tmppp = (char*)Tmppp + CopyLen;
	Tmppp = (char*)Tmppp + sizeof(char);
	ZeroMemory(Tmppp, 1);
	return (char*)TmpAlloc;
}
//�ı��滻(�ַ���������,�����ı�,Ԥ�滻�ı�);
wchar_t *Mod_Text::TextReplace(const wchar_t *TextBuff, const wchar_t *SearchText, const wchar_t *Replacestrbuf) {
	if (!TextBuff)return false;
	void *pTmpp = nullptr;
	if (TmpAlloc) {
		UINT NewTmpLen = (wcslen(TextBuff) * sizeof(wchar_t)) + sizeof(wchar_t);
		pTmpp = malloc(NewTmpLen);
		if (!pTmpp)return nullptr;
		ZeroMemory(pTmpp, NewTmpLen);
		CopyMemory(pTmpp, TextBuff, NewTmpLen);
		free(TmpAlloc);
		TmpAlloc = nullptr;
	}
	else pTmpp = (void *)TextBuff;
	UINT SrcTxtLen = wcslen((wchar_t*)pTmpp), ReplaceTxtlen = wcslen(Replacestrbuf);
	UINT SrcSize = SrcTxtLen * sizeof(wchar_t), ReplaceSize = ReplaceTxtlen * sizeof(wchar_t);
	void *pcSearch = wcsstr((wchar_t*)pTmpp, SearchText);
	if (!pcSearch)return (wchar_t*)pTmpp;
	UINT Lstrpos = (UINT)pcSearch - (UINT)pTmpp;
	TmpAlloc = malloc(SrcSize + ReplaceSize + sizeof(wchar_t));
	if (!TmpAlloc)return nullptr;
	void *Tmppp = TmpAlloc;
	ZeroMemory(Tmppp, SrcSize + ReplaceSize + sizeof(wchar_t));
	CopyMemory(Tmppp, pTmpp, Lstrpos);
	Tmppp = (char*)Tmppp + Lstrpos;
	CopyMemory(Tmppp, Replacestrbuf, ReplaceSize);
	Tmppp = (char*)Tmppp + ReplaceSize;
	void *pch = (char*)pcSearch + (wcslen(SearchText) * sizeof(wchar_t));
	UINT CopyLen = wcslen((wchar_t*)pch) * sizeof(wchar_t);
	CopyMemory(Tmppp, pch, CopyLen);
	Tmppp = (char*)Tmppp + CopyLen;
	Tmppp = (char*)Tmppp + sizeof(wchar_t);
	ZeroMemory(Tmppp, 1);
	return (wchar_t*)TmpAlloc;
}
//�ı�ת��д(�ַ���������);
void Mod_Text::TextToUpper(char *TextBuff) {
	if (!TextBuff)return;
	UINT sLen = strlen(TextBuff);
	for (UINT i = 0; i < sLen; i++) {
		if (TextBuff[i] >= 'a'&&TextBuff[i] <= 'z') {
			TextBuff[i] = TextBuff[i] + ('A' - 'a');
		}
	}
}
//�ı�ת��д(�ַ���������);
void Mod_Text::TextToUpper(wchar_t *TextBuff) {
	if (!TextBuff)return;
	UINT sLen = wcslen(TextBuff);
	for (UINT i = 0; i < sLen; i++) {
		if (TextBuff[i] >= 'a'&&TextBuff[i] <= 'z') {
			TextBuff[i] = TextBuff[i] + ('A' - 'a');
		}
	}
}
//�ı�תСд(�ַ���������);
void Mod_Text::TextToLower(char *TextBuff) {
	if (!TextBuff)return;
	UINT sLen = strlen(TextBuff);
	for (UINT i = 0; i < sLen; i++) {
		if (TextBuff[i] >= 'A'&&TextBuff[i] <= 'Z') {
			TextBuff[i] = TextBuff[i] + ('a' - 'A');
		}
	}
}
//�ı�תСд(�ַ���������);
void Mod_Text::TextToLower(wchar_t *TextBuff) {
	if (!TextBuff)return;
	UINT sLen = wcslen(TextBuff);
	for (UINT i = 0; i < sLen; i++) {
		if (TextBuff[i] >= 'A'&&TextBuff[i] <= 'Z') {
			TextBuff[i] = TextBuff[i] + ('a' - 'A');
		}
	}
}
//���������ĸ�ַ���(��Ҫ���ַ���Ŀ);
char *Mod_Text::RandText(UINT RandNumber) {
	if (TmpAlloc) { free(TmpAlloc); TmpAlloc = nullptr; }
	UINT TmpLen = RandNumber * sizeof(char) + sizeof(char);
	TmpAlloc = malloc(TmpLen);
	if (!TmpAlloc)return nullptr;
	ZeroMemory(TmpAlloc, TmpLen);
	char *pTmp = (char*)TmpAlloc;
	for (UINT i = 0; i < RandNumber; i++) {
		UINT rint = rand() % 3;
		switch (rint)
		{
		case 0:
			//pTmp[i] = (rand() % ((0xF7 - 0xB0) + 1) + 0xB0);
			pTmp[i] = rand() % (('9' - '0') + 1) + '0';
			break;
		case 1:
			pTmp[i] = rand() % (('z' - 'a') + 1) + 'a';
			break;
		case 2:
			pTmp[i] = rand() % (('Z' - 'A') + 1) + 'A';
			break;
		default:
			break;
		}
	}
	return (char*)TmpAlloc;
}
//���������ĸ�ַ���(��Ҫ���ַ���Ŀ);
wchar_t *Mod_Text::RandTextW(UINT RandNumber) {
	if (TmpAlloc) { free(TmpAlloc); TmpAlloc = nullptr; }
	UINT TmpLen = RandNumber * sizeof(wchar_t) + sizeof(wchar_t);
	TmpAlloc = malloc(TmpLen);
	if (!TmpAlloc)return nullptr;
	ZeroMemory(TmpAlloc, TmpLen);
	wchar_t *pTmp = (wchar_t*)TmpAlloc;
	for (UINT i = 0; i < RandNumber; i++) {
		UINT rint = rand() % 3;
		switch (rint)
		{
		case 0:
			pTmp[i] = rand() % (('9' - '0') + 1) + '0';
			break;
		case 1:
			pTmp[i] = rand() % (('z' - 'a') + 1) + 'a';
			break;
		case 2:
			pTmp[i] = rand() % (('Z' - 'A') + 1) + 'A';
			break;
		default:
			break;
		}
	}
	return (wchar_t*)TmpAlloc;
}
//��������ַ���(��Ҫ�ĺ�����Ŀ)
char *Mod_Text::RandChineseText(UINT RandNumber) {
	if (TmpAlloc) { free(TmpAlloc); TmpAlloc = nullptr; }
	UINT TmpLen = RandNumber * (sizeof(char) + sizeof(char)) + sizeof(char);
	TmpAlloc = malloc(TmpLen);
	if (!TmpAlloc)return nullptr;
	ZeroMemory(TmpAlloc, TmpLen);
	char *pTmp = (char*)&TmpAlloc;
	//ANSI:�����������뷶Χ���ֽڴ�B0-F7,���ֽڴ�A1-FE;
	UINT cout = 0;
	for (UINT i = 0; i < RandNumber; i++) {
		pTmp[cout] = rand() % ((0xF7 - 0xB0) + 1) + 0xB0;
		cout++;
		pTmp[cout] = rand() % ((0xFE - 0xA1) + 1) + 0xA1;
		cout++;

	}
	return (char*)TmpAlloc;
}
//��������ַ���(��Ҫ�ĺ�����Ŀ)
wchar_t *Mod_Text::RandChineseTextW(UINT RandNumber) {
	if (TmpAlloc) { free(TmpAlloc); TmpAlloc = nullptr; }
	UINT TmpLen = RandNumber * sizeof(wchar_t) + sizeof(wchar_t);
	TmpAlloc = malloc(TmpLen);
	if (!TmpAlloc)return nullptr;
	ZeroMemory(TmpAlloc, TmpLen);
	wchar_t *pTmp = (wchar_t *)TmpAlloc;
	//Unicode ��������ĺ�����Ϊ4E00-9FA5, ����20902������
	for (UINT i = 0; i < RandNumber; i++) {
		pTmp[i] = rand() % ((0x9FA5 - 0x4E00) + 1) + 0x4E00;
	}
	return (wchar_t*)TmpAlloc;
}

