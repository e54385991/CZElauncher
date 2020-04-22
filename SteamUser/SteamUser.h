#pragma once
#include <UIlib.h>
#include <atlimage.h>

#include "resource.h"
#include "SDK/Path.h"
#include "../ZElauncher/json/json.h"
#include "../ZElauncher/ModuleSDK/lib_http.h"
#include "../ZElauncher/ModuleSDK/lib_Process.h"


#import <progid:VBScript.RegExp>named_guids
#ifdef _DEBUG
#include "Debug/3.tlh"
#endif // _DEBUG


using namespace DuiLib;
class CSteamUserMain :
	public WindowImplBase
{
public:
	CSteamUserMain() {}
	~CSteamUserMain() { ::PostQuitMessage(0L); }
	LPCTSTR  GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);
	void InitWindow();
	CDuiString GetSkinFile();
	CDuiString GetSkinFolder();
	LPCTSTR GetResourceID() const;
	UILIB_RESOURCETYPE GetResourceType() const;
	CControlUI* CreateControl(LPCTSTR pstrClass);
	BOOL TerminateProcess(DWORD pid);
private:
	void Notify(TNotifyUI& msg);					//��Ϣ�����¼�
	void OnExit(const TNotifyUI& msg);				//�����뿪�¼�
	void OnClick(const TNotifyUI& msg);				//��ť�����¼�
	void GetUserInfo();								//��ȡ�û���Ϣ
	void LoginInSteamUser(CControlUI* pControl);	//�л�Steam�˺�
	void WriteRegiter(LPCTSTR pUserName);			//д���¼�˻���
	_bstr_t GetUserData();							//��ȡ�û�����
	_bstr_t GetPattern();							//��ȡ������ʽ
	CDuiString GetImageStr(LPCTSTR pSteamID);		//��ȡ����ͷ������
private:
	CPaintManagerUI m_pListManager;

};


