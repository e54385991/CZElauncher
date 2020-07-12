#pragma once
#include "ZElauncher.h"
#include "stdafx.h"
#include <Shlobj.h>

using namespace DuiLib;
class CZElauncherMain;
class C��¼�˻�UI : public WindowImplBase
{
public:
	C��¼�˻�UI(){}
	C��¼�˻�UI(bool Islogin);
	~C��¼�˻�UI() {} //::PostQuitMessage(0L); }
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	UILIB_RESOURCETYPE GetResourceType()const;
	LPCTSTR GetResourceID()const;
	void OnFinalMessage(HWND hWnd);
	LPCTSTR GetWindowClassName()const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void InitWindow();
private:
	void IELoginUser();
	bool IELoginVerify();
	void LoginUser();
	bool LoginVerify(std::string token);
	void ����POST��������(std::string& POST��������);
	void ��ʼ���û����༭��();
	void ������봦��(std::string �����ı�);
	void ���õ�¼UI״̬();
	void Logout();
	bool UserEvent(std::string ResultData, std::string& ret_token);
	void OnExit(const TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
	void SaveEditInfo();

private:
	bool m_Istip = false;
	bool m_IsLogin = false;
};