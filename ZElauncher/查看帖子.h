#pragma once
#include "stdafx.h"
#include "ModuleSDK/Path.h"
#include <Shlobj.h>

using namespace DuiLib;

extern CDuiString g_BBSUrl;

class C�鿴����UI : public WindowImplBase
{
public:
	C�鿴����UI(){}
	~C�鿴����UI() {} //::PostQuitMessage(0L); }
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	//UILIB_RESOURCETYPE GetResourceType()const;
	UILIB_RESTYPE GetResourceType()const;
	LPCTSTR GetResourceID()const;
	void OnFinalMessage(HWND hWnd);
	LPCTSTR GetWindowClassName()const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void InitWindow();
private:
	void OnExit(const TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
	void ��ȡ��������();
private:
	CPaintManagerUI m_pTilelayout;
};