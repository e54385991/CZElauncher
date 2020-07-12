#include "����.h"

bool IsSettingCreate = false;


LPCTSTR C����UI::GetWindowClassName()const
{
	return _T("SettingWnd");
}

void C����UI::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

void C����UI::InitEditInfo()
{
	//��ȡCSGOĿ¼
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_csgo")));
	if (!pEdit)return;
	TCHAR csgobuff[1024] = { 0 };
	TCHAR steambuff[1024] = { 0 };
	TCHAR cfgbuff[1024] = { 0 };
	GetRunPath(cfgbuff, sizeof(cfgbuff));
	_tcscat(cfgbuff, _T("\\bin\\Config.cfg"));
	GetPrivateProfileString(_T("ZElauncher"), _T("CSGOPath"), NULL, csgobuff, sizeof(csgobuff), cfgbuff);
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
	GetPrivateProfileString(_T("ZElauncher"), _T("SteamPath"), NULL, steambuff, sizeof(steambuff), cfgbuff);
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
	GetPrivateProfileString(_T("ZElauncher"), _T("edit_bkimge"), NULL, steambuff, sizeof(steambuff), cfgbuff);
	if (_tcslen(steambuff) > 3) {
		pEdit->SetText(steambuff);
	}
	//��ȡ�������б���ͼƬĿ¼
	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_listbkimge")));
	if (!pEdit)return;
	GetPrivateProfileString(_T("ZElauncher"), _T("edit_listbkimge"), NULL, steambuff, sizeof(steambuff), cfgbuff);
	if (_tcslen(steambuff) > 3) {
		pEdit->SetText(steambuff);
	}
	//��ȡԤ������״̬
	DuiLib::CCheckBoxUI* pOption = static_cast<DuiLib::CCheckBoxUI*>(m_pm.FindControl(_T("Option_HotMap")));
	if (!pOption)return;
	GetPrivateProfileString(_T("ZElauncher"), _T("DownloadHotmap"), NULL, steambuff, sizeof(steambuff), cfgbuff);
	if (_tcscmp(steambuff, _T("true")) == 0) {
		pOption->SetCheck(true, false);
	}
}

LRESULT C����UI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT C����UI::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("Setting.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	if (IsSettingCreate)__super::Close();
	IsSettingCreate = true;
	InitEditInfo();
	return 0;
}

void C����UI::SaveEditInfo()
{
	TCHAR cfgbuff[1024] = { 0 };
	GetRunPath(cfgbuff, sizeof(cfgbuff));
	_tcscat(cfgbuff, _T("\\bin\\Config.cfg"));
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_csgo")));
	if (!pEdit)return;
	CDuiString tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("CSGOPath"), tmpstr.GetData(), cfgbuff);
	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_steam")));
	if (!pEdit)return;
	tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("SteamPath"), tmpstr.GetData(), cfgbuff);

	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_listbkimge")));
	if (!pEdit)return;
	tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("edit_listbkimge"), tmpstr.GetData(), cfgbuff);

	pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_bkimge")));
	if (!pEdit)return;
	tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("edit_bkimge"), tmpstr.GetData(), cfgbuff);

	if (g_IsDownloadHotmap)tmpstr = _T("true");
	else tmpstr = _T("false");
	WritePrivateProfileString(_T("ZElauncher"), _T("DownloadHotmap"), tmpstr.GetData(), cfgbuff);
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
	bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN;
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
	g_IsDownloadHotmap = pOption->IsSelected();
	//g_IsDownloadHotmap = !g_IsDownloadHotmap;
}

void C����UI::OnClick(CControlUI* pSender)
{
	if (_tcscmp(pSender->GetName(), _T("closebtn")) == 0)OnExit();
	else if (_tcscmp(pSender->GetName(), _T("btn_getcsgopath")) == 0)GetNewPath(_T("edit_csgo"));
	else if (_tcscmp(pSender->GetName(), _T("btn_getsteampath")) == 0)GetNewPath(_T("edit_steam"));
	else if (_tcscmp(pSender->GetName(), _T("btn_bkimge_path")) == 0)GetImagePath(_T("edit_bkimge"));
	else if (_tcscmp(pSender->GetName(), _T("btn__listbkimge_path")) == 0)GetImagePath(_T("edit_listbkimge"));
	else if (_tcscmp(pSender->GetName(), _T("Option_HotMap")) == 0)SetDownloadHotMap();
}

void C����UI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)::SetForegroundWindow(m_hWnd);//���ڴ�����Ϻ�,��ȡ����;
	if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);
	OutputDebugString(msg.sType);
	OutputDebugString(L"\n");
}