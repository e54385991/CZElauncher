#pragma once
#ifndef _LIB_PRCOESS_H
#define _LIB_PROCESS_H
#include <windows.h>
#include <iostream>
#include <sstream>
#include <tchar.h>
#include <TlHelp32.h>

namespace lib_Process
{
	class Process
	{
	public:
		Process();
		~Process();
		//PID��ȡ���ھ��
		//@parameter PID
		HWND _fastcall PidToHwnd(DWORD pid);
		//��������ȡ���ھ��
		//@parameter ������
		HWND _fastcall ProcessNameToHwnd(const TCHAR* ProcessName);
		//��������ȡPID
		//@parameter ������
		DWORD _fastcall ProcessNameToPid(const TCHAR* ProcessName);
		//�������Ƿ����
		//@parameter ������
		bool _fastcall IsProcesNameExist(const TCHAR* ProcessName);
		//����ID�Ƿ����
		//@parameter ������
		bool _fastcall IsPidExist(DWORD pid);
		//��ȡģ���ַ
		//@parameter ������
		//@parameter ģ����
		DWORD _fastcall GetModuleAddress(const TCHAR* ProcessName, const TCHAR* ModuleName);
		//���̴���
		//@parameter �ļ���
		//@parameter ���в���
		void _fastcall ProcessRun(const TCHAR* FileName, const TCHAR* Parameter);
	private:

	};

}

#endif

