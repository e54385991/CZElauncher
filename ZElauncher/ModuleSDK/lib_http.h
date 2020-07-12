#ifndef __lib_htpp_h
#define __lib_http_h
#include <windows.h>
#include <iostream>
#include <algorithm>
#include <wininet.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <tchar.h>
#include <openssl/ssl.h>

#include "lib_String.h"

#import <msscript.ocx>
//#import <vbscript.dll>rename("RGB","rgb")

#pragma comment(lib,"wininet.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libssl.lib")
#pragma warning(disable:4996)

namespace lib_http
{
	using namespace std;
	using namespace MSScriptControl;
	using namespace lib_String;
	//���ػص�����,����true��������,����falseֹͣ����;
	//@parameter �����ļ��ܴ�С
	//@parameter ��ǰ�������ݴ�С
	//@parameter ÿ���������ݴ�С
	typedef bool(__stdcall *DownloadProc)(size_t FileSize, UINT nState, size_t DownloadSize, size_t SecondSize);
	class CLibhttp
	{
	public:
		CLibhttp();
		~CLibhttp();
		//POST����
		//@parameter �����ַ
		//@parameter ��������
		//@parameter ��������
		//@parameter ����Э��ͷ
		//@parameter ����Cookies
		//@parameter ����Cokies
		bool _fastcall POST(std::string url, std::string &ResultData, std::string RequestData = "", std::string Header = "", std::string Cookies = "", std::string ReturnCookies = "");
		//POST����
		//@parameter �����ַ
		//@parameter ��������(����string����,ת������������)
		//@parameter ��������
		//@parameter ����Э��ͷ
		//@parameter ����Cookies
		//@parameter ����Cokies
		bool _fastcall POST(std::wstring url, std::string &ResultData, std::wstring RequestData = L"", std::wstring Header = L"", std::wstring Cookies = L"", std::wstring ReturnCookies = L"");
		//GET����
		//@parameter �����ַ
		//@parameter ��������
		//@parameter ����Э��ͷ
		//@parameter ����Cookies
		//@parameter ����Cokies
		bool _fastcall GET(std::string url, std::string &ResultData, std::string Header = "", std::string Cookies = "", std::string ReturnCookies = "");
		//GET����
		//@parameter �����ַ
		//@parameter ��������(����string����,ת������������)
		//@parameter ����Э��ͷ
		//@parameter ����Cookies
		//@parameter ����Cokies
		bool _fastcall GET(std::wstring url, std::string &ResultData, std::wstring Header = L"", std::wstring Cookies = L"", std::wstring ReturnCookies = L"");
		//��Э��ͷ�л�ȡCookies
		std::string __fastcall GetCookies(const char* httpHeader);
		//GET��ֹ�ض���
		bool _fastcall GET(bool forbidjump, const char* url, std::string& ResultData, std::string Header, std::string Cookies, std::string& ReturnCookies);
		//�����ļ�
		//@parameter ���ص�ַ
		//@parameter �����ļ�Ŀ¼
		//@parameter �ص����� DownloadProc(size_t FileSize, UINT nState, size_t DownloadSize, size_t SecondSize);
		bool _fastcall DownloadFile(std::string url, std::string FilePath, DownloadProc DownloadPr = nullptr);
		//�����ļ�
		//@parameter ���ص�ַ
		//@parameter �����ļ�Ŀ¼
		//@parameter �ص����� DownloadProc(size_t FileSize, UINT nState, size_t DownloadSize, size_t SecondSize);
		bool _fastcall DownloadFile(std::wstring url, std::wstring FilePath, DownloadProc DownloadPr = nullptr);
		//��ȡָ����ַ����IE Cookies
		//@parameter ����Url��ַ
		std::string _fastcall GetLocalCookies(std::string host);
		//��ȡָ����ַ����IE Cookies
		//@parameter ����Url��ַ
		std::wstring _fastcall GetLocalCookies(std::wstring host);
		//��ȡ��ҳҳ���ַ
		//@parameter url
		std::string _fastcall GetURLPage(std::string url);
		//��ȡ��ҳҳ���ַ
		//@parameter url
		std::wstring _fastcall GetURLPage(std::wstring url);
		//��ȡָ��Url��Host��ַ
		//@parameter Url
		std::string _fastcall GetHost(std::string url);
		//��ȡָ��Url��Host��ַ
		//@parameter Url
		std::wstring _fastcall GetHost(std::wstring url);
		//����JScript
		//@parameter JScript����
		//@parameter JScript������������func(a,b)
		VARIANT _fastcall ExecuteJScript(std::string JScript, std::string FuncetionName, std::string ErrorLog = "");
		//����JScript
		//@parameter JScript����
		//@parameter JScript������������func(a,b)
		VARIANT _fastcall ExecuteJScript(std::wstring JScript, std::wstring FuncetionName, std::wstring ErrorLog = L"");
		//������ʽ
		//@parameter ������ʽ
		//@parameter ����Ŀ���ַ���
		//@parameter �Ƿ����ִ�Сд
		//@parameter �Ƿ�ƥ�����
		//@parameter �Ƿ�ȫ��ƥ��
		bool _fastcall RegEx_FindString(std::string Regex, std::string DestString, bool IsCase = false, bool IsMultiRows = true, bool IsGlobal = true);
		//������ʽ
		//@parameter ������ʽ
		//@parameter ����Ŀ���ַ���
		//@parameter �Ƿ����ִ�Сд
		//@parameter �Ƿ�ƥ�����
		//@parameter �Ƿ�ȫ��ƥ��
		bool _fastcall RegEx_FindString(std::wstring Regex, std::wstring DestString, bool IsCase = false, bool IsMultiRows = true, bool IsGlobal = true);
	private:

	};
	//Http Socket��ʽ����;
	class CLibhttp2
	{
	public:
		CLibhttp2() {};
		~CLibhttp2() {};
		bool __fastcall Initalize();
		void __fastcall UnInitalize();
		std::string __fastcall GetHost(std::string url);
		std::wstring __fastcall GetHost(std::wstring url);
		std::string __fastcall GetURLPage(std::string url);
		std::wstring __fastcall GetURLPage(std::wstring url);
		UINT __fastcall GetProt(TCHAR* url);
		lib_String::CLibString __fastcall GetCookies();
		lib_String::CLibString __fastcall GethtmlData(std::string& ResultData);
		bool __fastcall Get(const TCHAR* Url, std::string& ResultData, const TCHAR* Header = nullptr, const TCHAR* Cookies = nullptr);
	private:
		lib_String::CLibString m_htmlHeader;
		lib_String::CLibString m_htmldata;

	};
}

#endif
