#include "�鿴����.h"

CDuiString g_BBSUrl = L"";
CDuiString g_BBSTitle = L"";

bool IsThreadExit = false;

void C�鿴����UI::InitWindow()
{
	IsThreadExit = false;
	CTextUI* pText = static_cast<CTextUI*>(m_pm.FindControl(_T("Text_Out_")));
	if (pText) {
		CDuiString sText = _T("��ǰ����:  ");
		sText += g_BBSTitle;
		pText->SetText(sText);
	}
	CControlUI* pbkimg = static_cast<CControlUI*>(m_pm.FindControl(_T("img2_bkimage")));
	if (pbkimg) {
		_bstr_t CfgPath = GetCFGPath();
		TCHAR* pszbuff = new TCHAR[MAX_PATH]();
		GetPrivateProfileString(_T("ZElauncher"), _T("edit_bkimge"), NULL, pszbuff, (MAX_PATH * sizeof(TCHAR)), CfgPath);
		if (_tcslen(pszbuff) > 5)pbkimg->SetBkImage(pszbuff);
	}
	CProgressUI* pProgress = static_cast<CProgressUI*>(m_pm.FindControl(_T("Progress_BBS")));
	if (pProgress) {
		pProgress->SetValue(30);
		srand(time(NULL));
		pProgress->SetTextColor(RGB(rand() % 255, rand() % 255, rand() % 255));
	}
	��ʼ���ؼ�����();
	//��ȡ��������();
	std::thread t1 = std::thread(&C�鿴����UI::��ȡ��������, this);
	t1.detach();
	
}

void C�鿴����UI::OnExit()
{
	IsThreadExit = true;
	Sleep(500);
	__super::Close();
}

void C�鿴����UI::OnClick(CControlUI* pSender)
{
	if (_tcscmp(pSender->GetName(), _T("closebtn")) == 0)OnExit();
}

void C�鿴����UI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)::SetForegroundWindow(m_hWnd);//���ڴ�����Ϻ�,��ȡ����;
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);
	else if (_tcscmp(msg.sType, _T("setfocus")) == 0 && _tcscmp(msg.pSender->GetName(), _T("Get_fish")) == 0)Getfish();
}

void C�鿴����UI::Getfish() {
	lib_http::CLibhttp http;
	std::string HtmlData;
	UINT nFish = 0;
	for (auto fish : m_fishurl) {
		CDuiString url = L"https://bbs.93x.net/";
		url += fish;
		http.GET(url.GetData(), HtmlData,L"",gCookies.GetBSTR());
		HtmlData = DecodeToString(CP_UTF8, HtmlData);
		//_bstr_t temp = HtmlData.c_str();_bstr_t temp = HtmlData.c_str();
		if (HtmlData.find("���㻹��������") != std::string::npos)nFish++;
		
	}
	TCHAR szBuf[1024] = { 0 };
	_stprintf(szBuf, _T("ץ������,������������ => ������[ %d ]����"), nFish);
	//MessageBox(NULL, _T("ץ������,������������!"), NULL, MB_TOPMOST);
	MessageBox(NULL, szBuf, NULL, MB_TOPMOST);
	CLabelUI* pLabel = static_cast<CLabelUI*>(m_pm.FindControl(_T("Get_fish")));
	if (pLabel)pLabel->SetVisible(false);
}

CDuiString C�鿴����UI::GetImageStr(std::string& imgurl)
{
	lib_http::CLibhttp http;
	std::string HtmlData;
	http.GET(imgurl, HtmlData);
	if (HtmlData.find("404") != std::string::npos) {
		HtmlData = "";
		http.GET("https://bbs.93x.net/uc_server/images/noavatar_middle.gif", HtmlData);
	}
	if (HtmlData.size() < 10)return _T("");
	if (HtmlData.find("404") != std::string::npos)return _T("");
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, HtmlData.size());
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, HtmlData.c_str(), HtmlData.size());
	GlobalUnlock(hGlobal);
	IStream* pStream = NULL;
	CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
	CImage img;
	img.Load(pStream);
	char* pSearch = (char*)imgurl.c_str();
	char* pChar = strrchr(pSearch, '/');
	TCHAR szbuf[MAX_PATH] = { 0 };
	srand(time(NULL));
	if (pChar) {
		for (UINT i = 0; i < 3; i++) {
			pChar[0] = '_';
			pChar = strrchr(pSearch, '/');
		}
		pChar++;
		_bstr_t temp = pChar;
		_tcscpy(szbuf, temp);
	}
	else _stprintf(szbuf, _T("BBS_image_%d.png"), rand() % 255);
	m_pm.RemoveImage(szbuf);
	m_pm.AddImage(szbuf, (HBITMAP)img, img.GetWidth(), img.GetHeight(), false);
	img.Detach();
	pStream->Release();
	GlobalFree(hGlobal);
	return szbuf;

}

void C�鿴����UI::��ȡ��������()
{
	lib_http::CLibhttp http;
	std::string htmldata;
	http.GET(g_BBSUrl.GetData(), htmldata, L"", gCookies.GetBSTR());
	if (htmldata.length() < 10)return;
	std::string pStrMulti = DecodeToString(CP_UTF8, htmldata);
	UINT nFloor = 0;
	����������Ϣ(pStrMulti, &nFloor);

	std::string shtml = std::move(pStrMulti);
	UINT Pos = shtml.find("<span title=\"��"), BBS_pageNumMax = 0;
	if (Pos != std::string::npos) {
		BBS_pageNumMax = atoi(shtml.substr(Pos + strlen("<span title=\"��"), shtml.find("ҳ\">", Pos) - Pos - strlen("ҳ\">")).c_str());
	}
	for (int i = 1; i < BBS_pageNumMax; i++) {
		TCHAR szbuf[60] = { 0 };
		_stprintf(szbuf, _T("&page=%d"), i + 1);
		CDuiString url = g_BBSUrl;
		url += szbuf;
		http.GET(url.GetData(), htmldata, L"", gCookies.GetBSTR());
		if (htmldata.length() < 10)return;
		pStrMulti = DecodeToString(CP_UTF8, htmldata);
		����������Ϣ(pStrMulti, &nFloor);
	}
	if (IsThreadExit)return;
	srand(time(NULL));
	m_pProgress->SetTextColor(RGB(rand() % 255, rand() % 255, rand() % 255));
	m_pProgress->SetValue(100);
	m_pProgress->SetText(_T("�������ݻ�ȡ���!"));
}

void C�鿴����UI::����������Ϣ(std::string& htmldata, UINT* nFloor) {
	UINT nValue = 29;
	HtmlParser parser;
	shared_ptr<HtmlDocument> doc = parser.Parse(htmldata.c_str(), htmldata.size());
	std::vector<shared_ptr<HtmlElement>> plhin = std::move(doc->GetElementByClassName("plhin"));
	//std::vector<shared_ptr<HtmlElement>> tf = std::move(doc->GetElementByClassName("t_f"));
	for (auto authi : plhin) {
		if (IsThreadExit)return;
		_bstr_t ree = authi->html().c_str();
		
		std::vector<shared_ptr<HtmlElement>> xw1 = std::move(authi->GetElementByClassName("authi"));
		if (xw1.empty())continue;
		std::vector<shared_ptr<HtmlElement>> nicname = std::move(xw1[0]->GetElementByClassName("xw1"));
		if (nicname.empty())continue;
		std::vector<shared_ptr<HtmlElement>> mz = std::move(authi->GetElementByClassName("avtm"));
		if (mz.empty())continue;

		CDialogBuilder BuilderXml;
		CControlUI* pNewXml = BuilderXml.Create(_T("xml\\BBS_Nick.xml"), NULL, NULL, &m_pTilelayout);
		if (!pNewXml)continue;
		m_BBSList->Add(pNewXml);

		CLabelUI* pLabel = static_cast<CLabelUI*>(m_pTilelayout.FindSubControlByName(pNewXml, _T("Image")));
		if (pLabel) {
			std::string srcstr = std::move(mz[0]->html());
			int nPos = srcstr.find("src=\"");
			if (nPos != std::string::npos) {
				UINT offset = nPos + strlen("src=\"");
				std::string psrc = srcstr.substr(offset, srcstr.find("\"", offset) - offset);
				pLabel->SetBkImage(GetImageStr(psrc));
			}
		}
		CTextUI* pText = static_cast<CTextUI*>(m_pTilelayout.FindSubControlByName(pNewXml, _T("Text_nickname")));
		if (pText) {
			_bstr_t sText = nicname[0]->text().c_str();
			pText->SetText(sText);
		}
		(*nFloor)++;
		pText = static_cast<CTextUI*>(m_pTilelayout.FindSubControlByName(pNewXml, _T("Text_Memony")));
		if (pText) {
			if (*nFloor == 1)pText->SetText(_T("¥��"));
			else {
				TCHAR szBuf[50] = { 0 };
				_stprintf(szBuf, _T("�� %d ¥"), *nFloor);
				pText->SetText(szBuf);
			}
		}

		srand(time(NULL));
		std::vector<shared_ptr<HtmlElement>> tf = std::move(authi->GetElementByClassName("t_f"));
		if (tf.empty())continue;
		pText = static_cast<CTextUI*>(m_pTilelayout.FindSubControlByName(pNewXml, _T("Text_BBS")));
		if (pText) {
			std::string htmltext;
			tf[0]->PlainStylize2(htmltext);
			_bstr_t tfText = htmltext.c_str();
			CDuiString tfformat = tfText.GetBSTR();
			tfformat.Replace(_T("&nbsp;"), _T(" "));
			tfformat.Replace(_T("\n\n"), _T(""));
			//tfText = tf[0]->html().c_str();
			pText->SetTextColor(RGB(rand() % 255, rand() % 255, rand() % 255));
			pText->SetText(tfformat);
		}
		/*pText = static_cast<CTextUI*>(m_pTilelayout.FindSubControlByName(pNewXml, _T("Text_BBS")));
		if (pText && tf.size() >= plhin.size()) {
			std::string htmltext;
			tf[i]->PlainStylize2(htmltext);
			_bstr_t tfText = htmltext.c_str();
			CDuiString tfformat = tfText.GetBSTR();
			tfformat.Replace(_T("&nbsp;"), _T(" "));
			tfformat.Replace(_T("\n\n"), _T(""));
			//tfText = tf[0]->html().c_str();
			pText->SetTextColor(RGB(rand() % 255, rand() % 255, rand() % 255));
			pText->SetText(tfformat);
		}*/

		m_pProgress->SetTextColor(RGB(rand() % 255, rand() % 255, rand() % 255));
		m_pProgress->SetValue(nValue);
	}
	size_t nPoss = htmldata.find("����׽������");
	if (nPoss != std::string::npos) {
		_bstr_t url = htmldata.substr(nPoss, htmldata.find("<img src=\"source/plugin/taobo_fishing") - nPoss).c_str();
		CDuiString sText = url.GetBSTR();
		nPoss = sText.Find(_T("','")) + 3;
		int rp = sText.Find(_T("','get'"), nPoss) - nPoss;
		sText = sText.Mid(nPoss, rp);
		OutputDebugString(sText);
		OutputDebugStringA("\r\n");
		m_fishurl.emplace_back(sText);
		CLabelUI* pLabel = static_cast<CLabelUI*>(m_pm.FindControl(_T("Get_fish")));
		if (pLabel)pLabel->SetVisible(true);
	}
}


LPCTSTR C�鿴����UI::GetWindowClassName()const
{
	return _T("�鿴����");
}

CDuiString C�鿴����UI::GetSkinFile()
{
	return _T("BBS_look.xml");
}

CDuiString C�鿴����UI::GetSkinFolder()
{
	return _T("Skin\\ListRes\\");
}

UILIB_RESTYPE C�鿴����UI::GetResourceType()const
{
	return UILIB_FILE;
}

LPCTSTR C�鿴����UI::GetResourceID()const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void C�鿴����UI::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

CControlUI* C�鿴����UI::CreateControl(LPCTSTR pstrClass)
{
	return nullptr;
}

void C�鿴����UI::��ʼ���ؼ�����() {
	m_BBSList = static_cast<CListUI*>(m_pm.FindControl(_T("Table_LookBBS")));
	if (!m_BBSList)goto erron_;
	m_pProgress = static_cast<CProgressUI*>(m_pm.FindControl(_T("Progress_BBS")));
	if (!m_pProgress)goto erron_;

	return;

erron_:
	MessageBox(NULL, _T("�ؼ���ʼ��ʧ��!"), NULL, MB_ICONERROR | MB_TOPMOST);
	IsThreadExit = true;
	Sleep(500);
	__super::Close();
}