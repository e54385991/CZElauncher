#pragma once
#include "stdafx.h"
#include "ModuleSDK/Path.h"
#include <Shlobj.h>

using namespace DuiLib;

extern CDuiString g_BBSUrl;
extern CDuiString g_BBSTitle;

class C�鿴����UI : public WindowImplBase
{
public:
	C�鿴����UI(CPaintManagerUI* pManager) { m_paintmanager = pManager; }
	~C�鿴����UI() {} //::PostQuitMessage(0L); }
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	//UILIB_RESTYPE GetResourceType()const;
	UILIB_RESTYPE GetResourceType()const;
	LPCTSTR GetResourceID()const;
	void OnFinalMessage(HWND hWnd);
	LPCTSTR GetWindowClassName()const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void InitWindow();
	void ��ȡ��������();
private:
	void OnExit();
	void Notify(TNotifyUI& msg);
	void OnClick(CControlUI* pSender);
	void ����������Ϣ(std::string& htmldata, UINT* nFloor);
	CDuiString GetImageStr(std::string& imgurl);
	void ��ʼ���ؼ�����();
	void Getfish();
private:
	CProgressUI* m_pProgress = nullptr;
	CListUI* m_BBSList = nullptr;

private:
	CPaintManagerUI* m_paintmanager;
	CPaintManagerUI m_pTilelayout;
	std::vector<CDuiString> m_fishurl;
};

