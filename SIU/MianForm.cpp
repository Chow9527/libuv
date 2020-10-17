#include "stdafx.h"
#include "MianForm.h"
#include <string>


const std::wstring MianForm::kClassName = L"SIU";

MianForm::MianForm()
{
}


MianForm::~MianForm()
{
}

std::wstring MianForm::GetSkinFolder()
{
	return L"siu";
}

std::wstring MianForm::GetSkinFile()
{
	return L"siu.xml";
}

std::wstring MianForm::GetWindowClassName() const
{
	return kClassName;
}

void MianForm::InitWindow()
{
	m_Open = dynamic_cast<ui::Button*>(FindControl(L"btn_open"));
	m_Open->AttachClick(nbase::Bind(&MianForm::OnClick,this,std::placeholders::_1));
}

LRESULT MianForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}


bool MianForm::OnClick(ui::EventArgs* args)
{
	//
	nim_comp::ShowMsgBox(GetHWND(), nim_comp::MsgboxCallback(), L"消息框", false, L"提示", false,L"确定",true);
	return true;
}