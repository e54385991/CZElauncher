#pragma once
#include "stdafx.h"
#include <atlimage.h>

using namespace DuiLib;

class C�̳�UI :
	public CContainerUI
	, public INotifyUI
{
public:
	C�̳�UI(CPaintManagerUI& m_paintmanage);
	void ������ȡ�̳����ݺ���();
private:
	void Notify(TNotifyUI& msg);
	void OnClick(CControlUI* Click);
	void OnCreate();
	void ��ȡ�̳�����();
	void ����ͼƬ����״̬();
	bool �ж�������ʽ���ݿ��Ƿ����(TCHAR* ������ʽ);
	std::string ��ȡƤ��html����(const TCHAR* Page);
private:
	CPaintManagerUI* m_paintmanage_;
	CPaintManagerUI m_pm2;
	UINT m_PageNum;
	UINT m_pageNumMax;
};