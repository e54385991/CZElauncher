#include "��̳.h"

std::string g_BBSListUrl = "";

C��̳UI::~C��̳UI()
{
}

C��̳UI::C��̳UI(CPaintManagerUI& m_paintmanager)
{
	m_paintmanager_ = &m_paintmanager;
	CContainerUI* pBBS = static_cast<CContainerUI*>(builder.Create(_T("BBS.xml"), NULL, NULL, &m_PaintManager2));
	if (pBBS) {
		this->Add(pBBS);
		m_paintmanager_->AddNotifier(this);
	}
	else {
		this->RemoveAll();
		return;
	}
}

void C��̳UI::OnClick(CControlUI* Click)
{
	if (_tcscmp(Click->GetName(), _T("bbs_ze")) == 0)GetBBSList("https://bbs.93x.net/forum.php?mod=forumdisplay&fid=38");
	else if (_tcscmp(Click->GetName(), _T("bbs_discuss")) == 0)GetBBSList("https://bbs.93x.net/forum.php?mod=forumdisplay&fid=39");
	else if (_tcscmp(Click->GetName(), _T("bbs_ttt")) == 0)GetBBSList("https://bbs.93x.net/forum.php?mod=forumdisplay&fid=114");
	else if (_tcscmp(Click->GetName(), _T("bbs_Skin")) == 0)GetBBSList("https://bbs.93x.net/forum.php?mod=forumdisplay&fid=279");
}

void C��̳UI::MenuClick(CControlUI* Click)
{
	if(_tcscmp(Click->GetText(),_T("ʹ��������鿴����"))==0)BrowserBBS();
	else if (_tcscmp(Click->GetText(), _T("ˢ������")) == 0)ShowBBSList();
	else if (_tcscmp(Click->GetText(), _T("�鿴����")) == 0)OnLookBBS();
	else if (_tcscmp(Click->GetText(), _T("�л���ʾ�ö�����")) == 0) {
		��ֹ�ö�����ʾ = !��ֹ�ö�����ʾ;
		ShowBBSList();
	}
}

void C��̳UI::BrowserBBS()
{
	CListUI* plist = static_cast<CListUI*>(m_paintmanager_->FindControl(_T("List_bbs")));
	if (!plist)return;
	CListTextElementUI* pItem = static_cast<CListTextElementUI*>(plist->GetItemAt(plist->GetCurSel()));
	if (!pItem)return;
	CDuiString url = pItem->GetUserData();
	url.Replace(_T("amp;"), _T(""));
	ShellExecute(NULL, _T(""), url, NULL, NULL, SW_SHOW);
}

void C��̳UI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)OnCreate();
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);
	else if (_tcscmp(msg.sType, _T("itemclick")) == 0)MenuClick(msg.pSender);
	else if (_tcscmp(msg.sType, _T("menu")) == 0 && _tcscmp(msg.pSender->GetName(), _T("List_bbs")) == 0) {
		CDuiPoint point(0, 0);
		GetCursorPos(&point);
		STRINGorID xml(_T("Menu_BBS.xml"));
		CMenuWnd* pMenu = CMenuWnd::CreateMenu(nullptr, xml, point, m_paintmanager_);

	}
	//else if (_tcscmp(msg.sType, _T("itemactivate")) == 0)OnLookBBS();
}

void C��̳UI::OnCreate()
{
	CListUI* plistbkimg = static_cast<CListUI*>(m_paintmanager_->FindControl(_T("List_bbs")));
	if (plistbkimg) {
		_bstr_t cfgbuff = GetCFGPath();
		TCHAR* pszbuff = new TCHAR[MAX_PATH]();
		GetPrivateProfileString(_T("ZElauncher"), _T("edit_listbkimge"), NULL, pszbuff, (MAX_PATH * sizeof(TCHAR)), cfgbuff);
		if (_tcslen(pszbuff) > 5) {
			plistbkimg->SetItemBkColor(RGB(0xFF, 0xE7, 0xE7));
			plistbkimg->SetBkImage(pszbuff);
		}
	}
	std::thread t1 = std::thread(&C��̳UI::GetBBSList, this, (_bstr_t)"https://bbs.93x.net/forum.php?mod=forumdisplay&fid=38", NULL);
	t1.detach();
	std::thread t2 = std::thread(&C��̳UI::Threadlaba, this);
	t2.detach();
}

void C��̳UI::Threadlaba()
{
	for (;;) {
		if (!�Ƿ����ڻ�ȡ������Ϣ) {
			CTextUI* pText = static_cast<CTextUI*>(m_paintmanager_->FindControl(_T("bbs_say")));
			if (!pText || ������Ϣ.size() < 1)continue;
			_bstr_t sText = ������Ϣ[m_SaynCount].c_str();
			m_SaynCount++;
			if (m_SaynCount >= ������Ϣ.size())m_SaynCount = 0;
			pText->SetText(sText);
		}
		Sleep(3000);
	}
}

void C��̳UI::ShowBBSList()
{
	std::thread t1 = std::thread(&C��̳UI::GetBBSList, this, (_bstr_t)g_BBSListUrl.c_str(), NULL);
	t1.detach();
}

std::vector<shared_ptr<HtmlElement>> C��̳UI::GetTagValue(const char* name, const char* tagname, shared_ptr<HtmlElement>& tbody)
{
	std::vector<shared_ptr<HtmlElement>> tagvalue = tbody->GetElementByClassName(name);
	std::vector<shared_ptr<HtmlElement>> Result;
	if (tagvalue.size() < 1)return Result;
	return tagvalue[0]->GetElementByClassName(tagname);
}

void C��̳UI::OnLookBBS()
{
	/*MessageBox(NULL, L"�ܱ�Ǹ����ܻ�û�����,�����ĵȴ�.", L"Tip", MB_ICONQUESTION);
	return;*/
	CListUI* pList = static_cast<CListUI*>(m_paintmanager_->FindControl(_T("List_bbs")));
	if (!pList)return;
	CListTextElementUI* pItem = static_cast<CListTextElementUI*>(pList->GetItemAt(pList->GetCurSel()));
	if (!pItem) {
		MessageBox(NULL, L"��ѡ��һ��������ʹ�øù���!", NULL, NULL);
		return;
	}
	CDuiString ItemText = pItem->GetUserData();
	ItemText.Replace(_T("amp;"), _T(""));
	g_BBSUrl = ItemText;
	g_BBSTitle = pItem->GetText(1);
	C�鿴����UI* pSetting = new C�鿴����UI(m_paintmanager_);
	if (pSetting == NULL) return;
	pSetting->Create(NULL, _T("�鿴����"), UI_WNDSTYLE_DIALOG, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 900, 500);
	pSetting->CenterWindow();
	pSetting->ShowModal();
	//::ShowWindow(*pSetting, SW_SHOW);
	//::SetWindowPos(*pSetting, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

bool IsThread3 = false;

void C��̳UI::GetBBSList(_bstr_t url, UINT type /* = NULL */)
{
	if (IsThread3)return;
	IsThread3 = true;
	g_BBSListUrl = url;
	CListUI* pList = static_cast<CListUI*>(m_paintmanager_->FindControl(_T("List_bbs")));
	if (!pList) { IsThread3 = false; return; }
	pList->RemoveAll();
	������Ϣ.clear();
	lib_http::CLibhttp http;
	std::string htmldata;
	http.GET(url.GetBSTR(), htmldata, L"", gCookies.GetBSTR());
	if (htmldata.length() < 10) { IsThread3 = false; return; }
	//����ת��
	std::string htmlDecode = DecodeToString(CP_UTF8, htmldata);
	if (strstr(htmlDecode.c_str(), "�����û���˽����,��������Ҫ350������Ϸʱ�䷽�ɽ���")) {
		MessageBox(NULL, L"�����û���˽����,��������Ҫ350������Ϸʱ�䷽�ɽ���!", NULL, MB_ICONWARNING);
		IsThread3 = false;
		return;
	}
	HtmlParser parser;
	shared_ptr<HtmlDocument> doc = parser.Parse(htmlDecode.c_str(), htmlDecode.size());
	//�ͷŻ�����;
	/*delete[]pStrMulti;
	delete[]pStrHtml*/;
	//html����
	CBBSUI::���� Tmp_ = { "" };
	_bstr_t tmpText = "";

	GetSaylaba(doc);
	//���������б��ȡ����
	shared_ptr<HtmlElement> BBSlist = std::move(doc->GetElementById("threadlisttableid"));
	if (!BBSlist) {
		MessageBox(NULL, L"��ȡ����ʧ��,������!", NULL, NULL);
		IsThread3 = false;
		return;
	}
	std::vector<shared_ptr<HtmlElement>> threadlisttableid = std::move(BBSlist->GetElementByTagName("tbody"));
	const char Element_name[3][10] = { "common","lock","new" };
	const char Element_type[3][15] = { "<�ö���>","<������>","<δ����>" };
	for (auto tbody : threadlisttableid) {
		Tmp_ = { "" };
		//��ȡ�������ӱ���
		for (UINT j = 0; j < 3; j++) {
			//��ȡ����Element;
			std::vector<shared_ptr<HtmlElement>> Element = std::move(tbody->GetElementByClassName(Element_name[j]));
			if (Element.size() < 1)continue;
			if (strstr(tbody->html().c_str(), "�ö�����"))tmpText = Element_type[j];
			else {
				//��ȡ������������;
				std::vector<shared_ptr<HtmlElement>> eletype = Element[0]->GetElementByTagName("em");
				if (eletype.size() > 0) {
					tmpText = eletype[0]->text().c_str();
				}
				else tmpText = Element_type[2];
			}
			//std::string htmltitle = tbody->html();
			Tmp_.typename_ = tmpText;
			//��ȡ�������ƺ�����;
			std::vector<shared_ptr<HtmlElement>> sxst = std::move(Element[0]->GetElementByClassName("s xst"));
			if (sxst.size() < 1)continue;
			Tmp_.������ = sxst[0]->text().c_str();
			Tmp_.hrefurl = sxst[0]->GetAttribute("href").c_str();
			//�ж��Ƿ��к��;
			std::vector<shared_ptr<HtmlElement>> award = std::move(Element[0]->GetElementByTagName("font"));
			if (award.size() > 1) {
				const char* pChar = award[1]->GetValue().c_str();
				if (strstr(pChar, "���")) {
					Tmp_.�Ƿ��к�� = true;
					Tmp_.������� = pChar;
				}
			}
			Tmp_.type = 0x2F + j;
			break;
		}
		//��ȡ����ظ��Ͳ鿴��
		std::vector<shared_ptr<HtmlElement>> num = std::move(tbody->GetElementByClassName("num"));
		if (num.size() > 0) {
			std::vector<shared_ptr<HtmlElement>> sxst = std::move(num[0]->GetElementByClassName("xi2"));
			if (sxst.size() > 0) {
				tmpText = sxst[0]->text().c_str();
				Tmp_.�ظ��� = std::move(tmpText);
				tmpText = num[0]->text().c_str();
				Tmp_.�鿴�� = std::move(tmpText);
			}
		}
		if (Tmp_.������.length() > 1)������Ϣ.emplace_back(Tmp_);
	}

	srand((UINT)time(NULL));
	for (auto ListBBS : ������Ϣ) {
		if (��ֹ�ö�����ʾ) {
			if (strstr(ListBBS.typename_.c_str(), Element_type[0]) || strstr(ListBBS.typename_.c_str(), Element_type[1]))continue;
		}
		CListTextElementUI* pItemText = new CListTextElementUI();
		pItemText->SetTag(pItemText->GetTag() + 1);
		pList->Add(pItemText);
		_bstr_t sText = ListBBS.typename_.c_str();
		pItemText->SetText(0, sText);

		sText = ListBBS.������.c_str();
		pItemText->SetText(1, sText);

		sText = ListBBS.�������.c_str();
		pItemText->SetText(2, sText);

		sText = ListBBS.�ظ���.c_str();
		sText += "/";
		sText += ListBBS.�鿴��.c_str();
		pItemText->SetText(3, sText);

		sText = ListBBS.hrefurl.c_str();
		pItemText->SetUserData(sText.GetBSTR());

		//if (ListBBS.type > 1)pItemText->SetTextColor_(RGB(rand() % 255, rand() % 255, rand() % 255));
		if (strstr(ListBBS.typename_.c_str(), Element_type[0]) || strstr(ListBBS.typename_.c_str(), Element_type[1]))
			pItemText->SetTextColor_(RGB(rand() % 255, rand() % 255, rand() % 255));
	}
	IsThread3 = false;
}

void C��̳UI::GetSaylaba(shared_ptr<HtmlDocument>& doc)
{
	�Ƿ����ڻ�ȡ������Ϣ = true;
	������Ϣ.clear();
	m_SaynCount = 0;
	shared_ptr<HtmlElement> laba = std::move(doc->GetElementById("laba"));
	std::string tmptext;
	std::vector<shared_ptr<HtmlElement>> tbodyroot = std::move(doc->SelectElement("//tr[@style]"));
	if (tbodyroot.size() < 1)return;
	std::vector<shared_ptr<HtmlElement>> tbodyroot_;
	tbodyroot[0]->SelectElement("//div[@style]", tbodyroot_);
	tmptext = std::move(tbodyroot[0]->html());
	_bstr_t Tmpstr = tbodyroot[0]->text().c_str();
	CDuiString strin = Tmpstr.GetBSTR();
	strin.Replace(L"\t\n\t", L"  ");
	strin.Replace(L"\t", L"  ");
	int npos = -1;
	do {
		Tmpstr = strin.Mid(npos + 1, strin.Find(L"\n", npos + 1) - (npos + 1));
		npos = strin.Find(L"\n", npos + 1);
		tmptext = Tmpstr;
		if (tmptext.length() > 2 && tmptext.find("Сʱǰ") == std::string::npos && tmptext.find("��ǰ") == std::string::npos)������Ϣ.emplace_back(tmptext);
	} while (npos != -1);
	�Ƿ����ڻ�ȡ������Ϣ = false;
}

