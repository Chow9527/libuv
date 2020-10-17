#include "stdafx.h"
#include "MainForm.h"

const std::wstring MainForm::kClassName = L"BaiduNetDiskDemo";

MainForm::MainForm()
{
}


MainForm::~MainForm()
{
}

std::wstring MainForm::GetSkinFolder()
{
	return L"mainform";
}

std::wstring MainForm::GetSkinFile()
{
	return L"main.xml";
}

std::wstring MainForm::GetWindowClassName() const
{
	return kClassName;
}

void MainForm::InitWindow()
{
	m_OptionBox = dynamic_cast<ui::OptionBox*>(FindControl(L"mynetdisk"));
	m_pRoot->AttachBubbledEvent(ui::kEventClick, nbase::Bind(&MainForm::OnClicked, this, std::placeholders::_1));
}

LRESULT MainForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
} 


bool MainForm::OnClicked(ui::EventArgs* args)
{
	return true;
}