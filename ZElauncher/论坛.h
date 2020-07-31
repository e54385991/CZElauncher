#pragma once
#include "stdafx.h"
#include "�鿴����.h"
#include "htmlsdk/html_parser.hpp"

using namespace DuiLib;

namespace CBBSUI
{
	struct ����
	{
		std::string ������;
		std::string �ظ���;
		std::string �鿴��;
		std::string hrefurl;
		bool �Ƿ��к��;
		std::string �������;
		std::string ����ܽ��;
		std::string ���ʣ����;
		UINT type;
		std::string typename_;
	};
	struct ����
	{
		std::string ��������;
		std::string �����û�;
		std::string ����ʱ��;
		UINT type;
	};
}

class C��̳UI :
	public CContainerUI
	,public INotifyUI
{
public:
	C��̳UI(CPaintManagerUI& m_paintmanager);;
	~C��̳UI();
	void ShowBBSList();
private:
	void Notify(TNotifyUI& msg);
	void MenuClick(CControlUI* Click);
	void OnCreate();
	void OnClick(CControlUI* Click);
	void GetBBSList(_bstr_t url, UINT type = NULL);
	void GetSaylaba(shared_ptr<HtmlDocument>& doc);
	void Threadlaba();
	std::vector<std::shared_ptr<HtmlElement>> GetTagValue(const char* name, const char* tagname, shared_ptr<HtmlElement>& tbody);
	void BrowserBBS();
	void OnLookBBS();
private:
	CDialogBuilder builder;
	CPaintManagerUI m_pm2;
	CPaintManagerUI* m_paintmanager_;
	std::vector<CBBSUI::����> ������Ϣ;
	std::vector<std::string> ������Ϣ;
	UINT m_SaynCount;
	bool �Ƿ����ڻ�ȡ������Ϣ;
	bool ��ֹ�ö�����ʾ;
};