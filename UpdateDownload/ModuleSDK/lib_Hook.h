#pragma once
#ifndef _LIB_HOOK_H
#define _LIB_HOOK_H
#include <windows.h>
#include <tchar.h>
#include <vector>
//x86-x64����
namespace lib_Hook
{
	struct HookInfo
	{
		void *pHookAddress = nullptr;
		size_t HookLength = 0;
		void *pNewFun = nullptr;
		void *pOldFunCall = nullptr;
		void *pAlloc = nullptr;
	};
	class Hook
	{
	public:
		Hook();
		~Hook();
		//����Hook
		//@parameter Ŀ���ַ
		//@parameter ��ַҪ�����ַ����
		//@parameter �µĺ�����ַ
		void* _fastcall SetHook(void* DestAddress, size_t length, void* NewFunction);
		//ж��Hook
		//@parameter Ŀ���ַ
		//@parameter �µĺ�����ַ
		bool _fastcall UnHook(void* DestAddress, void* NewFunction);
		//��ͣHook
		//@parameter Ŀ���ַ
		//@parameter �Ƿ���ͣHook
		bool _fastcall SetHookState(void* DestAddress, void* NewFunction, bool IsPause);
		//��ѯHook��Ϣ
		//@parameter Ŀ���ַ
		//@parameter �µĺ�����ַ
		HookInfo _fastcall QueryHookInfo(void* DestAddress, void* NewFunction);
		//��Ŀ���ַ���������ڴ�
		//@parameter Ŀ���ַ
		//@parameter ��С
		void* _fastcall LocalAlloc(void* DestAddres, size_t size);
		//�ͷ�������ڴ�
		//@parameter Ŀ���ַ
		//@parameter ��������ڴ��С
		void _fastcall LocalFree(void* lpbuff);
		//�ͷ�����HOOK;
		void _fastcall ReleaseHook();
	private:
		
	};
}

#endif // !_LIB_HOOK_H
