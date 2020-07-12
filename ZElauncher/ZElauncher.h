#pragma once
#include "stdafx.h"
#include "������.h"
#include "���.h"
#include "����.h"
#include "�̳�.h"
#include "������.h"
#include "����Ƥ��.h"
#include "��¼�˺�.h"

class C�̳�UI;
class C����Ƥ��UI;
using namespace DuiLib;
class CZElauncherMain : 
	public WindowImplBase
{
public:
	CZElauncherMain() {}
	~CZElauncherMain() { ::PostQuitMessage(0L); }
	LPCTSTR  GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);
	void InitWindow();
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	LPCTSTR GetResourceID() const;
	UILIB_RESOURCETYPE GetResourceType() const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void SetSkinName(LPCTSTR SkinName);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void ��ȡZE��ͼ��������(LPCTSTR maptype);
	void ������ȡZE��ͼ��������();
	_bstr_t GetChineseMapName(_bstr_t Map_englishName);
public:
	void OnSetting(bool IsModel = false);
	void OnLogin(bool IsLogin, bool IsModel = false);
	void InitLogin();
	void LogoutUser(_bstr_t Tiptext);
	bool UserIslogin();
	void OutTip(_bstr_t pOutTip);
	void OutTip2(_bstr_t pOutTip2);
	bool IsCreateShop = false;
	bool IsCreateSkin = false;
private:
	void Notify(TNotifyUI& msg);
	void OnExit(const TNotifyUI& msg);
	void OnClick(const TNotifyUI& msg);
	void OnMenu(const TNotifyUI& msg);
	void OnInitialize();
	void Get93xServerListInfo();
private:
	const TCHAR* m_pMenuBtn = _T("menubtn");
	const TCHAR* m_pMinBtn = _T("minbtn");
	const TCHAR* m_pCloseBtn = _T("closebtn");
	const TCHAR* m_pBugBtn = _T("BUGbtn");
	const TCHAR* m_pBBSBtn = _T("BBSbtn");
	C�̳�UI* pShop = nullptr;
	C����Ƥ��UI* pSkin = nullptr;

};