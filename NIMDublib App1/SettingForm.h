#pragma once
#include "framework.h"

class SettingForm :public nim_comp::WindowEx
{
public:
	SettingForm();
	~SettingForm();

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;

	virtual void InitWindow() override;

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	static const LPCTSTR kClassName;

private:
	bool SetCurWeeksCmb(int n);
	bool OnTotalWeeksCmbSelChange(ui::EventArgs* args);

	void AddClass(ui::ListBox* listbox);

	ui::Combo* CurWeeksCmb;
	ui::Combo* TotalWeeksCmb;
	ui::Combo* PerClassTimeCmb;
	ui::Combo* PerBreakTimeCmb;
	ui::Combo* ForenoonCoursesCmb;
	ui::Combo* AfternoonCoursesCmb;
	ui::Combo* NightCoursesCmb;
	ui::ListBox* ForenoonCoursesList;
	ui::ListBox* AfternoonCoursesList;
	ui::ListBox* NightCoursesList;
};

