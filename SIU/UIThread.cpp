#include "stdafx.h"
#include "UIThread.h"
#include "MianForm.h"


enum ThreadId
{
	kThreadUI
};



void UIThread::Init()
{
	nbase::ThreadManager::RegisterThread(kThreadUI);

	// 获取资源路径，初始化全局参数
	std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
	ui::GlobalManager::Startup(theme_dir + L"themes\\" , ui::CreateControlCallback(), false, L"", L"");
	// 创建一个默认带有阴影的居中窗口
	MianForm* window = new MianForm();
	window->Create(NULL, MianForm::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	window->CenterWindow();
	window->ShowWindow();
}

void UIThread::Cleanup()
{
	ui::GlobalManager::Shutdown();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}