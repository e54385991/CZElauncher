#pragma once
#include <windows.h>
#include <iostream>
#include <thread>
#include <atlimage.h>

#include "resource.h"
#include "unzip.h"
#include "Duilib/StdAfx.h"
#include "../ZElauncher/json/json.h"
#include "../ZElauncher/ModuleSDK/lib_http.h"
#include "../ZElauncher/ModuleSDK/lib_Process.h"


#import <progid:VBScript.RegExp>named_guids
#ifdef _DEBUG
#include "Debug/3.tlh"
#endif // _DEBUG


using namespace DuiLib;
class CUpdateUI :
	public WindowImplBase
{
public:
	CUpdateUI() {}
	~CUpdateUI() { ::PostQuitMessage(0L); }
	LPCTSTR  GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	LPCTSTR GetResourceID() const;
	UILIB_RESTYPE GetResourceType() const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void SetProgressText(LPCTSTR pString);
	void SetProgressValue(UINT Value);
private:
	void Notify(TNotifyUI& msg);					//��Ϣ�����¼�
	void OnExit(const TNotifyUI& msg);				//�����뿪�¼�
	void OnClick(const TNotifyUI& msg);				//��ť�����¼�
	void OnCreate();
	CDuiString GetUpdateVersionUrl();
	void DownloadThread(CDuiString& Url);
private:
	CPaintManagerUI m_pListManager;

};


