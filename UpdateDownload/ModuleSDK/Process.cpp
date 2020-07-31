#include "Process.h"

Mod_Process::Mod_Process() {

}
Mod_Process::~Mod_Process() {

}
//������ȡID(������);
DWORD Mod_Process::ProcessNameTakeID(const char *ProcessName) {
	HANDLE Snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Snap == INVALID_HANDLE_VALUE)return 0;
	PROCESSENTRY32 ProcessEntry32 = { sizeof(ProcessEntry32),0 };
	Process32First(Snap, &ProcessEntry32);
	Mod_Text Text;
	//������ʱ�洢�ڴ�;
	UINT Max_Path = MAX_PATH * sizeof(char);
	char *ProcName = (char*)malloc(Max_Path), *Tmpchar = (char*)malloc(Max_Path);
	if (!ProcName)return 0; if (!Tmpchar) { free(ProcName); return 0; }
	//�ڴ������;
	ZeroMemory(ProcName, Max_Path); ZeroMemory(Tmpchar, Max_Path);
	//ת��Сд����бȽ�;
	strcpy(ProcName, ProcessName);
	Text.TextToLower(ProcName);
	do
	{
		strcpy(Tmpchar, ProcessEntry32.szExeFile);
		Text.TextToLower(Tmpchar);
		if (strcmp(ProcName, Tmpchar) == 0) { CloseHandle(Snap); return ProcessEntry32.th32ProcessID; }
	} while (Process32Next(Snap, &ProcessEntry32));
	CloseHandle(Snap);
	return 0;
}
//������ȡID(������);
DWORD Mod_Process::ProcessNameTakeID(const wchar_t *ProcessName) {
	HANDLE Snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Snap == INVALID_HANDLE_VALUE)return 0;
	PROCESSENTRY32W ProcessEntry32 = { sizeof(ProcessEntry32),0 };
	Process32FirstW(Snap, &ProcessEntry32);
	Mod_Text Text;
	//������ʱ�洢�ڴ�;
	UINT Max_Path = MAX_PATH * sizeof(wchar_t);
	wchar_t *ProcName = (wchar_t*)malloc(Max_Path), *Tmpchar = (wchar_t*)malloc(Max_Path);
	if (!ProcName)return 0; if (!Tmpchar) { free(ProcName); return 0; }
	//�ڴ������;
	ZeroMemory(ProcName, Max_Path); ZeroMemory(Tmpchar, Max_Path);
	//ת��Сд����бȽ�;
	wcscpy(ProcName, ProcessName);
	Text.TextToLower(ProcName);
	do
	{
		wcscpy(Tmpchar, ProcessEntry32.szExeFile);
		Text.TextToLower(Tmpchar);
		if (wcscmp(ProcName, Tmpchar) == 0) { CloseHandle(Snap); return ProcessEntry32.th32ProcessID; }
	} while (Process32NextW(Snap, &ProcessEntry32));
	CloseHandle(Snap); 
	return 0;
}
//������ȡ���ھ��(������);
BOOL WINAPI EnumWindowsProc(HWND hWnd, LPARAM lParam) {
	DWORD PID = 0, *ptmp = (DWORD*)lParam;
	GetWindowThreadProcessId(hWnd, &PID);
	if (PID == ptmp[0]) { ptmp[1] = (DWORD)hWnd; return FALSE; }
	else return TRUE;
}
HWND Mod_Process::ProcessIDTakeHWND(DWORD pid) {
	DWORD pTmp[2] = { pid, 0 };
	EnumWindows(EnumWindowsProc, (LPARAM)pTmp);
	return (HWND)pTmp[1];
}
//��ȡ����ģ���ַ(����ID,ģ����);
DWORD Mod_Process::GetProcessModuleBaseAddress(DWORD pid, const char *ModuleName) {
	HANDLE Snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, pid);
	if (Snap == INVALID_HANDLE_VALUE)return NULL;
	MODULEENTRY32 ModuleEntry32 = { sizeof(ModuleEntry32),0 };
	Module32First(Snap, &ModuleEntry32);
	//������ʱ�洢�ڴ�;
	UINT Max_Path = MAX_PATH * sizeof(char);
	char *pTmpchar = (char*)malloc(Max_Path), *pTmpchar2 = (char*)malloc(Max_Path);
	if (!pTmpchar)return 0; if (!pTmpchar2) { free(pTmpchar); return 0; }
	//�ڴ������;
	ZeroMemory(pTmpchar, Max_Path); ZeroMemory(pTmpchar2, Max_Path);
	//ת��Сд����бȽ�;
	Mod_Text Text;
	CopyMemory(pTmpchar, ModuleName, strlen(ModuleName) * sizeof(char));
	Text.TextToLower(pTmpchar);
	do 
	{
		CopyMemory(pTmpchar2, ModuleEntry32.szModule, (strlen(ModuleEntry32.szModule) * sizeof(char)) + sizeof(char));
		Text.TextToLower(pTmpchar2);
		if (strcmp(pTmpchar, pTmpchar2) == 0) { free(pTmpchar); free(pTmpchar2); return (DWORD)ModuleEntry32.modBaseAddr; }
	} while (Module32Next(Snap, &ModuleEntry32));
	free(pTmpchar); free(pTmpchar2);
	return NULL;
}
//��ȡ����ģ���ַ(����ID,ģ����);
DWORD Mod_Process::GetProcessModuleBaseAddress(DWORD pid, const wchar_t *ModuleName) {
	HANDLE Snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, pid);
	if (Snap == INVALID_HANDLE_VALUE)return NULL;
	MODULEENTRY32W ModuleEntry32 = { sizeof(ModuleEntry32),0 };
	Module32FirstW(Snap, &ModuleEntry32);
	//������ʱ�洢�ڴ�;
	UINT Max_Path = MAX_PATH * sizeof(wchar_t);
	wchar_t *pTmpchar = (wchar_t*)malloc(Max_Path), *pTmpchar2 = (wchar_t*)malloc(Max_Path);
	if (!pTmpchar)return 0; if (!pTmpchar2) { free(pTmpchar); return 0; }
	//�ڴ������;
	ZeroMemory(pTmpchar, Max_Path); ZeroMemory(pTmpchar2, Max_Path);
	//ת��Сд����бȽ�;
	Mod_Text Text;
	CopyMemory(pTmpchar, ModuleName, wcslen(ModuleName) * sizeof(char));
	Text.TextToLower(pTmpchar);
	do
	{
		CopyMemory(pTmpchar2, ModuleEntry32.szModule, (wcslen(ModuleEntry32.szModule) * sizeof(wchar_t)) + sizeof(wchar_t));
		Text.TextToLower(pTmpchar2);
		if (wcscmp(pTmpchar, pTmpchar2) == 0) { free(pTmpchar); free(pTmpchar2); return (DWORD)ModuleEntry32.modBaseAddr; }
	} while (Module32NextW(Snap, &ModuleEntry32));
	free(pTmpchar); free(pTmpchar2);
	return NULL;
}
//��������(������);
BOOL Mod_Process::TerminateProcess(const char *ProcessName) {
	DWORD pid = 0;
	if (!ProcessName)pid = GetCurrentProcessId();
	else {
		Mod_Process Process;
		pid = Process.ProcessNameTakeID(ProcessName);
	}
	HANDLE HProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!HProcess)return FALSE;
	::TerminateProcess(HProcess, NULL);
	CloseHandle(HProcess);
	return TRUE;
}
//��������(������);
BOOL Mod_Process::TerminateProcess(const wchar_t *ProcessName) {
	DWORD pid = 0;
	if (!ProcessName)pid = GetCurrentProcessId();
	else {
		Mod_Process Process;
		pid = Process.ProcessNameTakeID(ProcessName);
	}
	HANDLE HProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!HProcess)return FALSE;
	::TerminateProcess(HProcess, NULL);
	CloseHandle(HProcess);
	return TRUE;
}
//��������(����ID);
BOOL Mod_Process::TerminateProcess(DWORD pid) {
	DWORD lpid = pid;
	if (!lpid)lpid = GetCurrentProcessId();
	HANDLE HProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, lpid);
	if (!HProcess)return FALSE;
	::TerminateProcess(HProcess, NULL);
	CloseHandle(HProcess);
	return TRUE;
}
//�����Ƿ����(������);
BOOL  Mod_Process::ProcessIsExists(const char *ProcessName) {
	HANDLE Snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Snap == INVALID_HANDLE_VALUE)return FALSE;
	PROCESSENTRY32 ProcessEntry32 = { sizeof(ProcessEntry32),0 };
	Process32First(Snap, &ProcessEntry32);
	Mod_Text Text;
	//������ʱ�洢�ڴ�;
	UINT Max_Path = MAX_PATH * sizeof(char);
	char *ProcName = (char*)malloc(Max_Path), *Tmpchar = (char*)malloc(Max_Path);
	if (!ProcName)return 0; if (!Tmpchar) { free(ProcName); return 0; }
	//�ڴ������;
	ZeroMemory(ProcName, Max_Path); ZeroMemory(Tmpchar, Max_Path);
	strcpy(ProcName, ProcessName);
	Text.TextToLower(ProcName);
	do
	{
		strcpy(Tmpchar, ProcessEntry32.szExeFile);
		Text.TextToLower(Tmpchar);
		if (strcmp(ProcName, Tmpchar) == 0) { CloseHandle(Snap); return TRUE; }
	} while (Process32Next(Snap, &ProcessEntry32));
	CloseHandle(Snap);
	return 0;
}
//�����Ƿ����(������);
BOOL  Mod_Process::ProcessIsExists(const wchar_t *ProcessName) {
	HANDLE Snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Snap == INVALID_HANDLE_VALUE)return FALSE;
	PROCESSENTRY32W ProcessEntry32 = { sizeof(ProcessEntry32),0 };
	Process32FirstW(Snap, &ProcessEntry32);
	Mod_Text Text;
	//������ʱ�洢�ڴ�;
	UINT Max_Path = MAX_PATH * sizeof(wchar_t);
	wchar_t *ProcName = (wchar_t*)malloc(Max_Path), *Tmpchar = (wchar_t*)malloc(Max_Path);
	if (!ProcName)return 0; if (!Tmpchar) { free(ProcName); return 0; }
	//�ڴ������;
	ZeroMemory(ProcName, Max_Path); ZeroMemory(Tmpchar, Max_Path);
	wcscpy(ProcName, ProcessName);
	Text.TextToLower(ProcName);
	do
	{
		wcscpy(Tmpchar, ProcessEntry32.szExeFile);
		Text.TextToLower(Tmpchar);
		if (wcscmp(ProcName, Tmpchar) == 0) { CloseHandle(Snap); return TRUE; }
	} while (Process32NextW(Snap, &ProcessEntry32));
	CloseHandle(Snap);
	return FALSE;
}
//�����Ƿ����(����ID);
BOOL  Mod_Process::ProcessIsExists(DWORD pid) {
	HANDLE Snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (Snap == INVALID_HANDLE_VALUE)return FALSE;
	PROCESSENTRY32 ProcessEntry32 = { sizeof(ProcessEntry32),0 };
	Process32First(Snap, &ProcessEntry32);
	do
	{
		if (pid == ProcessEntry32.th32ProcessID) { CloseHandle(Snap); return TRUE; }
	} while (Process32Next(Snap, &ProcessEntry32));
	CloseHandle(Snap);
	return FALSE;
}
//���н���(�ļ���,��������)
void Mod_Process::ProcessCreate(const char *pFileName, const char *pCmd)
{
	ShellExecute(0, "open", pFileName, pCmd, NULL, SW_SHOWDEFAULT);
	return;
}
//���н���(�ļ���,��������)
void Mod_Process::ProcessCreate(const wchar_t *pFileName, const wchar_t *pCmd)
{
	ShellExecuteW(0, L"open", pFileName, pCmd, NULL, SW_SHOWDEFAULT);
	return;
}