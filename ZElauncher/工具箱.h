#pragma once
#include "stdafx.h"
#include <atlimage.h>

using namespace DuiLib;

class C������UI :
	public CContainerUI
	, public INotifyUI
{
public:
	C������UI(CPaintManagerUI& m_paintmanage);

private:
	void Notify(TNotifyUI& msg);
	void OnClick(CControlUI* Click);
	void OnCreate();

private:
	CPaintManagerUI* m_paintmanager;
	CPaintManagerUI m_pm;
};