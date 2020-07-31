#include "�̳�.h"

C�̳�UI::C�̳�UI(CPaintManagerUI& m_paintmanage)
{
	m_paintmanager = &m_paintmanage;
	m_PageNum = 1;
	CDialogBuilder Builder;
	CContainerUI* pShop = static_cast<CContainerUI*>(Builder.Create(_T("Shop.xml"), NULL, NULL, &m_pm));
	if (pShop) {
		this->Add(pShop);
		m_paintmanager->AddNotifier(this);
	}
	else {
		this->RemoveAll();
		return;
	}
}

void C�̳�UI::OnClick(CControlUI* Click)
{
	if (_tcscmp(Click->GetName(), _T("page_up")) == 0) { 
		m_PageNum--; 
		if (m_PageNum < 1)m_PageNum = 1;
		else ��ȡ�̳�����();
	}
	else if (_tcscmp(Click->GetName(), _T("page_next")) == 0) { 
		m_PageNum++; 
		if (m_PageNum > m_pageNumMax)m_PageNum = m_pageNumMax;
		else ��ȡ�̳�����();
	}
	else if (_tcsstr(Click->GetName(), _T("Label_Skin")))
	{
		CDuiString url = _T("https://bbs.93x.net/");
		url += Click->GetUserData();
		url.Replace(_T("amp;"), _T(""));
		ShellExecute(NULL, _T("open"), url, NULL, NULL, SW_NORMAL);
	}
}

void C�̳�UI::Notify(TNotifyUI& msg)
{

	if (_tcscmp(msg.sType, _T("windowinit")) == 0)OnCreate();
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);

}

std::string C�̳�UI::��ȡƤ��html����(const TCHAR* Page)
{
	std::string Result;
	{
		lib_http::CLibhttp http;
		CDuiString url = _T("https://bbs.93x.net/plugin.php?id=dc_mall&uid=0&page=");
		url += Page;
		CDuiString Cookies = gCookies.GetBSTR();
		http.GET(url.GetData(), Result, _T(""), Cookies.GetData());
		//http.GET(url.GetData(), Result);
	}
	return std::move(Result);
}

void C�̳�UI::����ͼƬ����״̬()
{
	for (int i = 0; i < 12; i++) {
		TCHAR szbuf[1024] = { 0 };
		_stprintf(szbuf, _T("panel_Skin%d"), i + 1);
		CContainerUI* pPanel = static_cast<CContainerUI*>(m_paintmanager->FindControl(szbuf));
		if (pPanel)pPanel->SetVisible(false);
	}
}

bool C�̳�UI::�ж�������ʽ���ݿ��Ƿ����(TCHAR* ������ʽ)
{
	TCHAR dbPath[MAX_PATH] = { 0 };
	GetRunPath(dbPath, sizeof(dbPath));
	_tcscat(dbPath, _T("\\bin\\Regex.db"));
	GetPrivateProfileString(_T("ZElauncher"), _T("Shop"), NULL, ������ʽ, 4096, dbPath);
	if (_tcslen(������ʽ) < 5)return false; 
	return true;
}

void C�̳�UI::��ȡ�̳�����()
{
	����ͼƬ����״̬();
	CButtonUI* pLogin = static_cast<CButtonUI*>(m_paintmanager->FindControl(_T("Button_Login")));
	if (!pLogin)return;
	if (_tcscmp(pLogin->GetText(), _T("{u}{a}δ��¼{/a}{/u}")) == 0 || _tcscmp(pLogin->GetText(), _T("{u}{a}��¼ʧ��{/a}{/u}")) == 0 || _tcscmp(pLogin->GetText(), _T("{u}{a}Cookies��ȡΪ��{/a}{/u}")) == 0) {
		MessageBox(NULL, _T("��ȡ�̳���Ϣʧ��,���ȵ�¼!"), NULL, MB_OK | MB_TOPMOST);
		g_pZElauncher->IsCreateShop = false;
		return;
	}
	TCHAR* ������ʽ = new TCHAR[4096]();
	if (!�ж�������ʽ���ݿ��Ƿ����(������ʽ)) {
		MessageBox(NULL, _T("��ȡ������ʽ���ݿ�ʧ��,��ȷ�����ݿ��Ƿ�ɾ��!"), NULL, MB_OK | MB_TOPMOST);
		g_pZElauncher->IsCreateShop = false;
		return;
	}
	//��ȡ�̳���ҳ����
	TCHAR page[50] = { 0 };
	_stprintf(page, _T("%d"), m_PageNum);
	std::string Result = ��ȡƤ��html����(page);
	if (Result.length() < 10) {
		MessageBox(NULL, _T("��ȡ�̳�����ʧ��,��ȷ���Ƿ�ɷ���93.net!"), NULL, MB_OK | MB_TOPMOST);
		g_pZElauncher->IsCreateShop = false;
		return;
	}
	UINT nLen = Result.size() * sizeof(TCHAR);
	TCHAR* pStrHtml = new TCHAR[nLen]();
	_MultiByteToWideChar(CP_UTF8, NULL, Result.c_str(), Result.length(), pStrHtml, nLen);
	if (wcsstr(pStrHtml,L"��ʾ��Ϣ"))return;
#pragma region ������ʽ�㷨
	CContainerUI* pPage = static_cast<CContainerUI*>(m_paintmanager->FindControl(_T("page_panel")));
	if (pPage)pPage->SetVisible(true);
	VBScript_RegExp_55::IRegExp2Ptr pRegexp(__uuidof(VBScript_RegExp_55::RegExp));
	pRegexp->PutGlobal(VARIANT_TRUE);
	pRegexp->put_Multiline(VARIANT_FALSE);
	pRegexp->PutIgnoreCase(VARIANT_FALSE);
	pRegexp->PutPattern(������ʽ);
	VBScript_RegExp_55::IMatchCollectionPtr Item = pRegexp->Execute(pStrHtml);
	srand(GetTickCount());
	for (int i = 0; i < Item->GetCount(); i++) {
		if (i > 12)break;
		VBScript_RegExp_55::IMatch2Ptr Match = Item->GetItem(i);
		if(!Match)continue;
		VBScript_RegExp_55::ISubMatchesPtr Submatch = Match->GetSubMatches();
		if (!Submatch)continue;
		//��ʾƤ��
		_bstr_t sText = "";
		TCHAR szbuf[1024] = { 0 };
		_stprintf(szbuf, _T("panel_Skin%d"), i + 1);
		CContainerUI* pPanel = static_cast<CContainerUI*>(m_paintmanager->FindControl(szbuf));
		if (pPanel)pPanel->SetVisible(true);
		//��ȡƤ������
		sText = static_cast<_bstr_t>(Submatch->GetItem(1L));
		_stprintf(szbuf, _T("Text_SkinName%d"), i + 1);
		CTextUI* pText = static_cast<CTextUI*>(m_paintmanager->FindControl(szbuf));;
		if (pText) {
			pText->SetTextColor(RGB(rand() % 255, rand() % 255, rand() % 255));
			pText->SetText(sText);
		}
		//��ȡƤ��ͼƬ
		sText = static_cast<_bstr_t>(Submatch->GetItem(2L));
		_stprintf(szbuf, _T("Label_Skin%d"), i + 1);
		COptionUI* pLabel = static_cast<COptionUI*>(m_paintmanager->FindControl(szbuf));;
		if (pLabel) {
			//����Ƥ����������
			_bstr_t tmp23 = static_cast<_bstr_t>(Submatch->GetItem(0L));
			pLabel->SetUserData(tmp23);
			//����Ƥ��ͼƬ
			_stprintf(szbuf, _T("Temp\\%d.png"), i);
			std::string ImgData;
			if (sText.length() > 3)
			{
				lib_http::CLibhttp htp;
				std::wstring url;
				url = L"https://bbs.93x.net/";
				url += sText;
				htp.GET(url, ImgData);
				if (ImgData.size() > 20 && ImgData.find("404") == std::string::npos) {
					HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, ImgData.size());
					void* pData = GlobalLock(hGlobal);
					memcpy(pData, ImgData.c_str(), ImgData.size());
					GlobalUnlock(hGlobal);
					IStream* pStream = NULL;
					CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
					CImage img;
					img.Load(pStream);
					m_paintmanager->RemoveImage(szbuf);
					m_paintmanager->AddImage(szbuf, (HBITMAP)img, img.GetWidth(), img.GetHeight(), false);
					pLabel->SetBkImage(szbuf);
					img.Detach();
					pStream->Release();
					GlobalFree(hGlobal);
				}
				else {
					pLabel->SetBkImage(_T("skin\\nopic.jpg"));
				}
			}
			else {
				pLabel->SetBkImage(_T("skin\\nopic.jpg"));
			}

		}
		//��ȡ��Ǯ����
		sText = _T("{c #386382}�۸�:{/c}{c #FF0000}");
		sText += static_cast<_bstr_t>(Submatch->GetItem(3L));
		sText += _T("{/c}{c #386382}");
		sText += static_cast<_bstr_t>(Submatch->GetItem(4L));
		sText += _T("{/c}");
		_stprintf(szbuf, _T("Text_SkinMoney%d"), i + 1);
		pText = static_cast<CTextUI*>(m_paintmanager->FindControl(szbuf));
		if (pText)pText->SetText(sText);
		//��ȡVIP��Ǯ����
		sText = _T("{c #386382}VP��:{/c}{c #FF0000}");
		sText += static_cast<_bstr_t>(Submatch->GetItem(5L));
		sText += _T("{/c}{c #386382}");
		sText += static_cast<_bstr_t>(Submatch->GetItem(6L));
		sText += _T("{/c}");
		_stprintf(szbuf, _T("Text_SkinVip%d"), i + 1);
		pText = static_cast<CTextUI*>(m_paintmanager->FindControl(szbuf));
		if (pText)pText->SetText(sText);
	}
	pRegexp.Release();
	_stprintf(page, _T("%d/%d"), m_PageNum, m_pageNumMax);
	CTextUI* pPaget = static_cast<CTextUI*>(m_paintmanager->FindControl(_T("page_text")));
	if (pPaget)pPaget->SetText(page);
#pragma endregion
	{
		std::wstring shtml = pStrHtml;
		UINT Pos = shtml.find(L"<span title=\"��");
		if (Pos != std::wstring::npos) {
			m_pageNumMax = _wtoi(shtml.substr(Pos + wcslen(L"<span title=\"��"), shtml.find(L"ҳ\">", Pos) - Pos - wcslen(L"ҳ\">")).c_str());
		}
	}
	delete[]pStrHtml;
}

void C�̳�UI::������ȡ�̳����ݺ���()
{
	/*std::thread �߳̾�� = std::thread(&C�̳�UI::��ȡ�̳�����, this);
	�߳̾��.detach();*/
	��ȡ�̳�����();
}

void C�̳�UI::OnCreate()
{
	m_pageNumMax = 2;
	/*std::list<std::future<void>> lk;
	auto __p = std::async(std::launch::async, &C�̳�UI::GetSkininfo, this);
	lk.push_back(std::move(__p));*/
	//GetSkininfo();
	//HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ThreadCreate, this, NULL, NULL);
}

