#include "MainThread.h"
#include "LoginForm.h"
#include "resource.h"

void MainThread::Init()
{
	nbase::ThreadManager::RegisterThread(kThreadUI);


	// 启动杂事处理线程
	misc_thread_.reset(new MiscThread(kThreadGlobalMisc, "Global Misc Thread"));
	misc_thread_->Start(); 
	log4cpp::PropertyConfigurator::configure("./log.conf");

	//// 开启DPI感知
	//auto dpi = ui::DpiManager::GetInstance();
	//if (dpi->SetAdaptDPI()) {
	//	auto ndpi = dpi->GetSystemDPI();
	//	OutputDebugString(nbase::StringPrintf(L"DPI：%d", ndpi).c_str());
	//	dpi->SetScale(ndpi);
	//}
	//bool adapt_dpi = dpi->IsAdaptDPI();

	//ui::GlobalManager::OpenResZip(MAKEINTRESOURCE(IDR_RESOURCES), L"THEME", "");
	//ui::GlobalManager::Startup(L"resources\\", ui::CreateControlCallback(), false);
	std::wstring theme_dir = nbase::win32::GetCurrentModuleDirectory();
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);
	


	// 创建主窗口
	LoginForm* loginwnd = new LoginForm();
	loginwnd->Create(NULL, LoginForm::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	loginwnd->CenterWindow();
	loginwnd->ShowWindow();
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();

	misc_thread_->Stop();
	misc_thread_.reset(nullptr);

	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}

void MiscThread::Init()
{
	nbase::ThreadManager::RegisterThread(this->thread_id_);
}

void MiscThread::Cleanup()
{
	nbase::ThreadManager::UnregisterThread();
}
