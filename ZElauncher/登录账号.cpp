#include "��¼�˺�.h"

bool IsLoginCreate = false;
bool is_btn_IElogin = false, is_btn_Login = false, is_btn_getsecurecode = false, is_btn_Logout = false, Is_IELogin = false;

LPCTSTR C��¼�˻�UI::GetWindowClassName()const
{
	return _T("LoginingWnd");
}

CDuiString C��¼�˻�UI::GetSkinFile()
{
	return _T("Login.xml");
}

CDuiString C��¼�˻�UI::GetSkinFolder()
{
	return _T("Skin\\ListRes\\");
}

UILIB_RESTYPE C��¼�˻�UI::GetResourceType()const
{
	return UILIB_FILE;
}

LPCTSTR C��¼�˻�UI::GetResourceID()const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void C��¼�˻�UI::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

CControlUI* C��¼�˻�UI::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void C��¼�˻�UI::���õ�¼UI״̬()
{
	if (m_IsLogin) {
		CContainerUI* pContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("panel_show")));
		if (!pContainer)MessageBox(NULL, _T("���ݳ���!"), NULL, NULL);
		pContainer->SetVisible(false);
		COptionUI* pOption = static_cast<COptionUI*>(m_pm.FindControl(_T("btn_Logout")));
		if (!pOption)MessageBox(NULL, _T("���ݳ���!"), NULL, NULL);
		pOption->SetVisible(true);
	}
	else {
		CContainerUI* pContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("panel_show")));
		if (!pContainer)MessageBox(NULL, _T("���ݳ���!"), NULL, NULL);
		pContainer->SetVisible(true);
		COptionUI* pOption = static_cast<COptionUI*>(m_pm.FindControl(_T("btn_Logout")));
		if (!pOption)MessageBox(NULL, _T("���ݳ���!"), NULL, NULL);
		pOption->SetVisible(false);
	}
}

void C��¼�˻�UI::InitWindow()
{
	if (IsLoginCreate)__super::Close();
	IsLoginCreate = true;
	���õ�¼UI״̬();
	��ʼ���û����༭��();
}

void C��¼�˻�UI::SaveEditInfo()
{
	_bstr_t CfgPath = GetCFGPath();
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("Edit_UserName")));
	if (!pEdit)return;
	CDuiString tmpstr = pEdit->GetText();
	WritePrivateProfileString(_T("ZElauncher"), _T("Login_UserName"), tmpstr.GetData(), CfgPath);
	if (Is_IELogin)WritePrivateProfileString(_T("ZElauncher"), _T("Login_IELogin"), _T("true"), CfgPath);
	else WritePrivateProfileString(_T("ZElauncher"), _T("Login_IELogin"), _T("false"), CfgPath);
}

void C��¼�˻�UI::OnExit(const TNotifyUI& msg)
{
	SaveEditInfo();
	IsLoginCreate = false;
	__super::Close();
}

C��¼�˻�UI::C��¼�˻�UI(bool Islogin)
{ 
	m_IsLogin = Islogin;
}

bool C��¼�˻�UI::IELoginVerify()
{
	//��ȡ������Ϣ;
	lib_http::CLibhttp http;
	std::string Cookies;
	std::string Result;
	http.GET("https://bbs.93x.net/plugin.php?id=xnet_steam_openid:SoftLogin", Result, "", Cookies);
	//JSON����
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(Result, root)) {
		if (!root.isNull()) {
			Json::Value userdata = root["userdata"];
			if (userdata.isNull())return false;
			if (userdata["username"].isNull())return false;
			return true;
		}
	}
	return false;
}

void C��¼�˻�UI::IELoginUser()
{
	Is_IELogin = true;
	//lib_http::CLibhttp http;
	//gCookies = http.GetLocalCookies("https://bbs.93x.net").c_str();
	gCookies = "";
	MessageBox(NULL, _T("��¼�˻���,��ر�IE.(�ǵù�ѡ�Զ���¼!)"), NULL, MB_OK | MB_TOPMOST);
	STARTUPINFO si;
	GetStartupInfo(&si);
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	if (CreateProcess(_T("C:\\Program Files\\Internet Explorer\\iexplore.exe"), (LPWSTR)_T("open https://bbs.93x.net"), NULL, NULL, NULL, NULL, NULL, NULL, &si, &pi) == 0) {
		MessageBox(NULL, _T("IE���̴���ʧ��,��ȷ���Ƿ�װIE.\r\n���ֶ���IE���� https://bbs.93x.net ��¼�˻�."), NULL, MB_OK | MB_TOPMOST);
		return;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	if (IELoginVerify())
	{
		MessageBox(NULL, _T("IE�ɹ���¼�˻�!"), _T("Tip:"), MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
		OnExit(TNotifyUI());
	}
	else MessageBox(NULL, _T("IE��¼ʧ��!"), NULL, MB_OK | MB_TOPMOST);
	g_pZElauncher->InitLogin();
}

void C��¼�˻�UI::����POST��������(std::string& POST��������)
{
	POST�������� = "username=";
	strCoding �ַ�������;
	_bstr_t �û��� = "";
	CEditUI* pEditUser = static_cast<CEditUI*>(m_pm.FindControl(_T("Edit_UserName")));
	CEditUI* pEditPWD = static_cast<CEditUI*>(m_pm.FindControl(_T("Edit_PassWord")));
	if (!pEditUser || !pEditPWD) {
		MessageBox(NULL, _T("��¼ʧ��,UI��Ϣ��ȡʧ��!"), NULL, NULL);
		return;
	}
	�û��� = pEditUser->GetText();
	std::string Tmpstr = �ַ�������.UrlUTF8(�û���);
	POST�������� += Tmpstr;
	POST�������� += "&timestamp=";
	char tmpbuff[1024] = { 0 };
	sprintf(tmpbuff, "%lld", time(NULL));
	POST�������� += tmpbuff;
	POST�������� += "&securecode=";
	_bstr_t �û����� = "";
	�û����� = pEditPWD->GetText();
	std::string securecode;
	securecode.append(�û�����);
	POST�������� += Encode_sha512(securecode).c_str();
}

void C��¼�˻�UI::��ʼ���û����༭��()
{
	TCHAR tmpbuff[1024] = { 0 };
	_bstr_t CfgPath = GetCFGPath();
	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("Edit_UserName")));
	if (!pEdit)return;
	GetPrivateProfileString(_T("ZElauncher"), _T("Login_UserName"), NULL, tmpbuff, sizeof(tmpbuff), CfgPath);
	if (_tcslen(tmpbuff) > 3) {
		pEdit->SetText(tmpbuff);
	}
	GetPrivateProfileString(_T("ZElauncher"), _T("Login_IELogin"), NULL, tmpbuff, sizeof(tmpbuff), CfgPath);
	if (_tcsstr(tmpbuff, _T("true")))Is_IELogin = true;
}

void C��¼�˻�UI::������봦��(std::string �����ı�)
{
	if (�����ı�.find("username not found!") != std::string::npos) {
		int npos = �����ı�.find("username not found!") + strlen("username not found!");
		int npos2 = �����ı�.find("\"}");
		std::string errinfo = �����ı�.substr(npos, npos2 - npos);
		strCoding encode;
		errinfo = encode.usc2ToAnsi(errinfo);
		std::string errtip = "��¼ʧ��!\r\nδ�ҵ��û���";
		errtip += errinfo;
		MessageBoxA(NULL, errtip.c_str(), "����:", MB_OK | MB_TOPMOST);
	}
	else if (�����ı�.find("securecode not match") != std::string::npos) {
		MessageBoxA(NULL, "��¼ʧ��!\r\n��¼����ȫ�����!", "����:", MB_OK | MB_TOPMOST);
	}
	else {
		int npos = �����ı�.find("errmsg\":\"") + strlen("errmsg\":\"");
		int npos2 = �����ı�.find("\"}");
		std::string errinfo = �����ı�.substr(npos, npos2 - npos);
		strCoding encode;
		errinfo = encode.usc2ToAnsi(errinfo);
		std::string errtip = "��¼ʧ��!\r\n";
		errtip += errinfo;
		MessageBoxA(NULL, errtip.c_str(), "����:", MB_OK | MB_TOPMOST);
	}
}

bool C��¼�˻�UI::UserEvent(std::string ResultData, std::string &ret_token)
{
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(ResultData, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
	{
		if (root.isNull()) {
			MessageBox(NULL, _T("Json���ݻ�ȡʧ��."), NULL, NULL);
			return false;
		}
		if (!root["userdata"].isNull()) {
			Json::Value Json_�û����� = root["userdata"];
			if (!Json_�û�����["user_token"].isNull()) {
				ret_token = Json_�û�����["user_token"].asCString();
			}
			else {
				MessageBox(NULL, _T("�û����ݻ�ȡʧ��."), NULL, NULL);
				return false;
			}
		}
		else if (!root["errcode"].isNull()) {
			std::string errcode = root["errcode"].asString();
			if (strcmp(errcode.c_str(), "500") == 0) {
				������봦��(ResultData);
				return false;
			}
		}
	}
	return true;
}

bool C��¼�˻�UI::LoginVerify(std::string token)
{
	std::string RequestUrl = "https://bbs.93x.net/plugin.php?id=xnet_core_api:ForumLogin&token=", ReturnCookes = "";
	std::string �����ı� = "";
	RequestUrl += token;
	lib_http::CLibhttp http;
	http.GET(true, RequestUrl.c_str(), �����ı�, "", "NotCookie", ReturnCookes);
	http.GET("https://bbs.93x.net/", �����ı�, "", ReturnCookes);
	UINT nLen = �����ı�.size() * sizeof(TCHAR);
	wchar_t* pStrHtml = new wchar_t[nLen];
	MultiByteToWideChar(CP_UTF8, NULL, �����ı�.c_str(), �����ı�.length(), pStrHtml, nLen);
	if (wcsstr(pStrHtml, L"�һ�����") && wcsstr(pStrHtml, L"����ע��"))return false;
	_bstr_t cfgbuff = GetRunPath();
	cfgbuff += _T("\\bin\\Cookies.data");
	_bstr_t FileName = cfgbuff;
	FILE* pFile = fopen(FileName, "wb+");
	if (!pFile) {
		delete[]pStrHtml;
		MessageBox(NULL, _T("��ȡCookiesʧ��,��ȷ���Ƿ�ӵ���ļ�����Ȩ��!"), NULL, MB_OK);
		return false;
	}
	fseek(pFile, SEEK_SET, 0);
	fwrite((void*)ReturnCookes.c_str(), ReturnCookes.size(), 1, pFile);
	fclose(pFile);
	gCookies = ReturnCookes.c_str();
	g_pZElauncher->InitLogin();
	delete[]pStrHtml;
	return true;
}

void C��¼�˻�UI::LoginUser()
{
	Is_IELogin = false;
	std::string �����ı�, POST��������, Tmpstr, token;
	����POST��������(POST��������);
	lib_http::CLibhttp http;
	http.POST("https://bbs.93x.net/plugin.php?id=xnet_core_api:GetForumUserDetail", �����ı�, POST��������);
	if (!UserEvent(�����ı�, token))return;
	if (LoginVerify(token))
	{
		MessageBox(NULL, _T("�ɹ���¼�˻�!"), _T("Tip:"), MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
		OnExit(TNotifyUI());
	}
	else MessageBox(NULL, _T("��¼ʧ��!"), NULL, MB_OK | MB_ICONERROR | MB_TOPMOST);

}

void C��¼�˻�UI::Logout()
{
	_bstr_t cfgbuff = GetRunPath();
	cfgbuff += _T("\\bin\\Cookies.data");
	_bstr_t FileName = cfgbuff;
	FILE* pFile = fopen(FileName, "wb+");
	if (!pFile)return;
	fclose(pFile);
	g_pZElauncher->LogoutUser("δ��¼");
	m_IsLogin = false;
	���õ�¼UI״̬();
	g_pZElauncher->IsCreateShop = false;
	gCookies = "";
	g_SteamID64 = "";
}

void C��¼�˻�UI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)::SetForegroundWindow(m_hWnd);//���ڴ�����Ϻ�,��ȡ����;
	else if (_tcscmp(msg.pSender->GetName(), _T("closebtn")) == 0)OnExit(msg);
	else if (_tcscmp(msg.pSender->GetName(), _T("btn_IElogin")) == 0) {
		SaveEditInfo();
		if (!is_btn_IElogin) {
			is_btn_IElogin = true;
			IELoginUser();
		}
		else {
			is_btn_IElogin = false;
		}
	}
	else if (_tcscmp(msg.pSender->GetName(), _T("btn_Login")) == 0) {
		SaveEditInfo();
		if (!is_btn_Login) {
			is_btn_Login = true;
			LoginUser();
		}
		else {
			is_btn_Login = false;
		}
	}
	else if (_tcscmp(msg.pSender->GetName(), _T("btn_getsecurecode")) == 0) {
		if (!is_btn_getsecurecode) {
			is_btn_getsecurecode = true;
			ShellExecute(NULL, _T("open"), _T("https://bbs.93x.net/plugin.php?id=xnet_steam_openid:SoftLogin_getsecurecode"), NULL, NULL, NULL);
		}
		else {
			is_btn_getsecurecode = false;
		}
	}
	else if (_tcscmp(msg.pSender->GetName(), _T("btn_Logout")) == 0) {
		if (!is_btn_Logout) {
			is_btn_Logout = true;
			Logout();
		}
		else {
			is_btn_Logout = false;
		}
	}
}