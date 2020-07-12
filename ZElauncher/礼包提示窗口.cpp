#include "�����ʾ����.h"


void CAwardTipUI::Notify(TNotifyUI& msg)
{
	if (_tcscmp(msg.sType, _T("windowinit")) == 0)OnTip();
	else if (_tcscmp(msg.sType, _T("click")) == 0)OnClick(msg.pSender);
}

LRESULT CAwardTipUI::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("Award_tip.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	return 0;
}

void CAwardTipUI::OnClick(CControlUI* pSender)
{
	if (_tcscmp(pSender->GetName(), _T("closebtn")) == 0)OnClose();
	else if (_tcscmp(pSender->GetName(), _T("minbtn")) == 0)SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, NULL);
}

void CAwardTipUI::OnTip()
{
	std::wstring pStr = m_AwardTip.GetData();
	std::wstring Str_yezi;
	std::wstring Str_bili22;
	std::wstring Str_huaji;
	UINT Pos = pStr.find(_T("���ѻ��"));
	if (Pos != std::wstring::npos) {
		UINT nLen = _tcslen(_T("���ѻ��"));
		Str_yezi = pStr.substr(Pos + nLen, pStr.find(_T("Ҷ��"), Pos) - Pos - nLen);
	}
	else Str_yezi = L"None";
	Pos = pStr.find(_T("���� ͷ��"));
	if (Pos != std::wstring::npos) {
		UINT nLen = _tcslen(_T("���� ͷ��"));
		Str_huaji = pStr.substr(Pos + nLen, pStr.find(_T("��"), Pos) - Pos - nLen);
	}
	else Str_huaji = L"None";
	Pos = pStr.find(_T("BiliBili22��"));
	if (Pos != std::wstring::npos) {
		UINT nLen = _tcslen(_T("BiliBili22��"));
		Str_bili22 = pStr.substr(Pos + nLen, pStr.find(_T("��"), Pos) - Pos - nLen);
	}
	else Str_bili22 = L"None";
	CLabelUI* pLabel_bili22 = static_cast<CLabelUI*>(m_pm.FindControl(_T("Label_bili22")));
	if (pLabel_bili22) {
		std::wstring pText = _T("bilibili22�� ");
		pText += Str_bili22;
		pText += _T(" ��");
		pLabel_bili22->SetText(pText.c_str());
	}
	CLabelUI* pLabel_huaji = static_cast<CLabelUI*>(m_pm.FindControl(_T("Label_huaji")));
	if (pLabel_huaji) {
		std::wstring pText = _T("����ͷ�� ");
		pText += Str_huaji;
		pText += _T(" ��");
		pLabel_huaji->SetText(pText.c_str());
	}
	CLabelUI* pLabel_yezi = static_cast<CLabelUI*>(m_pm.FindControl(_T("Label_yezi")));
	if (pLabel_yezi) {
		std::wstring pText = _T("Ҷ�� ");
		pText += Str_yezi;
		pText += _T(" ");
		pLabel_yezi->SetText(pText.c_str());
	}
}

LRESULT CAwardTipUI::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CAwardTipUI::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

void CAwardTipUI::OnClose()
{
	__super::Close();
}

LRESULT CAwardTipUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}