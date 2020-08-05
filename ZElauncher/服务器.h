#pragma once
#include "stdafx.h"
#include "��ѯ���������.h"

using namespace DuiLib;
class C������UI;
extern bool IsChineseMap;
extern C������UI* pServer;
namespace Server {
	struct ServerInfo
	{
		std::string name;
		UINT type;
		bool Is93x;
	};
}

class C������UI :
	public CContainerUI
	,public INotifyUI
{
public:
	C������UI(CPaintManagerUI& m_pm, HWND hWnd);
	~C������UI();
	void RefreshServer();
	void ShowServerList(const char* pName, UINT type = NULL, bool Is93x = false);
	void CopyServerInfo(UINT nIndex);
	void JoinServer();
	void DownloadMap93x(LPCTSTR lpMapName, bool IsRunGame);
	void static DownloadMapjsly(LPCTSTR lpMapName);
	void static DownloadMapfys(LPCTSTR lpMapName);
	void RunGame(LPCTSTR lpServer);
	void OnLookPlayer();
	bool IsFileExist(LPCTSTR lpFileName);
private:
	void Notify(TNotifyUI& msg);
	//�°��ȡ�������б�
	void GetServerList2(const char* pName, UINT type = NULL);
	void Get93xServerList2(const char* pName, UINT type = NULL);
	void MenuClick(CControlUI* Click);
	void OnCreate();
	void OnClick(CControlUI* Click);
	//�༭�����
	void ClearEdit();
	//LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//LPCTSTR GetWindowClassName()const;
private:
	CPaintManagerUI* m_paManager;
	CPaintManagerUI m_pTmpManager;
	CDialogBuilder builder;
	HWND m_hWnd;
	static bool Is93x;
};