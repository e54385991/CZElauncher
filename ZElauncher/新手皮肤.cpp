#include "新手皮肤.h"

C新手皮肤UI::C新手皮肤UI(CPaintManagerUI& m_paintmanage)
{
	m_paintmanage_ = &m_paintmanage;
	m_PageNum = 1;
	CDialogBuilder Builder;
	pSKin = static_cast<CContainerUI*>(Builder.Create(_T("SkinConvert.xml"), NULL, NULL, &m_pm2));
	if (pSKin) {
		this->Add(pSKin);
		m_paintmanage_->AddNotifier(this);
	}
	else {
		this->RemoveAll();
		return;
	}
}

void C新手皮肤UI::兑换皮肤(LPCTSTR pUrl)
{
	if (!g_pZElauncher->UserIslogin()) {
		MessageBox(NULL, _T("您没有登录,请登录后在进行该操作!"), NULL, MB_OK | MB_ICONERROR | MB_TOPMOST);
		return;
	}
	//获取正则表达式数据
	_bstr_t dbPath = GetRunPath();
	dbPath += _T("\\bin\\Regex.db");
	//申请表达式缓存空间
	TCHAR* pTpatternSkinInfoTip = new TCHAR[4096];
	TCHAR* pTpatternSkinInfoTip2 = new TCHAR[4096];
	TCHAR* pTpatternSkinConvert = new TCHAR[4096];
	ZeroMemory(pTpatternSkinInfoTip, 4096);
	ZeroMemory(pTpatternSkinInfoTip2, 4096);
	ZeroMemory(pTpatternSkinConvert, 4096);
	//读取表达式数据
	GetPrivateProfileString(_T("ZElauncher"), _T("SkinConvertTip"), NULL, pTpatternSkinInfoTip, 4096, dbPath);
	if (_tcslen(pTpatternSkinInfoTip) < 5) { MessageBox(NULL, _T("字符数据库丢失!无法获取新手礼包数据!"), NULL, MB_OK); delete[]pTpatternSkinConvert; delete[]pTpatternSkinInfoTip; delete[]pTpatternSkinInfoTip2; return; }
	GetPrivateProfileString(_T("ZElauncher"), _T("SkinConvertUrl"), NULL, pTpatternSkinConvert, 4096, dbPath);
	if (_tcslen(pTpatternSkinConvert) < 5) { MessageBox(NULL, _T("字符数据库丢失!无法获取新手礼包数据!"), NULL, MB_OK); delete[]pTpatternSkinConvert; delete[]pTpatternSkinInfoTip; delete[]pTpatternSkinInfoTip2; return; }
	GetPrivateProfileString(_T("ZElauncher"), _T("SkinConvertTip2"), NULL, pTpatternSkinInfoTip2, 4096, dbPath);
	if (_tcslen(pTpatternSkinInfoTip2) < 5) { MessageBox(NULL, _T("字符数据库丢失!无法获取新手礼包数据!"), NULL, MB_OK); delete[]pTpatternSkinConvert; delete[]pTpatternSkinInfoTip; delete[]pTpatternSkinInfoTip2; return; }
	std::string htmldata;
	lib_http::CLibhttp http;
	CDuiString Cookies = gCookies.GetBSTR();
	http.GET(pUrl, htmldata, _T(""), Cookies.GetData());
	UINT nLen = htmldata.size() * sizeof(TCHAR);
	TCHAR* pStrHtml = new TCHAR[nLen];
	MultiByteToWideChar(CP_UTF8, NULL, htmldata.c_str(), htmldata.length(), pStrHtml, nLen);
#pragma region 正在表达式算法
	VBScript_RegExp_55::IRegExp2Ptr pRegexp(__uuidof(VBScript_RegExp_55::RegExp));
	pRegexp->PutGlobal(VARIANT_TRUE);
	pRegexp->put_Multiline(VARIANT_FALSE);
	pRegexp->PutIgnoreCase(VARIANT_FALSE);
	pRegexp->PutPattern(pTpatternSkinInfoTip);
	VBScript_RegExp_55::IMatchCollectionPtr ItemTip = pRegexp->Execute(pStrHtml);
	pRegexp->PutPattern(pTpatternSkinInfoTip2);
	VBScript_RegExp_55::IMatchCollectionPtr ItemTip2 = pRegexp->Execute(pStrHtml);
	pRegexp->PutPattern(pTpatternSkinConvert);
	VBScript_RegExp_55::IMatchCollectionPtr ItemUrl = pRegexp->Execute(pStrHtml);
	_bstr_t sResultTip = _T("");
	_bstr_t sSkinConvertUrl = _T("");
	if (ItemTip->GetCount() > 0) {
		VBScript_RegExp_55::IMatch2Ptr Match = ItemTip->GetItem(0L);
		if (!Match) { MessageBox(NULL, _T("皮肤兑换失败,获取数据失败!"), NULL, MB_OK); goto Exit; }
		VBScript_RegExp_55::ISubMatchesPtr Submatch = Match->GetSubMatches();
		if (!Submatch) { MessageBox(NULL, _T("皮肤兑换失败,获取数据失败!"), NULL, MB_OK); goto Exit; }
		sResultTip += _T("\t确定兑换该皮肤吗?\r\n\r\n获取途径:") + static_cast<_bstr_t>(Submatch->GetItem(0L)) + _T("\r\n");
	}
	if (ItemTip2->GetCount() > 0) {
		VBScript_RegExp_55::IMatch2Ptr Match = ItemTip2->GetItem(0L);
		if (!Match) { MessageBox(NULL, _T("皮肤兑换失败,获取数据失败!"), NULL, MB_OK); goto Exit; }
		VBScript_RegExp_55::ISubMatchesPtr Submatch = Match->GetSubMatches();
		if (!Submatch) { MessageBox(NULL, _T("皮肤兑换失败,获取数据失败!"), NULL, MB_OK); goto Exit; }
		sResultTip += _T("获取途径说明:") + static_cast<_bstr_t>(Submatch->GetItem(0L)) + _T("\r\n");
	}
	if (ItemUrl->GetCount() > 0) {
		VBScript_RegExp_55::IMatch2Ptr Match = ItemUrl->GetItem(0L);
		if (!Match) { MessageBox(NULL, _T("皮肤兑换失败,获取数据失败!"), NULL, MB_OK); goto Exit; }
		VBScript_RegExp_55::ISubMatchesPtr Submatch = Match->GetSubMatches();
		if (!Submatch) { MessageBox(NULL, _T("皮肤兑换失败,获取数据失败!"), NULL, MB_OK); goto Exit; }
		sSkinConvertUrl = _T("https://bbs.93x.net/") + static_cast<_bstr_t>(Submatch->GetItem(0L));
		sResultTip += static_cast<_bstr_t>(Submatch->GetItem(1L)) + _T("\r\n");
	}
	else {
		MessageBox(NULL, _T("皮肤兑换失败,获取数据失败!"), NULL, MB_OK);
		return;
	}
	if (MessageBox(NULL, sResultTip, _T("Tip:"), MB_YESNO) == IDYES) {
		ZeroMemory(pTpatternSkinConvert, 4096);
		//读取表达式数据
		GetPrivateProfileString(_T("ZElauncher"), _T("SkinConvertResult"), NULL, pTpatternSkinConvert, 4096, dbPath);
		if (_tcslen(pTpatternSkinConvert) < 5) { MessageBox(NULL, _T("字符数据库丢失!无法获取新手礼包数据!"), NULL, MB_OK); goto Exit; }
		//开始兑换皮肤
		htmldata = "";
		http.GET(static_cast<TCHAR*>(sSkinConvertUrl), htmldata, _T(""), Cookies.GetData());
		//编码转换
		nLen = htmldata.size() * sizeof(TCHAR);
		TCHAR* pTmpHtml = new TCHAR[nLen]();
		MultiByteToWideChar(CP_UTF8, NULL, htmldata.c_str(), htmldata.length(), pTmpHtml, nLen);
		pRegexp->PutPattern(pTpatternSkinConvert);
		VBScript_RegExp_55::IMatchCollectionPtr ItemResult = pRegexp->Execute(pTmpHtml);
		delete[]pTmpHtml;
		CDuiString sTip = _T("");
		if (ItemResult->GetCount() > 0) {
			VBScript_RegExp_55::IMatch2Ptr Match = ItemResult->GetItem(0L);
			if (!Match) { MessageBox(NULL, _T("获取返回数据失败!"), NULL, MB_OK); goto Exit2; }
			VBScript_RegExp_55::ISubMatchesPtr Submatch = Match->GetSubMatches();
			if (!Submatch) { MessageBox(NULL, _T("获取返回数据失败!"), NULL, MB_OK); goto Exit2; }
			_bstr_t tmpbstr = static_cast<_bstr_t>(Submatch->GetItem(0L));
			sTip = static_cast<TCHAR*>(tmpbstr);
			sTip.Replace(_T("<br>"), _T("\r\n"));
			sTip.Replace(_T("</p>"), _T(""));
		}
		Exit2:
		MessageBox(NULL, sTip, _T("Tip:"), MB_OK);

	}
#pragma endregion
	Exit:
	delete[]pStrHtml;
	delete[]pTpatternSkinConvert; 
	delete[]pTpatternSkinInfoTip;
	delete[]pTpatternSkinInfoTip2;
	return;
}

void C新手皮肤UI::OnClick(CControlUI* Click)
{
	if (_tcsstr(Click->GetName(), _T("Button_SkinConvert")))兑换皮肤(Click->GetUserData());
}

void C新手皮肤UI::Notify(TNotifyUI& msg)
{

	if (_tcscmp(msg.sType, _T("windowinit")) == 0)OnCreate();
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);
}

std::string C新手皮肤UI::获取皮肤html数据(const TCHAR* Page)
{
	std::string Result;
	{
		lib_http::CLibhttp http;
		CDuiString url = _T("https://bbs.93x.net/plugin-xnet_steam_store_client_items-csgo_skinlist.html?FreeExperience=");
		url += Page;
		CDuiString Cookies = gCookies.GetBSTR();
		http.GET(url.GetData(), Result, _T(""), Cookies.GetData());
	}
	return Result;
}

CDuiString C新手皮肤UI::获取图片地址(LPCTSTR pUrl, UINT iFlag)
{
	lib_http::CLibhttp http;
	std::string Htmldata;
	http.GET(pUrl, Htmldata);
	if (Htmldata.length() < 10)return _T("skin\\nopic.jpg");
	if (Htmldata.find("404") != std::string::npos)return _T("skin\\nopic.jpg");
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, Htmldata.size());
	void* pData = GlobalLock(hGlobal);
	memcpy(pData, Htmldata.c_str(), Htmldata.size());
	GlobalUnlock(hGlobal);
	IStream* pStream = NULL;
	CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
	CImage img;
	img.Load(pStream);
	TCHAR szbuf[MAX_PATH] = { 0 };
	_stprintf(szbuf, _T("Skin_%d.png"), iFlag);
	m_paintmanage_->RemoveImage(szbuf);
	m_paintmanage_->AddImage(szbuf, (HBITMAP)img, img.GetWidth(), img.GetHeight(), false);
	img.Detach();
	pStream->Release();
	GlobalFree(hGlobal);
	return szbuf;
}

void C新手皮肤UI::获取新手皮肤数据()
{
	/*CButtonUI* pLogin = static_cast<CButtonUI*>(m_paintmanager->FindControl(_T("Button_Login")));
	if (!pLogin)return;
	if (_tcscmp(pLogin->GetText(), _T("{u}{a}未登录{/a}{/u}")) == 0 || _tcscmp(pLogin->GetText(), _T("{u}{a}登录失败{/a}{/u}")) == 0 || _tcscmp(pLogin->GetText(), _T("{u}{a}Cookies获取为空{/a}{/u}")) == 0) {
		MessageBox(NULL, _T("获取新手皮肤礼包失败,请先登录!"), NULL, NULL);
		return;
	}*/
	if (!g_pZElauncher->UserIslogin()) {
		MessageBox(NULL, _T("获取新手皮肤礼包失败,请先登录!"), NULL, NULL);
		return;
	}
	CTileLayoutUI* pTilelayout = static_cast<CTileLayoutUI*>(m_paintmanage_->FindControl(_T("Table_SkinConvert")));
	if (!pTilelayout)return;
	//获取正则表达式数据
	_bstr_t dbPath = GetRunPath();
	dbPath += _T("\\bin\\Regex.db");
	//申请表达式缓存空间
	TCHAR* pTpatternSkin = new TCHAR[4096];
	TCHAR* pTpatternImg = new TCHAR[4096];
	TCHAR* pTpatternName = new TCHAR[4096];
	ZeroMemory(pTpatternImg, 4096);
	ZeroMemory(pTpatternSkin, 4096);
	ZeroMemory(pTpatternName, 4096);
	//读取表达式数据
	GetPrivateProfileString(_T("ZElauncher"), _T("SkinConvert"), NULL, pTpatternSkin, 4096, dbPath);
	if (_tcslen(pTpatternSkin) < 5) { MessageBox(NULL, _T("字符数据库丢失!无法获取新手礼包数据!"), NULL, MB_OK); g_pZElauncher->IsCreateSkin = false; delete[]pTpatternSkin; delete[]pTpatternImg; delete[]pTpatternName; return; }
	GetPrivateProfileString(_T("ZElauncher"), _T("SkinConvertImg"), NULL, pTpatternImg, 4096, dbPath);
	if (_tcslen(pTpatternImg) < 5) { MessageBox(NULL, _T("字符数据库丢失!无法获取新手礼包数据!"), NULL, MB_OK); g_pZElauncher->IsCreateSkin = false; delete[]pTpatternSkin; delete[]pTpatternImg; delete[]pTpatternName; return; }
	GetPrivateProfileString(_T("ZElauncher"), _T("SkinConvertName"), NULL, pTpatternName, 4096, dbPath);
	if (_tcslen(pTpatternName) < 5) { MessageBox(NULL, _T("字符数据库丢失!无法获取新手礼包数据!"), NULL, MB_OK); g_pZElauncher->IsCreateSkin = false; delete[]pTpatternSkin; delete[]pTpatternImg; delete[]pTpatternName; return; }
	//获取新手礼包数据
	std::string Result = 获取皮肤html数据(_T("1"));
	if (Result.size() < 10) { MessageBox(NULL, _T("获取新手礼包数据失败,请确认是否可访问[bbs.93x.net]!"), NULL, MB_OK); g_pZElauncher->IsCreateSkin = false; delete[]pTpatternSkin; delete[]pTpatternImg; return; }
	//编码转换
	UINT nLen = Result.size() * sizeof(TCHAR);
	TCHAR* pStrHtml = new TCHAR[nLen];
	MultiByteToWideChar(CP_UTF8, NULL, Result.c_str(), Result.length(), pStrHtml, nLen);
	//用户数据保存
#pragma region 正则表达式算法
	VBScript_RegExp_55::IRegExp2Ptr pRegexp(__uuidof(VBScript_RegExp_55::RegExp));
	pRegexp->PutGlobal(VARIANT_TRUE);
	pRegexp->put_Multiline(VARIANT_FALSE);
	pRegexp->PutIgnoreCase(VARIANT_FALSE);
	pRegexp->PutPattern(pTpatternSkin);
	VBScript_RegExp_55::IMatchCollectionPtr Item = pRegexp->Execute(pStrHtml);
	pRegexp->PutPattern(pTpatternName);
	VBScript_RegExp_55::IMatchCollectionPtr ItemName = pRegexp->Execute(pStrHtml);
	pRegexp->PutPattern(pTpatternImg);
	VBScript_RegExp_55::IMatchCollectionPtr ItemImg = pRegexp->Execute(pStrHtml);
	for (int i = 0; i < Item->GetCount(); i++) {
#pragma region 添加皮肤数据Item
#pragma region 获取皮肤兑换URL
		_bstr_t sSkinConvert = _T("");
		VBScript_RegExp_55::IMatch2Ptr Match = Item->GetItem(i);
		if (!Match)continue;
		VBScript_RegExp_55::ISubMatchesPtr Submatch = Match->GetSubMatches();
		if (!Submatch)continue;
		sSkinConvert = _T("https://bbs.93x.net/") + static_cast<_bstr_t>(Submatch->GetItem(0L));
#pragma endregion
#pragma region 获取皮肤名字
		_bstr_t sSkinName = _T("");
		if (i < ItemName->GetCount()) {
			Match = ItemName->GetItem(i);
			if (!Match)continue;
			Submatch = Match->GetSubMatches();
			if (!Submatch)continue;
			sSkinName = static_cast<_bstr_t>(Submatch->GetItem(0L));
		}
#pragma endregion
#pragma region 获取皮肤图片
		_bstr_t sSkinImg = _T("");
		if (i < ItemImg->GetCount()) {
			Match = ItemImg->GetItem(i);
			if (!Match)continue;
			Submatch = Match->GetSubMatches();
			if (!Submatch)continue;
			sSkinImg = static_cast<_bstr_t>(Submatch->GetItem(0L));
		}
#pragma endregion
		CDialogBuilder BuilderXml;
		CControlUI* pNewXml = BuilderXml.Create(_T("xml\\SkinConvert_Button.xml"), NULL, NULL, &m_pTilelayout);
		if (!pNewXml)continue;
		pTilelayout->Add(pNewXml);
		CButtonUI* pButton = static_cast<CButtonUI*>(m_pTilelayout.FindSubControlByName(pNewXml, _T("Button_SkinConvert")));
		if (pButton)pButton->SetUserData(sSkinConvert);
		CTextUI* pText = static_cast<CTextUI*>(m_pTilelayout.FindSubControlByName(pNewXml, _T("Text_SkinConvert")));
		srand(GetTickCount());
		if (pText) {
			pText->SetTextColor(RGB(rand() % 255, rand() % 255, rand() % 255));
			pText->SetText(sSkinName);
		}
		CLabelUI* pLabel = static_cast<CLabelUI*>(m_pTilelayout.FindSubControlByName(pNewXml, _T("Img_SkinConvert")));
		if (pLabel)pLabel->SetBkImage(获取图片地址(sSkinImg, i));
		
	}
	pRegexp.Release();
#pragma endregion
#pragma endregion
	delete[]pStrHtml;
	delete[]pTpatternSkin;
	delete[]pTpatternImg;
	delete[]pTpatternName;
}

void C新手皮肤UI::启动线程()
{
	std::thread t1 = std::thread(&C新手皮肤UI::获取新手皮肤数据, this);
	t1.detach();

}

void C新手皮肤UI::OnCreate()
{
	m_pageNumMax = m_PageNum;
}

