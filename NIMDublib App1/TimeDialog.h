#pragma once
#include "framework.h";
typedef std::function<void(CourseTime)> TimeDlgCallback;

void ShowTimeDlg(HWND hwnd, CourseTime& defaulttime, TimeDlgCallback cb, const std::wstring& title = L"Ê±¼ä");

class TimeDialog : public nim_comp::WindowEx
{
public:
	TimeDialog(CourseTime& time);
	~TimeDialog();

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;

	virtual void InitWindow() override;
	virtual void Close(UINT nRet = IDOK) override;

	static const LPCTSTR kClassName;

	friend void ShowTimeDlg(HWND hwnd, CourseTime& defaulttime, TimeDlgCallback cb, const std::wstring& title);

private:

	bool OnClicked(ui::EventArgs* msg);

	void SetTitle(const std::wstring& str);
	void Show(TimeDlgCallback cb);

	void EndTimeDlg(nim_comp::MsgBoxRet ret);

	CourseTime course_time_;
	ui::Label* title_;
	ui::Button* btn_yes_;
	ui::Button* btn_no_;
	ui::Combo* begin_hour_cmb_;
	ui::Combo* begin_minute_cmb_;
	ui::Combo* end_hour_cmb_;
	ui::Combo* end_minute_cmb_;

	TimeDlgCallback timedialog_callback_;
};

