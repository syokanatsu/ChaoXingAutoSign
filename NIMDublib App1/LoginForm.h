#pragma once
#include "framework.h"

class LoginForm :public nim_comp::WindowEx
{
public :
	LoginForm();
	~LoginForm();

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;

	virtual void InitWindow() override;

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	bool OnClick(ui::EventArgs* msg);

	static const std::wstring kClassName;

	static void GetUid();
	static std::string cookies;
	static std::string uid;

private:
	bool loginWithPassword();
	bool loginWithQRCode();
	void getAuthStatus(std::string uuid, std::string enc);
	std::string makeRandomUUID(int n);

	void ShowMainWindow();

	bool ifOpenMainWindow;
};

