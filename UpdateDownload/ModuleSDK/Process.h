#pragma once
#ifndef Process_H
#define Process_H
#include "Text.h"
#include <windows.h>
#include <TlHelp32.h>
#pragma warning(disable:4996)
class Mod_Process {
public:
	Mod_Process();
	~Mod_Process();
	//������ȡID(������);
	DWORD ProcessNameTakeID(const char *ProcessName);
	//������ȡID(������);
	DWORD ProcessNameTakeID(const wchar_t *ProcessName);
	//����IDȡ���ھ��(������);
	HWND ProcessIDTakeHWND(DWORD pid);
	//��ȡ����ģ���ַ(����ID,ģ����);
	DWORD GetProcessModuleBaseAddress(DWORD pid, const char *ModuleName);
	//��ȡ����ģ���ַ(����ID,ģ����);
	DWORD GetProcessModuleBaseAddress(DWORD pid, const wchar_t *ModuleName);
	//��������(������);
	BOOL TerminateProcess(const char *ProcessName);
	//��������(������);
	BOOL TerminateProcess(const wchar_t *ProcessName);
	//��������(����ID);
	BOOL TerminateProcess(DWORD pid);
	//�����Ƿ����(������);
	BOOL ProcessIsExists(const char *ProcessName);
	//�����Ƿ����(������);
	BOOL ProcessIsExists(const wchar_t *ProcessName);
	//�����Ƿ����(����ID);
	BOOL ProcessIsExists(DWORD pid);
	//���н���(�ļ���,��������)
	void ProcessCreate(const char *pFileName, const char *pCmd);
	//���н���(�ļ���,��������)
	void ProcessCreate(const wchar_t *pFileName, const wchar_t *pCmd);

private:

};
#endif

