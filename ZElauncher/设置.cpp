#include "����.h"

bool IsSettingCreate = false;
//bool is_btn_getcsgopath = false, is_btn_getsteampath = false, is_btn_bkimge_path = false, is_btn__listbkimge_path = false;

LPCTSTR C����UI::GetWindowClassName()const
{
	return _T("SettingWnd");
}

CDuiString C����UI::GetSkinFile()
{
	return _T("Setting.xml");
}

CDuiString C����UI::GetSkinFolder()
{
	return _T("Skin\\ListRes\\");
}

UILIB_RESTYPE C����UI::GetResourceType()const
{
	return UILIB_FILE;
}

LPCTSTR C����UI::GetResourceID()const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void C����UI::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

CControlUI* C����UI::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void C����UI::InitEditInfo()
{
	//��ȡCSGOĿ¼
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_csgo")));
	if (!pEdit)return;
	TCHAR csgobuff[1024] = { 0 };
	TCHAR steambuff[1024] = { 0 };
	_bstr_t CfgPath = GetCFGPath();
	GetPrivateProfileString(_T("ZElauncher"), _T("CSGOPath"), NULL, csgobuff, sizeof(csgobuff), CfgPath);
	if (_tcslen(csgobuff) > 3) {
		pEdit->SetText(csgobuff);
	}
	else
	{
		GetCSGOPath(csgobuff, sizeof(csgobuff));
		pEdit->SetText(csgobuff);
	}
	//��ȡSteamĿ¼
	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_steam")));
	if (!pEdit)return;
	GetPrivateProfileString(_T("ZElauncher"), _T("SteamPath"), NULL, steambuff, sizeof(steambuff), CfgPath);
	if (_tcslen(steambuff) > 3) {
		pEdit->SetText(steambuff);
	}
	else
	{
		GetSteamPath(steambuff, sizeof(steambuff));
		pEdit->SetText(steambuff);
	}
	//��ȡ����ͼƬĿ¼
	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_bkimge")));
	if (!pEdit)return;
	GetPrivateProfileString(_T("ZElauncher"), _T("edit_bkimge"), NULL, steambuff, sizeof(steambuff), CfgPath);
	if (_tcslen(steambuff) > 3) {
		pEdit->SetText(steambuff);
	}
	//��ȡ�������б���ͼƬĿ¼
	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_listbkimge")));
	if (!pEdit)return;
	GetPrivateProfileString(_T("ZElauncher"), _T("edit_listbkimge"), NULL, steambuff, sizeof(steambuff), CfgPath);
	if (_tcslen(steambuff) > 3) {
		pEdit->SetText(steambuff);
	}
	//��ȡԤ������״̬
	DuiLib::CCheckBoxUI* pOption = static_cast<DuiLib::CCheckBoxUI*>(m_pm.FindControl(_T("Option_HotMap")));
	if (pOption)pOption->SetCheck(g_IsDownloadHotmap);
}

void C����UI::InitWindow()
{
	if (IsSettingCreate)__super::Close();
	IsSettingCreate = true;
	InitEditInfo();
}

void C����UI::SaveEditInfo()
{
	_bstr_t CfgPath = GetCFGPath();
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_csgo")));
	if (!pEdit)return;
	CDuiString tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("CSGOPath"), tmpstr.GetData(), CfgPath);
	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_steam")));
	if (!pEdit)return;
	tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("SteamPath"), tmpstr.GetData(), CfgPath);

	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_listbkimge")));
	if (!pEdit)return;
	tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("edit_listbkimge"), tmpstr.GetData(), CfgPath);

	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_bkimge")));
	if (!pEdit)return;
	tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("edit_bkimge"), tmpstr.GetData(), CfgPath);

	if (g_IsDownloadHotmap)tmpstr = _T("true");
	else tmpstr = _T("false");
	WritePrivateProfileString(_T("ZElauncher"), _T("DownloadHotmap"), tmpstr.GetData(), CfgPath);
}

void C����UI::OnExit()
{
	IsSettingCreate = false;
	SaveEditInfo();
	if (!Istip) { Istip = true; MessageBox(NULL, _T("����������Ҫ����������Ч!"), NULL, MB_TOPMOST); }
	__super::Close();
}

void C����UI::GetNewPath(LPCTSTR pEditName)
{
	TCHAR szBuffer[1024] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("��ѡ����ϷĿ¼:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (!idl)
	{
		return;
	}
	SHGetPathFromIDList(idl, szBuffer);
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(pEditName));
	if (!pEdit)return;
	pEdit->SetText(szBuffer);
	/*::SendMessage(m_hWnd, WM_LBUTTONDOWN, NULL, NULL);
	::SendMessage(m_hWnd, WM_LBUTTONUP, NULL, NULL);*/
}

void C����UI::GetImagePath(LPCTSTR pEditName)
{
	TCHAR szBuffer[1024] = { 0 };
	OPENFILENAME ofn = { sizeof(OPENFILENAME),0 };
	ofn.hwndOwner = this->m_hWnd;
	ofn.lpstrFileTitle = const_cast<LPWSTR>(_T("��ͼƬ�ļ�"));
	ofn.lpstrFile = const_cast<LPWSTR>(_T("*.*\0*.*\0\0"));
	ofn.lpstrFile = szBuffer;//����ļ��Ļ�����
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//��־����Ƕ�ѡҪ����OFN_ALLOWMULTISELECT
	BOOL bSel = GetOpenFileName(&ofn);
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(pEditName));
	if (!pEdit)return;
	if (_tcslen(szBuffer) > 3)pEdit->SetText(szBuffer);
}

void C����UI::SetDownloadHotMap()
{
	DuiLib::CCheckBoxUI* pOption = static_cast<DuiLib::CCheckBoxUI*>(m_pm.FindControl(_T("Option_HotMap")));
	if (!pOption)return;
	g_IsDownloadHotmap = !pOption->IsSelected();
	SaveEditInfo();
}

void C����UI::OnClick(CControlUI* pSender)
{
	if (_tcscmp(pSender->GetName(), _T("closebtn")) == 0)OnExit();
	else if (_tcscmp(pSender->GetName(), _T("minbtn")) == 0)SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, NULL);
	else if (_tcscmp(pSender->GetName(), _T("btn_getcsgopath")) == 0)GetNewPath(_T("edit_csgo"));
	else if (_tcscmp(pSender->GetName(), _T("btn_getsteampath")) == 0)GetNewPath(_T("edit_steam"));
	else if (_tcscmp(pSender->GetName(), _T("btn_bkimge_path")) == 0)GetImagePath(_T("edit_bkimge"));
	else if (_tcscmp(pSender->GetName(), _T("btn__listbkimge_path")) == 0)GetImagePath(_T("edit_listbkimge"));
	else if (_tcscmp(pSender->GetName(), _T("Option_HotMap")) == 0)SetDownloadHotMap();
}

void C����UI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)::SetForegroundWindow(m_hWnd);//���ڴ�����Ϻ�,��ȡ����;
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);
}