#pragma once
#include "stdafx.h"
#include <atlimage.h>

using namespace DuiLib;

class C����Ƥ��UI :
	public CContainerUI
	, public INotifyUI
{
	struct SkinConvertInfo
	{
		_bstr_t sSkinConvertUrl = "";
		_bstr_t sSkinName = "";
		_bstr_t sSkinImg = "";
	};
public:
	C����Ƥ��UI(CPaintManagerUI& m_paintmanage);
	void �����߳�();
	void ��ȡ����Ƥ������();
private:
	void Notify(TNotifyUI& msg);
	void OnClick(CControlUI* Click);
	void OnCreate();
	std::string ��ȡƤ��html����(const TCHAR* Page);
	CDuiString ��ȡͼƬ��ַ(LPCTSTR pUrl, UINT iFlag);
	void �һ�Ƥ��(LPCTSTR pUrl);
private:
	CPaintManagerUI* m_paintmanager;
	CPaintManagerUI m_pm;
	CPaintManagerUI m_pTilelayout;
	CContainerUI* pSKin = nullptr;
	UINT m_PageNum;
	UINT m_pageNumMax;
	std::vector<SkinConvertInfo> m_SkinConvertInfo;
};