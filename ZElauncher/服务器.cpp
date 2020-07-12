#include "������.h"

bool C������UI::Is93x = true;

C������UI* pServer = nullptr;
Server::ServerInfo g_Current_server_name;
CDuiString g_mapName;
CPaintManagerUI* g_paManager = nullptr;
bool IsChineseMap = true;

C������UI::~C������UI()
{
}

C������UI::C������UI(CPaintManagerUI& m_paintmanager, HWND hWnd)
{
	m_paManager = &m_paintmanager;
	m_hWnd = hWnd;
	pServer = this;
	CContainerUI* pServer = static_cast<CContainerUI*>(builder.Create(_T("server.xml"), NULL, NULL, &m_pTmpManager));
	if (pServer) {
		this->Add(pServer);
		m_paManager->AddNotifier(this);
	}
	else {
		this->RemoveAll();
		return;
	}
}

void C������UI::OnClick(CControlUI* Click)
{
	if (_tcscmp(Click->GetName(), _T("map_ze")) == 0)ShowServerList("����", NULL, Is93x);
	else if (_tcscmp(Click->GetName(), _T("map_zm")) == 0)ShowServerList("��Ⱦ", NULL, Is93x);
	else if (_tcscmp(Click->GetName(), _T("map_ZombieBuilder")) == 0)ShowServerList("��", NULL, Is93x);
	else if (_tcscmp(Click->GetName(), _T("map_mg")) == 0)ShowServerList("����", NULL, Is93x);
	else if (_tcscmp(Click->GetName(), _T("map_HideAndSeek")) == 0)ShowServerList("��", NULL, Is93x);
	else if (_tcscmp(Click->GetName(), _T("map_kz")) == 0)ShowServerList("kz", 1, Is93x);
	else if (_tcscmp(Click->GetName(), _T("map_dymz")) == 0)ShowServerList("��", NULL, Is93x);
	else if (_tcscmp(Click->GetName(), _T("map_athletics")) == 0)ShowServerList("����", NULL, Is93x);
	else if (_tcscmp(Click->GetName(), _T("map_AllServer")) == 0)ShowServerList("", NULL, Is93x);
	else if (_tcscmp(Click->GetName(), _T("map_Search")) == 0)ShowServerList("map_Search", 2, Is93x);
	else if (_tcscmp(Click->GetName(), _T("93x_btn")) == 0) {
		Is93x = !Is93x; 
		ShowServerList(g_Current_server_name.name.c_str(), g_Current_server_name.type, Is93x); 

	}
}

void C������UI::RefreshServer()
{
	ShowServerList(g_Current_server_name.name.c_str(), g_Current_server_name.type, g_Current_server_name.Is93x);
}

void _stdcall MenuProc(HWND hWnd, MenuInfo* MenuInfo)
{
	if (pServer) {
		if (_tcscmp(MenuInfo->MenuText.GetData(), _T("ˢ�·�����")) == 0)pServer->ShowServerList(g_Current_server_name.name.c_str(), g_Current_server_name.type, g_Current_server_name.Is93x);
		else if (_tcscmp(MenuInfo->MenuText.GetData(), _T("����IP��ַ")) == 0)pServer->CopyServerInfo(3);
		else if (_tcscmp(MenuInfo->MenuText.GetData(), _T("���Ƶ�ͼ��")) == 0)pServer->CopyServerInfo(1);
		else if (_tcscmp(MenuInfo->MenuText.GetData(), _T("���������")) == 0)pServer->JoinServer();
		else if (_tcscmp(MenuInfo->MenuText.GetData(), _T("�鿴���")) == 0)pServer->OnLookPlayer();
		else if (_tcscmp(MenuInfo->MenuText.GetData(), _T("�л���ͼ����ʾ��ʽ")) == 0) {
			IsChineseMap = !IsChineseMap;
			pServer->ShowServerList(g_Current_server_name.name.c_str(), g_Current_server_name.type, g_Current_server_name.Is93x);
		}
	}
} 

void C������UI::OnLookPlayer()
{
	CListUI* pList = static_cast<CListUI*>(m_paManager->FindControl(_T("List_Server")));
	CListTextElementUI* pItem = static_cast<CListTextElementUI*>(pList->GetItemAt(pList->GetCurSel()));
	CDuiString ItemText = pItem->GetText(3);
	g_ServerName_LookPlayer = ItemText;
	C��ѯ���������UI* pSetting = new C��ѯ���������UI();
	if (pSetting == NULL) return;
	pSetting->Create(NULL, _T("�鿴������"), UI_WNDSTYLE_DIALOG, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 900, 500);
	pSetting->CenterWindow();
	::ShowWindow(*pSetting, SW_SHOW);
	//::SetWindowPos(*pSetting, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void C������UI::ClearEdit()
{
	CEditUI* pSearch = static_cast<CEditUI*>(m_paManager->FindControl(_T("Edit_Search")));
	if (pSearch)pSearch->SetText(_T(""));
}

void C������UI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)OnCreate();
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);
	else if (_tcscmp(msg.sType, _T("itemactivate")) == 0)JoinServer();
	else if (_tcscmp(msg.sType, _T("setfocus")) == 0) {
		if (_tcscmp(msg.pSender->GetName(), _T("Edit_Search")) == 0)ClearEdit();
	}
	else if (_tcscmp(msg.sType, _T("menu")) == 0) {
		CMenuWnd* pMenu = new CMenuWnd(m_hWnd);
		CDuiPoint point = msg.ptMouse;
		ClientToScreen(m_hWnd, &point);
		STRINGorID xml(IDR_XML1);
		pMenu->Init(NULL, xml, _T("xml"), point, MenuProc);
	}

}

void C������UI::ShowServerList(const char* pName, UINT type /* = NULL */, bool Is93x)
{
	g_pZElauncher->OutTip(_T("���ڻ�ȡ��������,�����ĵȴ�...."));
	g_Current_server_name.name = pName;
	g_Current_server_name.type = type;
	g_Current_server_name.Is93x = Is93x;
	if (Is93x) {
		std::thread t1 = std::thread(&C������UI::Get93xServerList2, this, pName, type);
		t1.detach();
		//Get93xServerList2(pName, type);
	}
	else {
		std::thread t1 = std::thread(&C������UI::GetServerList2, this, pName, type);
		t1.detach();
	}
	g_pZElauncher->OutTip(_T("��������ȡ���"));
}

void C������UI::GetServerList(const char* pName, UINT type)
{
	CListUI* pList = static_cast<CListUI*>(m_paManager->FindControl(_T("List_Server")));
	pList->RemoveAll();
	std::string StrResult;
#pragma region ��ȡ������Url
	{
		lib_http::CLibhttp phttp;
		string url;
		strCoding encod;
		switch (type)
		{
		case 1:
			url = "https://csgo.wanmei.com/communityserver/2019?task=getlist&t=" + encod.UrlUTF8(const_cast<char*>(pName));
			break;
		case 2:
		{
			CEditUI* pSearch = static_cast<CEditUI*>(m_paManager->FindControl(_T("Edit_Search")));
			_bstr_t sSearchText = _T("");
			if (pSearch)sSearchText = pSearch->GetText().GetData();
			url = "https://csgo.wanmei.com/communityserver/2019?task=getlist&q=" + encod.UrlUTF8(static_cast<char*>(sSearchText));
		}
		break;
		default:
			url = "https://csgo.wanmei.com/communityserver/2019?task=getlist&q=" + encod.UrlUTF8(const_cast<char*>(pName));
			break;
		}
		phttp.GET(url, StrResult);
	}
#pragma endregion
	if (StrResult.length() < 10) { MessageBox(NULL, _T("��ȡ��������Ϣʧ��!"), NULL, NULL); return; }
	UINT nLen = StrResult.size() * sizeof(TCHAR);
	TCHAR* pStrHtml = new TCHAR[nLen];
	_MultiByteToWideChar(CP_UTF8, NULL, StrResult.c_str(), StrResult.length(), pStrHtml, nLen);
	UINT nLen2 = StrResult.size() * sizeof(TCHAR);
	char* pStrMulti = new char[nLen2];
	ZeroMemory(pStrMulti, nLen2);
	WideCharToMultiByte(CP_ACP, NULL, pStrHtml, nLen, pStrMulti, nLen2, NULL, NULL);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(pStrMulti, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
	{
		if (root.isNull())goto Exit;
		if (strcmp(root["status"].asCString(), "success") == 0) {
			Json::Value ServerList = root["result"]["serverList"];
			if (ServerList.isNull())goto Exit;
			for (UINT i = 0; i < ServerList.size(); i++) {
				std::string tmpStr;
				tmpStr = ServerList[i]["server_name"].asCString();
				if (strstr(tmpStr.c_str(), "X����")) {
					CListTextElementUI* pListElement = new CListTextElementUI;
					pListElement->SetTag(i);
					pList->Add(pListElement);
					_MultiByteToWideChar(CP_ACP, NULL, tmpStr.c_str(), tmpStr.size(), pStrHtml, nLen);
					pListElement->SetText(0, pStrHtml);

					tmpStr = ServerList[i]["Map"].asCString();
					_MultiByteToWideChar(CP_ACP, NULL, tmpStr.c_str(), tmpStr.size(), pStrHtml, nLen);
					pListElement->SetText(1, pStrHtml);

					char tmpbuf[50] = { 0 };
					sprintf(tmpbuf, "%d/%d", ServerList[i]["Players"].asInt(), ServerList[i]["MaxPlayers"].asInt());
					_MultiByteToWideChar(CP_ACP, NULL, tmpbuf, sizeof(tmpbuf), pStrHtml, nLen);
					pListElement->SetText(2, pStrHtml);

					tmpStr = ServerList[i]["server_addr"].asCString();
					_MultiByteToWideChar(CP_ACP, NULL, tmpStr.c_str(), tmpStr.size(), pStrHtml, nLen);
					pListElement->SetText(3, pStrHtml);
				}
			}
			for (UINT i = 0; i < ServerList.size(); i++) {
				std::string tmpStr;
				tmpStr = ServerList[i]["server_name"].asCString();
				if (!strstr(tmpStr.c_str(), "X����")) {
					CListTextElementUI* pListElement = new CListTextElementUI;
					pListElement->SetTag(i);
					pList->Add(pListElement);
					_MultiByteToWideChar(CP_ACP, NULL, tmpStr.c_str(), tmpStr.size(), pStrHtml, nLen);
					pListElement->SetText(0, pStrHtml);

					tmpStr = ServerList[i]["Map"].asCString();
					_MultiByteToWideChar(CP_ACP, NULL, tmpStr.c_str(), tmpStr.size(), pStrHtml, nLen);
					pListElement->SetText(1, pStrHtml);

					char tmpbuf[50] = { 0 };
					sprintf(tmpbuf, "%d/%d", ServerList[i]["Players"].asInt(), ServerList[i]["MaxPlayers"].asInt());
					_MultiByteToWideChar(CP_ACP, NULL, tmpbuf, sizeof(tmpbuf), pStrHtml, nLen);
					pListElement->SetText(2, pStrHtml);

					tmpStr = ServerList[i]["server_addr"].asCString();
					_MultiByteToWideChar(CP_ACP, NULL, tmpStr.c_str(), tmpStr.size(), pStrHtml, nLen);
					pListElement->SetText(3, pStrHtml);
				}
			}
		}
	}
Exit:
	delete[] pStrHtml;
	delete[] pStrMulti;
}

void C������UI::Get93xServerList(const char* pName, UINT type)
{
	CListUI* pList = static_cast<CListUI*>(m_paManager->FindControl(_T("List_Server")));
	pList->RemoveAll();
	//��ȡ��ҳJson����
	std::string StrResult;
	{
		lib_http::CLibhttp phttp;
		string url = "https://serverssoftware.93x.net/api/servers?game=csgo";
		if (!phttp.GET(url, StrResult))MessageBox(NULL, _T("��ȡ��������Ϣʧ��!"), NULL, NULL);
	}
	//���÷�������ʾ����
	std::vector<std::string> VectorType;
	string pType;
#pragma region ����������
	//�°�ʶ��
	if (strstr(pName, "����"))VectorType.emplace_back("ze");
	else if (strstr(pName, "��Ⱦ"))VectorType.emplace_back("zm");
	else if (strstr(pName, "����")) {
		VectorType.emplace_back("mg");
		VectorType.emplace_back("hg");
		VectorType.emplace_back("warmod");
	}
	else if (strstr(pName, "��")) {
		VectorType.emplace_back("prophurt");
		VectorType.emplace_back("hns");
	}
	else if (strstr(pName, "kz")) {
		VectorType.emplace_back("kz");
		VectorType.emplace_back("surf");
		VectorType.emplace_back("bhop");
	}
	else if (strstr(pName, "��")) {
		VectorType.emplace_back("ttt");
		VectorType.emplace_back("ba");
	}
	else if (strstr(pName, "����")) {
		VectorType.emplace_back("awp");
		VectorType.emplace_back("dm");
		VectorType.emplace_back("1hp");
		VectorType.emplace_back("duel");
		VectorType.emplace_back("de");
		VectorType.emplace_back("35hp");
	}
	else if (strstr(pName, "��"))VectorType.emplace_back("builder");
	else VectorType.emplace_back("");;
	//�ɰ�ʶ��
	/*if (strstr(pName, "����"))pType = "ze";
	else if (strstr(pName, "��Ⱦ"))pType = "zm";
	else if (strstr(pName, "����"))pType = "mg";
	else if (strstr(pName, "��"))pType = "prophurt";
	else if (strstr(pName, "kz"))pType = "kz";
	else if (strstr(pName, "��"))pType = "ttt";
	else if (strstr(pName, "����"))pType = "Builder";
	else if (strstr(pName, "��"))pType = "Builder";
	else pType = "";*/
#pragma endregion
	//����ת��
	UINT nLen = StrResult.size() * sizeof(TCHAR);
	TCHAR* pStrHtml = new TCHAR[nLen];
	_MultiByteToWideChar(CP_UTF8, NULL, StrResult.c_str(), StrResult.length(), pStrHtml, nLen);
	UINT nLen2 = StrResult.size() * sizeof(TCHAR);
	char* pStrMulti = new char[nLen2];
	ZeroMemory(pStrMulti, nLen2);
	WideCharToMultiByte(CP_ACP, NULL, pStrHtml, nLen, pStrMulti, nLen2, NULL, NULL);
#pragma region �������б�Json��ȡ
	Json::Reader reader;
	Json::Value root;
	if (reader.parse(pStrMulti, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
	{
		if (root.isNull())goto Exit;
		for (UINT i = 0; i < root.size(); i++) {
			std::string tmpStr;
			tmpStr = root[i]["mode"].asCString();
			//�°�ʶ��
			for (UINT n = 0; n < VectorType.size(); n++) {
				if (strstr(tmpStr.c_str(), VectorType[n].c_str())) {
					tmpStr = root[i]["name"].asCString();
					CListTextElementUI* pListElement = new CListTextElementUI;
					pListElement->SetTag(i);
					pList->Add(pListElement);
					_MultiByteToWideChar(CP_ACP, NULL, tmpStr.c_str(), tmpStr.size(), pStrHtml, nLen);
					pListElement->SetText(0, pStrHtml);

					tmpStr = root[i]["map_name"].asCString();
					_MultiByteToWideChar(CP_ACP, NULL, tmpStr.c_str(), tmpStr.size(), pStrHtml, nLen);
					pListElement->SetText(1, pStrHtml);

					char tmpbuf[250] = { 0 };
					sprintf(tmpbuf, "%d/%d", root[i]["players"].asInt(), root[i]["max_players"].asInt());
					_MultiByteToWideChar(CP_ACP, NULL, tmpbuf, sizeof(tmpbuf), pStrHtml, nLen);
					pListElement->SetText(2, pStrHtml);

					tmpStr = root[i]["ip"].asCString();
					sprintf(tmpbuf, "%s:%s", tmpStr.c_str(), root[i]["port"].asCString());
					_MultiByteToWideChar(CP_ACP, NULL, tmpbuf, sizeof(tmpbuf), pStrHtml, nLen);
					pListElement->SetText(3, pStrHtml);
				}
			}
			//�ɰ�ʶ��
			/*if (strstr(tmpStr.c_str(), pType.c_str())) {
				tmpStr = root[i]["name"].asCString();
				CListTextElementUI* pListElement = new CListTextElementUI;
				pListElement->SetTag(i);
				pList->Add(pListElement);
				_MultiByteToWideChar(CP_ACP, NULL, tmpStr.c_str(), tmpStr.size(), pStrHtml, nLen);
				pListElement->SetText(0, pStrHtml);

				tmpStr = root[i]["map_name"].asCString();
				_MultiByteToWideChar(CP_ACP, NULL, tmpStr.c_str(), tmpStr.size(), pStrHtml, nLen);
				pListElement->SetText(1, pStrHtml);

				char tmpbuf[250] = { 0 };
				sprintf(tmpbuf, "%d/%d", root[i]["players"].asInt(), root[i]["max_players"].asInt());
				_MultiByteToWideChar(CP_ACP, NULL, tmpbuf, sizeof(tmpbuf), pStrHtml, nLen);
				pListElement->SetText(2, pStrHtml);

				tmpStr = root[i]["ip"].asCString();
				sprintf(tmpbuf, "%s:%s", tmpStr.c_str(), root[i]["port"].asCString());
				_MultiByteToWideChar(CP_ACP, NULL, tmpbuf, sizeof(tmpbuf), pStrHtml, nLen);
				pListElement->SetText(3, pStrHtml);
			}*/
		}
	}
#pragma endregion
	Exit:
	VectorType.clear();
	//std::vector<std::string>().swap(VectorType);
	VectorType.shrink_to_fit();
	delete[] pStrHtml;
	delete[] pStrMulti;
}

void C������UI::GetServerList2(const char* pName, UINT type)
{
	CListUI* pList = static_cast<CListUI*>(m_paManager->FindControl(_T("List_Server")));
	pList->RemoveAll();

	std::string StrResult;
	std::vector < _bstr_t> VectorType;
#pragma region ����������
	//�°�ʶ��
	if (strstr(pName, "����")) {
		VectorType.emplace_back(pName);
		VectorType.emplace_back("ze");
		VectorType.emplace_back("����");
	}
	else if (strstr(pName, "��Ⱦ")) {
		VectorType.emplace_back(pName);
		VectorType.emplace_back("zm");
	}
	else if (strstr(pName, "����")) {
		VectorType.emplace_back(pName);
		VectorType.emplace_back("����");
		VectorType.emplace_back("mg");
	}
	else if (strstr(pName, "��")) {
		VectorType.emplace_back(pName);
	}
	else if (strstr(pName, "kz")) {
		VectorType.emplace_back("kz");
		VectorType.emplace_back("surf");
		VectorType.emplace_back("bhop");
	}
	else if (strstr(pName, "��")) {
		VectorType.emplace_back(pName);
		VectorType.emplace_back("ttt");
		VectorType.emplace_back("Խ��");
	}
	else if (strstr(pName, "����")) {
		VectorType.emplace_back(pName);
		VectorType.emplace_back("awp");
		VectorType.emplace_back("ǹ");
		VectorType.emplace_back("��");
		VectorType.emplace_back("����");
		VectorType.emplace_back("��");
	}
	else if (strstr(pName, "��"))VectorType.emplace_back("��");
	else if (strstr(pName, "map_Search")) {
		CEditUI* pSearch = static_cast<CEditUI*>(m_paManager->FindControl(_T("Edit_Search")));
		if (pSearch) {
			_bstr_t sSearchText = pSearch->GetText().GetData();
			TCHAR* pPos = sSearchText;
			_tcstok(sSearchText, _T(","));
			if (!pPos)VectorType.emplace_back(sSearchText);
			else {
				while (pPos) {
					VectorType.emplace_back(pPos);
					pPos = _tcstok(NULL, _T(","));
				}
			}
		}
	}
	else VectorType.emplace_back("");;
	lib_http::CLibhttp http;
	http.GET(_T("https://csgo.wanmei.com/communityserver/2019?task=getlist"), StrResult);
#pragma endregion
	if (StrResult.length() < 10) { MessageBox(NULL, _T("��ȡ��������Ϣʧ��!"), NULL, NULL); return; }
	UINT nLen = StrResult.size() * sizeof(TCHAR);
	TCHAR* pStrHtml = new TCHAR[nLen];
	_MultiByteToWideChar(CP_UTF8, NULL, StrResult.c_str(), StrResult.length(), pStrHtml, nLen);
	UINT nLen2 = StrResult.size() * sizeof(TCHAR);
	char* pStrMulti = new char[nLen2];
	ZeroMemory(pStrMulti, nLen2);
	WideCharToMultiByte(CP_ACP, NULL, pStrHtml, nLen, pStrMulti, nLen2, NULL, NULL);

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(pStrMulti, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
	{
		if (root.isNull())goto Exit;
		if (strcmp(root["status"].asCString(), "success") == 0) {
			if (root["result"]["serverList"].isNull())goto Exit;
			Json::Value ServerList = root["result"]["serverList"];
			if (ServerList.isNull())goto Exit;
#pragma region X������������ȡ
			for (UINT i = 0; i < ServerList.size(); i++) {
				std::string tmpStr;
				if(ServerList[i]["server_name"].isNull())continue;
				tmpStr = ServerList[i]["server_name"].asCString();
				transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(), ::tolower);
				for (UINT j = 0; j < VectorType.size(); j++) {
					if (strstr(tmpStr.c_str(), VectorType[j]) && strstr(tmpStr.c_str(), "x����")) {
						CListTextElementUI* pListElement = new CListTextElementUI;
						pListElement->SetTag(i);
						pList->Add(pListElement);
						_bstr_t sText = ServerList[i]["server_name"].asCString();
						pListElement->SetText(0, sText);

						if (ServerList[i]["Map"].isNull())continue;
						sText = ServerList[i]["Map"].asCString();
						if (IsChineseMap) {
							_bstr_t tmptext = g_pZElauncher->GetChineseMapName(sText);
							if (tmptext.length() > 5)sText = tmptext;
						}
						pListElement->SetText(1, sText);
						if (ServerList[i]["Players"].isNull())continue;
						if (ServerList[i]["MaxPlayers"].isNull())continue;
						TCHAR tmpbuf[50] = { 0 };
						_stprintf(tmpbuf, _T("%d/%d"), ServerList[i]["Players"].asInt(), ServerList[i]["MaxPlayers"].asInt());
						pListElement->SetText(2, tmpbuf);
						if (ServerList[i]["server_addr"].isNull())continue;
						sText = ServerList[i]["server_addr"].asCString();
						pListElement->SetText(3, sText);

						break;//����������ͬ����ظ���������
					}
				}

			}
#pragma endregion
#pragma region X��������
			//���͵�ð������
			int list93xCount = pList->GetCount();
			{
				for (int j = 1; j <= list93xCount; j++)
				{
					for (int i = 0; i < list93xCount - j; i++)
					{
						CListTextElementUI* pItem = static_cast<CListTextElementUI*>(pList->GetItemAt(i));
						CListTextElementUI* pItem2 = static_cast<CListTextElementUI*>(pList->GetItemAt(i + 1));
						_bstr_t temp1 = pItem->GetText(2);
						_bstr_t temp2 = pItem2->GetText(2);
						UINT num1 = _wtoi(temp1);
						UINT num2 = _wtoi(temp2);
						if (num1 < num2)
						{
							for (int n = 0; n < 4; n++)//��Ϊ���б�Ϊ��
							{
								CDuiString st1 = pItem->GetText(n);
								CDuiString st2 = pItem2->GetText(n);
								pItem->SetText(n, st2);
								pItem2->SetText(n, st1);
							}
						}
					}
				}
			}
#pragma endregion
//����������������ȡ
/*
#pragma region ����������������ȡ
			for (UINT i = 0; i < ServerList.size(); i++) {
				std::string tmpStr;
				if (ServerList[i]["server_name"].isNull())continue;
				tmpStr = ServerList[i]["server_name"].asCString();
				transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(), ::tolower);
				for (UINT j = 0; j < VectorType.size(); j++) {
					if (strstr(tmpStr.c_str(), VectorType[j]) && !strstr(tmpStr.c_str(), "x����")) {
						CListTextElementUI* pListElement = new CListTextElementUI;
						pListElement->SetTag(i);
						pList->Add(pListElement);
						_bstr_t sText = ServerList[i]["server_name"].asCString();
						pListElement->SetText(0, sText);

						if (ServerList[i]["Map"].isNull())continue;
						sText = ServerList[i]["Map"].asCString();
						if (IsChineseMap) {
							for (UINT j = 0; j < g_MapChinese.size(); j++) {
								if (strcmp(sText, g_MapChinese[j].Map_en.c_str()) == 0) {
									sText = g_MapChinese[j].Map_cn.c_str();
									sText += "(";
									sText += g_MapChinese[j].Map_en.c_str();
									sText += ")";
									break;
								}
							}
						}
						pListElement->SetText(1, sText);

						if (ServerList[i]["Players"].isNull())continue;
						if (ServerList[i]["MaxPlayers"].isNull())continue;
						TCHAR tmpbuf[50] = { 0 };
						_stprintf(tmpbuf, _T("%d/%d"), ServerList[i]["Players"].asInt(), ServerList[i]["MaxPlayers"].asInt());
						pListElement->SetText(2, tmpbuf);
						if (ServerList[i]["server_addr"].isNull())continue;
						sText = ServerList[i]["server_addr"].asCString();
						pListElement->SetText(3, sText);
						break;//����������ͬ����ظ���������
					}
				}
			}
#pragma endregion
#pragma region ������������
			{
				//���͵�ð������
				int listcount = pList->GetCount();
				for (int j = 1; j <= listcount; j++)
				{
					for (int i = 0; i < listcount - j; i++)
					{
						CListTextElementUI* pItem = static_cast<CListTextElementUI*>(pList->GetItemAt(i));
						if (!pItem)continue;
						CListTextElementUI* pItem2 = static_cast<CListTextElementUI*>(pList->GetItemAt(i + 1));
						if (!pItem2)continue;
						CDuiString temp1 = pItem->GetText(2);
						CDuiString temp2 = pItem2->GetText(2);
						if (_tcsstr(pItem->GetText(0), _T("X����")))continue;
						UINT num1 = _wtoi(temp1);
						UINT num2 = _wtoi(temp2);
						if (num1 < num2)
						{
							for (int n = 0; n < 4; n++)//��Ϊ���б�Ϊ��
							{
								CDuiString st1 = pItem->GetText(n);
								CDuiString st2 = pItem2->GetText(n);
								pItem->SetText(n, st2);
								pItem2->SetText(n, st1);
							}
						}
					}
				}
			}
#pragma endregion*/
		}
	}
	else goto Exit;
Exit:
	VectorType.clear();
	VectorType.shrink_to_fit();
	delete[] pStrHtml;
	delete[] pStrMulti;
}

void C������UI::Get93xServerList2(const char* pName, UINT type)
{
	CListUI* pList = static_cast<CListUI*>(m_paManager->FindControl(_T("List_Server")));
	pList->RemoveAll();
	//��ȡ��ҳJson����
	std::string StrResult;
	{
		lib_http::CLibhttp phttp;
		string url = "https://serverssoftware.93x.net/api/servers?game=csgo";
		if (!phttp.GET(url, StrResult))MessageBox(NULL, _T("��ȡ��������Ϣʧ��!"), NULL, NULL);
	}
	//���÷�������ʾ����
	std::vector<_bstr_t> VectorType;
	string pType;
#pragma region ����������
	//�°�ʶ��
	if (strstr(pName, "����")) {
		VectorType.emplace_back(pName);
		VectorType.emplace_back("ze");
		VectorType.emplace_back("����");
	}
	else if (strstr(pName, "��Ⱦ")) {
		VectorType.emplace_back(pName);
		VectorType.emplace_back("zm");
	}
	else if (strstr(pName, "����")) {
		VectorType.emplace_back(pName);
		VectorType.emplace_back("����");
		VectorType.emplace_back("��");
		VectorType.emplace_back("mg");
	}
	else if (strstr(pName, "��")) {
		VectorType.emplace_back(pName);
	}
	else if (strstr(pName, "kz")) {
		VectorType.emplace_back("kz");
		VectorType.emplace_back("surf");
		VectorType.emplace_back("bhop");
	}
	else if (strstr(pName, "��")) {
		VectorType.emplace_back(pName);
		VectorType.emplace_back("ttt");
		VectorType.emplace_back("Խ��");
	}
	else if (strstr(pName, "����")) {
		VectorType.emplace_back(pName);
		VectorType.emplace_back("awp");
		VectorType.emplace_back("ǹ");
		VectorType.emplace_back("��");
		VectorType.emplace_back("����");
		VectorType.emplace_back("��");
	}
	else if (strstr(pName, "��"))VectorType.emplace_back("��");
	else if (strstr(pName, "map_Search")) {
		CEditUI* pSearch = static_cast<CEditUI*>(m_paManager->FindControl(_T("Edit_Search")));
		if (pSearch) {
			_bstr_t sSearchText = pSearch->GetText().GetData();
			TCHAR* pPos = sSearchText;
			_tcstok(sSearchText, _T(","));
			if (!pPos)VectorType.emplace_back(sSearchText);
			else {
				while (pPos) {
					VectorType.emplace_back(pPos);
					pPos = _tcstok(NULL, _T(","));
				}
			}
		}
	}
	else VectorType.emplace_back("");;
#pragma endregion
	//����ת��
	UINT nLen = StrResult.size() * sizeof(TCHAR);
	TCHAR* pStrHtml = new TCHAR[nLen];
	_MultiByteToWideChar(CP_UTF8, NULL, StrResult.c_str(), StrResult.length(), pStrHtml, nLen);
	UINT nLen2 = StrResult.size() * sizeof(TCHAR);
	char* pStrMulti = new char[nLen2];
	ZeroMemory(pStrMulti, nLen2);
	WideCharToMultiByte(CP_ACP, NULL, pStrHtml, nLen, pStrMulti, nLen2, NULL, NULL);
#pragma region �������б�Json��ȡ
	Json::Reader reader;
	Json::Value root;
	int listcount = 0;
	if (reader.parse(pStrMulti, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��  
	{
		//δ����
		for (UINT i = 0; i < root.size(); i++) {
			std::string tmpStr;
			if (root[i]["name"].isNull())break;
			tmpStr = root[i]["name"].asCString();
			if (tmpStr.empty())break;
			//�°�ʶ��
			for (UINT n = 0; n < VectorType.size(); n++) {
				transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(), ::tolower);
				if (strstr(tmpStr.c_str(), VectorType[n])) {
					_bstr_t sText = "";
					sText = root[i]["name"].asCString();
					CListTextElementUI* pListElement = new CListTextElementUI;
					pListElement->SetTag(i);
					pList->Add(pListElement);
					pListElement->SetText(0, sText);

					if (root[i]["map_name"].isNull())continue;
					sText = root[i]["map_name"].asCString();
					if (IsChineseMap) {
						_bstr_t tmptext = g_pZElauncher->GetChineseMapName(sText);
						if (tmptext.length() > 5)sText = tmptext;
					}
					pListElement->SetText(1, sText);

					if (root[i]["max_players"].isNull())continue;
					if (root[i]["players"].isNull())continue;
					TCHAR tmpbuf[250] = { 0 };
					_stprintf(tmpbuf, _T("%d/%d"), root[i]["players"].asInt(), root[i]["max_players"].asInt());
					pListElement->SetText(2, tmpbuf);

					if (root[i]["port"].isNull())continue;
					sText = root[i]["ip"].asCString();
					_bstr_t port = root[i]["port"].asCString();
					_stprintf(tmpbuf, _T("%s:%s"), static_cast<TCHAR*>(sText), static_cast<TCHAR*>(port));
					pListElement->SetText(3, tmpbuf);

					break;//����������ͬ����ظ���������
				}
			}
		}
	}
	else goto Exit;
#pragma region ����
	//���͵�ð������
	listcount = pList->GetCount();
	for (int j = 1; j <= listcount; j++)
	{
		for (int i = 0; i < listcount - j; i++)
		{
			CListTextElementUI* pItem = static_cast<CListTextElementUI*>(pList->GetItemAt(i));
			CListTextElementUI* pItem2 = static_cast<CListTextElementUI*>(pList->GetItemAt(i + 1));
			_bstr_t temp1 = pItem->GetText(2);
			_bstr_t temp2 = pItem2->GetText(2);
			UINT num1 = _wtoi(temp1);
			UINT num2 = _wtoi(temp2);
			if (num1 < num2)
			{
				for (int n = 0; n < 4; n++)//��Ϊ���б�Ϊ��
				{
					CDuiString st1 = pItem->GetText(n);
					CDuiString st2 = pItem2->GetText(n);
					pItem->SetText(n, st2);
					pItem2->SetText(n, st1);
				}
			}
		}
	}
#pragma endregion
#pragma endregion
	Exit:
	VectorType.clear();
	//std::vector<std::string>().swap(VectorType);
	VectorType.shrink_to_fit();
	delete[] pStrHtml;
	delete[] pStrMulti;
}

void C������UI::OnCreate()
{
	CListUI* plistbkimg = static_cast<CListUI*>(m_paManager->FindControl(_T("List_Server")));
	if (plistbkimg) {
		TCHAR cfgbuff[1024] = { 0 };
		GetRunPath(cfgbuff, sizeof(cfgbuff));
		_tcscat(cfgbuff, _T("\\bin\\Config.cfg"));
		TCHAR* pszbuff = new TCHAR[MAX_PATH]();
		GetPrivateProfileString(_T("ZElauncher"), _T("edit_listbkimge"), NULL, pszbuff, (MAX_PATH * sizeof(TCHAR)), cfgbuff);
		if (_tcslen(pszbuff) > 5) {
			plistbkimg->SetItemBkColor(RGB(0xFF, 0xE7, 0xE7, 0xE7));
			plistbkimg->SetBkImage(pszbuff);
		}
	}
	ShowServerList("����", NULL, Is93x);
}

void C������UI::CopyServerInfo(UINT nIndex)
{
	CListUI* pList = static_cast<CListUI*>(m_paManager->FindControl(_T("List_Server")));
	CListTextElementUI* pItem = static_cast<CListTextElementUI*>(pList->GetItemAt(pList->GetCurSel()));
	CDuiString ItemText = pItem->GetText(nIndex);
	int npos = ItemText.Find(_T("<"));
	if (npos != -1) {
		ItemText = ItemText.Mid(npos + 1, ItemText.Find(_T(">")) - (npos + 1));
	}
	OpenClipboard(NULL);//�򿪼��а�
	EmptyClipboard();//��ռ��а�
	HANDLE hHandle = GlobalAlloc(GMEM_FIXED, 4096);//�����ڴ�
	char* pData = (char*)GlobalLock(hHandle);//�����ڴ棬���������ڴ���׵�ַ
	ZeroMemory(pData, 4096);
	WideCharToMultiByte(CP_ACP, NULL, ItemText.GetData(), ItemText.GetLength(), pData, 4096, NULL, NULL);
	SetClipboardData(CF_TEXT, hHandle);//���ü��а�����
	GlobalUnlock(hHandle);//�������
	CloseClipboard();//�رռ��а�
}

bool C������UI::IsFileExist(LPCTSTR lpFileName)
{
	TCHAR CSGOPath[MAX_PATH] = { 0 };
	TCHAR CfgPath[MAX_PATH] = { 0 };
	GetRunPath(CfgPath, sizeof(CfgPath));
	_tcscat(CfgPath, _T("\\bin\\Config.cfg"));
	GetPrivateProfileString(_T("ZElauncher"), _T("CSGOPath"), NULL, CSGOPath, sizeof(CSGOPath), CfgPath);
	_bstr_t szTemp = "";
	WIN32_FIND_DATA FindData = { 0 };
	szTemp = CSGOPath;
	szTemp += _T("\\csgo\\maps\\*.bsp");;
	HANDLE hFind = FindFirstFile(szTemp, &FindData);
	if (hFind == INVALID_HANDLE_VALUE)return false;
	do 
	{
		std::wstring map_filename = FindData.cFileName, map_filename2;
		CDuiString map_name = lpFileName;
		int mPos = map_name.Find(_T("<"));
		if (mPos != -1) {
			map_name = map_name.Mid(mPos + 1, map_name.Find(_T(">")) - (mPos + 1));
		}
		map_filename2 = map_name;
		map_filename2 += L".bsp";
		transform(map_filename.begin(), map_filename.end(), map_filename.begin(), ::tolower);
		transform(map_filename2.begin(), map_filename2.end(), map_filename2.begin(), ::tolower);
		if (_tcscmp(map_filename.c_str(), map_filename2.c_str()) == 0)return true;
	} while (FindNextFile(hFind, &FindData));
	return false;
}

void C������UI::JoinServer()
{
	g_paManager = m_paManager;
	//���������
	CListUI* pList = static_cast<CListUI*>(m_paManager->FindControl(_T("List_Server")));
	if (!pList)return;
	CListTextElementUI* pItem = static_cast<CListTextElementUI*>(pList->GetItemAt(pList->GetCurSel()));
	if (!pItem)return;
	//��ȡ��������ַ;
	CDuiString Serveraddr = pItem->GetText(3);
	_bstr_t ServerName = pItem->GetText(0);
#pragma region �����Ϣ
	CDuiString Outtip = _T("���ڼ���->");
	Outtip += static_cast<TCHAR*>(ServerName);
	g_pZElauncher->OutTip(Outtip.GetData());
#pragma  endregion
#pragma region �жϵ�ͼ�Ƿ����
	//��ȡ��ͼ����;
	CDuiString mapName = pItem->GetText(1);
	if (!IsFileExist(mapName)) {
		if (MessageBox(NULL, _T("����û�е�ǰ��ͼ,�Ƿ����ص�ͼ��,������Ϸ?"), _T("Tip"), MB_YESNO) == IDYES) {
			g_mapName = mapName;
			std::string sTempstr= static_cast<char*>(ServerName);
			transform(sTempstr.begin(), sTempstr.end(), sTempstr.begin(), ::toupper);
			if (sTempstr.find("X����") != std::string::npos) {
				std::thread t1 = std::thread(&C������UI::DownloadMap93x, this, (LPCTSTR)g_mapName.GetData(), true);
				t1.detach();
			}
			else if (sTempstr.find("��ʬ��԰") != std::string::npos)CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)C������UI::DownloadMapjsly, (LPVOID)g_mapName.GetData(), NULL, NULL);
			else if (sTempstr.find("������") != std::string::npos)CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)C������UI::DownloadMapfys, (LPVOID)g_mapName.GetData(), NULL, NULL);
			else {
				g_pZElauncher->OutTip(_T("��֧�ֵ�������,�޷����ص�ͼ!"));
				RunGame(Serveraddr);
			}
			::SendMessage(m_hWnd, WM_LBUTTONDOWN, NULL, NULL);
			::SendMessage(m_hWnd, WM_LBUTTONUP, NULL, NULL);
			return;
		}
		else RunGame(Serveraddr);
	}
	else RunGame(Serveraddr);
#pragma endregion

}

void C������UI::RunGame(LPCTSTR lpServer)
{
	TCHAR SteamPath[MAX_PATH] = { 0 };
	TCHAR FilePath[MAX_PATH] = { 0 };
	TCHAR CfgPath[MAX_PATH] = { 0 };
	TCHAR Server_Switch[25] = { 0 };
	GetRunPath(CfgPath, sizeof(CfgPath));
	_tcscat(CfgPath, _T("\\bin\\Config.cfg"));
	GetPrivateProfileString(_T("ZElauncher"), _T("SteamPath"), NULL, SteamPath, sizeof(SteamPath), CfgPath);
	_tcscpy(FilePath, SteamPath);
	_tcscat(FilePath, _T("\\steam.exe"));
	GetPrivateProfileString(_T("ZElauncher"), _T("Server_Switch"), NULL, Server_Switch, sizeof(Server_Switch), CfgPath);
	CDuiString parameter;
	lib_Process::Process Process;
	if (Process.IsProcesNameExist(_T("csgo.exe"))) {
		parameter = _T("steam://connect/");
		parameter += lpServer;
		ShellExecute(NULL, _T("open"), FilePath, parameter.GetData(), SteamPath, SW_SHOW);
	}
	else {
		parameter = _T("-applaunch 730 ");
		if (_tcscmp(Server_Switch, _T("true")) == 0)parameter += _T("-worldwide -novid +connect ");
		else parameter += _T("-perfectworld -novid +connect ");
		parameter += lpServer;
		//�ж�Steam�Ƿ�����
		if (Process.IsProcesNameExist(_T("steam.exe")) || Process.IsProcesNameExist(_T("Steam.exe")) || Process.IsProcesNameExist(_T("STEAM.exe"))) {
			ShellExecute(NULL, _T("open"), FilePath, parameter.GetData(), SteamPath, SW_SHOW);
		}
		else {
			ShellExecute(NULL, _T("open"), _T("explorer.exe"), FilePath, SteamPath, SW_SHOW);
			Sleep(1000);
			ShellExecute(NULL, _T("open"), FilePath, parameter.GetData(), SteamPath, SW_SHOW);
		}
	}
}

bool __stdcall DownloadProc_(size_t FileSize, UINT nState, size_t DownloadSize, size_t SecondSize)
{
	CDuiString Temp;
	if (nState == 2)
	{
		Temp = _T("<");
		Temp += g_mapName;
		Temp += _T(">��ͼ�������");
		g_pZElauncher->OutTip(Temp.GetData());
	}
	else if (nState == 1)
	{
		TCHAR szTemp[1024] = { 0 };
		Temp = _T("��������<");
		Temp += g_mapName;
		Temp += _T("> �����ٶ� %.2lf%s/S");
		float nSize = static_cast<float>(SecondSize), nDownlen = 0;
		TCHAR dwType[5] = _T("B");
		nDownlen = nSize / 1024;
		if (nDownlen >= 1) {
			nSize = nDownlen;
			_tcscpy(dwType, _T("KB"));
			nDownlen /= 1024;
			if (nDownlen >= 1) {
				nSize = nDownlen;
				_tcscpy(dwType, _T("MB"));
				nDownlen /= 1024;
				if (nDownlen >= 1) {
					nSize = nDownlen;
					_tcscpy(dwType, _T("GB"));
				}
				else nDownlen = nSize;
			}
			else nDownlen = nSize;
		}
		else nDownlen = nSize;
		_stprintf(szTemp, Temp.GetData(), nDownlen, dwType);
		g_pZElauncher->OutTip(szTemp);

	}
	return true;
}

void C������UI::DownloadMap93x(LPCTSTR lpMapName, bool IsRunGame)
{

	//���������
	CListUI* pList = static_cast<CListUI*>(m_paManager->FindControl(_T("List_Server")));
	if (!pList)return;
	//���ص�ͼ
	/*CDuiString sUrl = _T("http://softpredownload.93x.net:999/fastdlze/maps/");
	sUrl += lpMapName;
	sUrl += _T(".bsp.bz2");*/
	CDuiString map_name = lpMapName;
	g_mapName = map_name;
	int mPos = map_name.Find(_T("<"));
	if (mPos != -1) {
		map_name = map_name.Mid(mPos + 1, map_name.Find(_T(">")) - (mPos + 1));
	}
	_bstr_t map_downloadurl = "http://softpredownload.93x.net:999/";
	std::wstring map_type = lpMapName;
	transform(map_type.begin(), map_type.end(), map_type.begin(), ::tolower);
	if (map_type.find(L"ze_") != std::wstring::npos) {
		map_downloadurl += "fastdlze/maps/";
	} 
	else map_downloadurl += "dmgg93xnet/maps/";
	map_downloadurl += map_name.GetData();
	map_downloadurl += ".bsp.bz2";
	/*//��ַ�޷����غͷ���
	for (UINT j = 0; j < g_MapChinese.size(); j++) {
		if (wcscmp(map_name, g_MapChinese[j].Map_en) == 0) {
			//map_downloadurl = g_MapChinese[j].MapDownloadurl;
			break;
		}
	}*/
	TCHAR CSGOPath[MAX_PATH] = { 0 };
	TCHAR TempPath[MAX_PATH] = { 0 };
	TCHAR CfgPath[MAX_PATH] = { 0 };
	GetRunPath(CfgPath, sizeof(CfgPath));
	_tcscat(CfgPath, _T("\\bin\\Config.cfg"));
	GetPrivateProfileString(_T("ZElauncher"), _T("CSGOPath"), NULL, CSGOPath, sizeof(CSGOPath), CfgPath);
	CDuiString sMapFile = CSGOPath;
	sMapFile += _T("\\csgo\\maps\\");
	GetTempPath(MAX_PATH, TempPath);
	CDuiString sWriteFile = TempPath;
	sWriteFile += map_name;
	sWriteFile += _T(".bsp.bz2");
	lib_http::CLibhttp http;
	if (http.DownloadFile(map_downloadurl.GetBSTR(), sWriteFile.GetData(), DownloadProc_)) {
		std::string szOutPut;
		if (Zip7ZUnCompressed(sWriteFile.GetData(), _T(""), sMapFile.GetData(), szOutPut, NULL)) {
			_bstr_t sTip = _T("<");
			sTip += map_name.GetData();
			sTip += _T(">��ͼ��ѹ���");
			g_pZElauncher->OutTip(sTip);
			DeleteFile(sWriteFile.GetData());
		}
		else {
			UINT nPos = szOutPut.find("7-Zip (a) 9.22 beta  Copyright (c) 1999-2011 Igor Pavlov  2011-04-18") + _tcslen(_T("7-Zip (a) 9.22 beta  Copyright (c) 1999-2011 Igor Pavlov  2011-04-18"));
			if (nPos != std::string::npos) {
				_bstr_t Stip = szOutPut.substr(nPos, szOutPut.length() - nPos).c_str();
				g_pZElauncher->OutTip(Stip);
			}
			else g_pZElauncher->OutTip(_T("��ѹʧ��,����δ֪����!"));
		}
	}
	else {
		_bstr_t sTip = _T("��ͼ����ʧ��,δ��Ѱ��<");
		sTip += map_name.GetData();
		sTip += _T(">��ͼ����������");
		g_pZElauncher->OutTip(sTip);
	}
	if (IsRunGame)
	{
		CListTextElementUI* pItem = static_cast<CListTextElementUI*>(pList->GetItemAt(pList->GetCurSel()));
		if (!pItem)return;
		//��ȡ��������ַ;
		CDuiString Serveraddr = pItem->GetText(3);
		pServer->RunGame(Serveraddr);
	}
}

void C������UI::DownloadMapjsly(LPCTSTR lpMapName)
{

	//���������
	CListUI* pList = static_cast<CListUI*>(g_paManager->FindControl(_T("List_Server")));
	if (!pList)return;
	CListTextElementUI* pItem = static_cast<CListTextElementUI*>(pList->GetItemAt(pList->GetCurSel()));
	if (!pItem)return;
	//��ȡ��������ַ;
	CDuiString Serveraddr = pItem->GetText(3);
	//���ص�ͼ
	CDuiString sUrl = _T("http://fastdl.zombieden.cn/csgo/maps/");
	sUrl += lpMapName;
	sUrl += _T(".bsp.bz2");
	TCHAR CSGOPath[MAX_PATH] = { 0 };
	TCHAR TempPath[MAX_PATH] = { 0 };
	TCHAR CfgPath[MAX_PATH] = { 0 };
	GetRunPath(CfgPath, sizeof(CfgPath));
	_tcscat(CfgPath, _T("\\bin\\Config.cfg"));
	GetPrivateProfileString(_T("ZElauncher"), _T("CSGOPath"), NULL, CSGOPath, sizeof(CSGOPath), CfgPath);
	CDuiString sMapFile = CSGOPath;
	sMapFile += _T("\\csgo\\maps\\");
	GetTempPath(MAX_PATH, TempPath);
	CDuiString sWriteFile = TempPath;
	sWriteFile += lpMapName;
	sWriteFile += _T(".bsp.bz2");
	lib_http::CLibhttp http;
	if (http.DownloadFile(sUrl.GetData(), sWriteFile.GetData(), DownloadProc_)) {
		std::string szOutPut;
		if (Zip7ZUnCompressed(sWriteFile.GetData(), _T(""), sMapFile.GetData(), szOutPut, NULL)) {
			_bstr_t sTip = _T("<");
			sTip += lpMapName;
			sTip += _T(">��ͼ��ѹ���");
			g_pZElauncher->OutTip(sTip);
			DeleteFile(sWriteFile.GetData());
		}
		else {
			UINT nPos = szOutPut.find("7-Zip (a) 9.22 beta  Copyright (c) 1999-2011 Igor Pavlov  2011-04-18") + _tcslen(_T("7-Zip (a) 9.22 beta  Copyright (c) 1999-2011 Igor Pavlov  2011-04-18"));
			if (nPos != std::string::npos) {
				_bstr_t Stip = szOutPut.substr(nPos, szOutPut.length() - nPos).c_str();
				g_pZElauncher->OutTip(Stip);
			}
			else g_pZElauncher->OutTip(_T("��ѹʧ��,����δ֪����!"));
		}
		pServer->RunGame(Serveraddr);
	}
	else {
		g_pZElauncher->OutTip(_T("��ͼ����ʧ��,Ŀǰ��֧��ZE��ʬ���ܵ�ͼ!"));
		pServer->RunGame(Serveraddr);
	}

}

void C������UI::DownloadMapfys(LPCTSTR lpMapName)
{

	//���������
	CListUI* pList = static_cast<CListUI*>(g_paManager->FindControl(_T("List_Server")));
	if (!pList)return;
	CListTextElementUI* pItem = static_cast<CListTextElementUI*>(pList->GetItemAt(pList->GetCurSel()));
	if (!pItem)return;
	//��ȡ��������ַ;
	CDuiString Serveraddr = pItem->GetText(3);
	//���ص�ͼ
	CDuiString sUrl = _T("https://filedl.1mgou.com/fys_maps/");
	sUrl += lpMapName;
	sUrl += _T(".bsp.bz2");
	TCHAR CSGOPath[MAX_PATH] = { 0 };
	TCHAR TempPath[MAX_PATH] = { 0 };
	TCHAR CfgPath[MAX_PATH] = { 0 };
	GetRunPath(CfgPath, sizeof(CfgPath));
	_tcscat(CfgPath, _T("\\bin\\Config.cfg"));
	GetPrivateProfileString(_T("ZElauncher"), _T("CSGOPath"), NULL, CSGOPath, sizeof(CSGOPath), CfgPath);
	CDuiString sMapFile = CSGOPath;
	sMapFile += _T("\\csgo\\maps\\");
	GetTempPath(MAX_PATH, TempPath);
	CDuiString sWriteFile = TempPath;
	sWriteFile += lpMapName;
	sWriteFile += _T(".bsp.bz2");
	lib_http::CLibhttp http;
	if (http.DownloadFile(sUrl.GetData(), sWriteFile.GetData(), DownloadProc_)) {
		std::string szOutPut;
		if (Zip7ZUnCompressed(sWriteFile.GetData(), _T(""), sMapFile.GetData(), szOutPut, NULL)) {
			_bstr_t sTip = _T("<");
			sTip += lpMapName;
			sTip += _T(">��ͼ��ѹ���");
			g_pZElauncher->OutTip(sTip);
			DeleteFile(sWriteFile.GetData());
		}
		else {
			UINT nPos = szOutPut.find("7-Zip (a) 9.22 beta  Copyright (c) 1999-2011 Igor Pavlov  2011-04-18") + _tcslen(_T("7-Zip (a) 9.22 beta  Copyright (c) 1999-2011 Igor Pavlov  2011-04-18"));
			if (nPos != std::string::npos) {
				_bstr_t Stip = szOutPut.substr(nPos, szOutPut.length() - nPos).c_str();
				g_pZElauncher->OutTip(Stip);
			}
			else g_pZElauncher->OutTip(_T("��ѹʧ��,����δ֪����!"));
		}
		pServer->RunGame(Serveraddr);

	}
	else {
		g_pZElauncher->OutTip(_T("��ͼ����ʧ��,Ŀǰ��֧��ZE��ʬ���ܵ�ͼ!"));
		pServer->RunGame(Serveraddr);
	}

}
