#pragma once
#ifndef _Lib_String_H
#define _Lib_String_H
#include <windows.h>
#include <tchar.h>

#pragma warning(disable:4996)
namespace lib_String
{

	class CLibString
	{
	public:
		CLibString();
		~CLibString();
		CLibString(const char* lpstr);
		CLibString(const wchar_t* lpstr);

		//���������
		const CLibString& operator+(const CLibString& lpstr);
		void operator+=(const CLibString& lpstr);
		void operator=(const CLibString& lpstr);
		bool operator==(const CLibString& lpstr);
		bool operator==(const char* lpstr);
		bool operator==(const wchar_t* lpstr);

		//������������
		operator const wchar_t* () const noexcept;
		operator wchar_t* () const noexcept;
		operator const char* () const noexcept;
		operator char* () const noexcept;

		//��ȡ��С
		UINT wSize()const;
		UINT Size()const;
		//��ȡ�ַ�������
		UINT Length()const;
		//����
		//@parameter ת������
		void Decode(UINT nFlag);
		//Ѱ���ı�
		//@parameter ��Ѱ���ı�
		int Find(const char* lpFindstr, UINT nPos = NULL);
		//Ѱ���ı�
		//@parameter ��Ѱ���ı�
		int Find(const wchar_t* lpFindstr, UINT nPos = NULL);
		//ȡ�ı����
		//@parameter ��Ѱ���ı�
		//@parameter ȡ���ı�����
		CLibString Left(const CLibString& lpFindStr, UINT nCount)const;
		//ȡ�ı��ұ�
		//@parameter ��Ѱ���ı�
		//@parameter ȡ���ı�����
		CLibString Right(const CLibString& lpFindStr, UINT nCount)const;
		//ȡ�ı��м�
		//@parameter ��Ѱ������ı�
		//@parameter ��Ѱ���ұ��ı�
		CLibString Mid(const CLibString& lpLeftStr, const CLibString& lpRightStr, UINT nPos = NULL)const;
		//�ı��Ƚ�
		//@paramaeter ���Ƚ��ı�
		int Compare(const CLibString& lpstr);
	private:
		mutable char* m_lpstr;
		mutable wchar_t* m_lpstrw;
	};
}

#endif // !_Lib_String_H

