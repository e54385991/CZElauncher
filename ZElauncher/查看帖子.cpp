#include "�鿴����.h"

CDuiString g_BBSUrl = L"";

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

void C�鿴����UI::InitWindow()
{
	
}

void C�鿴����UI::OnExit(const TNotifyUI& msg)
{
	
	__super::Close();
}


void C�鿴����UI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0) {
		::SetForegroundWindow(m_hWnd);//���ڴ�����Ϻ�,��ȡ����;
		��ȡ��������();
	}
	else if (_tcscmp(msg.pSender->GetName(), _T("closebtn")) == 0)OnExit(msg);

}

void C�鿴����UI::��ȡ��������()
{
	CListUI* pTilelayout = static_cast<CListUI*>(m_pm.FindControl(_T("Table_LookBBS")));
	if (!pTilelayout)return;
	for (UINT i = 0; i < 20; i++) {
		CDialogBuilder BuilderXml;
		CControlUI* pNewXml = BuilderXml.Create(_T("xml\\BBS_Nick.xml"), NULL, NULL, m_paintmanager);
		if (!pNewXml)return;
		pTilelayout->Add(pNewXml);
	}
}