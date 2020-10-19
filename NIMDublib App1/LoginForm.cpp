#include "LoginForm.h"
#include "BasicForm.h"
#include "MainWindow.h"

const std::wstring LoginForm::kClassName = L"Login";
std::string LoginForm::cookies;
std::string LoginForm::uid;

LoginForm::LoginForm() :
	ifOpenMainWindow(false)
{
}

LoginForm::~LoginForm()
{
}

std::wstring LoginForm::GetSkinFolder()
{
	return L"login";
}

std::wstring LoginForm::GetSkinFile()
{
	return L"login.xml";
}

std::wstring LoginForm::GetWindowClassName() const
{
	return kClassName;
}

std::wstring LoginForm::GetWindowId() const
{
	return kClassName;
}

void LoginForm::InitWindow()
{
	this->SetTaskbarTitle(L"登录");
	m_pRoot->AttachBubbledEvent(ui::kEventClick, nbase::Bind(&LoginForm::OnClick, this, std::placeholders::_1));


	TCHAR buffer[10240] = { 0 };
	long n = GetPrivateProfileString(L"Login", L"Cookie", L"", buffer, sizeof(buffer), (nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str());
	if (n == 0) return;
	cookies = TextUtfEncoding::ToUtf8(buffer);
	n = GetPrivateProfileString(L"Login", L"uid", L"", buffer, sizeof(buffer), (nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str());
	if (n == 0) return;
	uid = TextUtfEncoding::ToUtf8(buffer);
	nbase::ThreadManager::PostDelayedTask(kThreadUI, nbase::Bind([this]() {
		nim_comp::ShowMsgBox(GetHWND(), [this](nim_comp::MsgBoxRet sel) {
			if (sel == 0)// yes
			{
				this->ifOpenMainWindow = true;
				this->Close();
			}
		}, L"检测到上次登录信息，是否使用上次登录信息？", false, L"", false, L"是", false, L"否", false);
	}), nbase::TimeDelta::FromMilliseconds(500));
	
}

LRESULT LoginForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (ifOpenMainWindow) {
		// 打开主界面
		ShowMainWindow();
	}
	else {
		// 关闭程序，不进入主界面
		PostQuitMessage(0L);
	}
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

bool LoginForm::OnClick(ui::EventArgs* msg)
{
	if (msg->pSender->GetName() == L"loginbtn") {
		if (loginWithPassword()) {
			nim_comp::Toast::ShowToast(L"登录成功", 1000, GetHWND());
			ifOpenMainWindow = true;
			nbase::ThreadManager::PostDelayedTask(kThreadUI, nbase::Bind(&LoginForm::Close, this, 1U), nbase::TimeDelta::FromMilliseconds(1500));
		}
	}
	else if (msg->pSender->GetName() == L"getQRCodebtn") {
		if (!loginWithQRCode()) {
			nim_comp::Toast::ShowToast(L"获取二维码失败", 1000, GetHWND());
		}
	}
	return true;
}

void LoginForm::GetUid()
{
	int pos = cookies.find("UID=") + 4;
	int endpos = cookies.find_first_of(';', pos);
	uid = cookies.substr(pos, endpos - pos);
}

bool LoginForm::loginWithPassword()
{
	string uname;
	string pwd;
	ui::RichEdit* rename = static_cast<ui::RichEdit*>(FindControl(L"unameEdit"));
	ui::RichEdit* repwd = static_cast<ui::RichEdit*>(FindControl(L"pwdEdit"));
	uname = TextUtfEncoding::ToUtf8(rename->GetText());
	pwd = TextUtfEncoding::ToUtf8(repwd->GetText());
	cpr::Url url{ "http://passport2.chaoxing.com/api/login" };
	cpr::Parameters params{
		{"name",uname},
		{"pwd",pwd},
		{"schoolid",""},
		{"verify","0"}
	};
	cpr::Header header = { {"User-Agent",USERAGENT} };
	auto res = cpr::Get(url, params, header);
	try
	{
		Value root;
		Reader reader;
		reader.parse(res.text, root);
		if (true == root["result"].asBool()) {
			cookies = res.cookies.GetEncoded();
			uid = res.cookies["_uid"];
			WritePrivateProfileString(L"Login", L"Cookie", TextUtfEncoding::FromUtf8(cookies).c_str(), (nbase::win32::GetCurrentModuleDirectory()+PROFILENAME).c_str());
			WritePrivateProfileString(L"Login", L"uid", TextUtfEncoding::FromUtf8(uid).c_str(), (nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str());
			auto err = GetLastError();
			OutputDebugString(nbase::StringPrintf(L"%d", err).c_str());
			//GetUid(); // 设置UID
			//nbase::WriteFile(L"login.dat", cookies);
			return true;
		}
		else {
			nim_comp::Toast::ShowToast(TextUtfEncoding::FromUtf8(root["errorMsg"].asString()), 1000, this->GetHWND());
		}
	}
	catch (const std::exception&e)
	{
		nim_comp::Toast::ShowToast(TextUtfEncoding::FromUtf8(e.what()), 1000, GetHWND());
	}
	return false;
}

bool LoginForm::loginWithQRCode()
{
	string uuid = makeRandomUUID(8);
	cpr::Url url{ "http://passport2.chaoxing.com/createqr" };
	cpr::Header header = { {"User-Agent",USERAGENT} };
	cpr::Parameters params = { {"uuid",uuid},{"fid","-1"} };
	auto res = cpr::Get(url, header, params);
	if (res.status_code == 200) {
		std::string TempDir = TextUtfEncoding::ToUtf8(nbase::win32::GetTempDir());
		FILE* fp;
		fopen_s(&fp, (TempDir + "qrcode.png").c_str(), "wb");
		if (fp) {
			fwrite(res.text.c_str(), 1, res.text.size(), fp);
			fclose(fp);
			ui::Label* qrcodeimg = static_cast<ui::Label*>(FindControl(L"qrcode"));
			qrcodeimg->SetBkImage(TextUtfEncoding::FromUtf8(TempDir) + L"qrcode.png");
			png_image image;
			memset(&image, 0, sizeof(image));
			image.version = PNG_IMAGE_VERSION;
			if (png_image_begin_read_from_memory(&image, res.text.c_str(), res.text.size()) != 0) {
				png_bytep buffer;
				image.format = PNG_FORMAT_RGBA;
				buffer = new png_byte[PNG_IMAGE_SIZE(image)];
				if (buffer != NULL &&
					png_image_finish_read(&image, NULL, buffer, 0, NULL) != 0) {
					auto result = ReadBarcode(image.width, image.height, (unsigned char*)buffer, image.width * 4, 4, 0, 1, 2,
						{ BarcodeFormat::QR_CODE }, false, true);
					if (result.isValid()) {
						int pos = result.text().find(L"enc=") + 4;
						int endpos = result.text().find_first_of(L'&', pos);
						string enc = TextUtfEncoding::ToUtf8(result.text().substr(pos, endpos - pos));
						nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&LoginForm::getAuthStatus, this, uuid, enc));
						return true;
					}
				}
			}
		}
	}
	return false;
}

void LoginForm::getAuthStatus(std::string uuid, std::string enc)
{
	bool isOK = false;
	cpr::Url url = "http://passport2.chaoxing.com/getauthstatus";
	cpr::Header header = { {"User-Agent",USERAGENT} };
	cpr::Parameters params = { {"uuid",uuid},{"enc",enc} };
	auto res = cpr::Post(url, params, header);
	if (!res.text.empty()) {
		Reader reader;
		Value root;
		reader.parse(res.text, root);
		if (isOK = root["status"].asBool()) {	// 登陆成功
			cookies = res.cookies.GetEncoded();
			uid = res.cookies["_uid"];
			WritePrivateProfileString(L"Login", L"Cookie", TextUtfEncoding::FromUtf8(cookies).c_str(), (nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str());
			WritePrivateProfileString(L"Login", L"uid", TextUtfEncoding::FromUtf8(uid).c_str(), (nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str());
			//GetUid();	// 设置uid
			//nbase::WriteFile(L"login.dat", cookies);
			nim_comp::Toast::ShowToast(L"登录成功", 1000, GetHWND());
			ifOpenMainWindow = true;
			nbase::ThreadManager::PostDelayedTask(kThreadUI, nbase::Bind(&LoginForm::Close, this, 1U), nbase::TimeDelta::FromMilliseconds(1500));
		}
	}
	if(!isOK) {
		nbase::ThreadManager::PostDelayedTask(kThreadUI, nbase::Bind(&LoginForm::getAuthStatus, this, uuid, enc), nbase::TimeDelta::FromSeconds(1));
	}
}

std::string LoginForm::makeRandomUUID(int n)
{
	srand(time(NULL));
	std::string uuid;
	char dict[] = { "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789" };
	uuid.clear();
	for (int i = 0; i < n; i++) {
		uuid += dict[rand() % 62];
	}
	return uuid;
}

void LoginForm::ShowMainWindow()
{
	MainWindow* mainwindow = new MainWindow();
	mainwindow->Create(NULL, MainWindow::kClassName, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	mainwindow->CenterWindow();
	mainwindow->ShowWindow();
}
