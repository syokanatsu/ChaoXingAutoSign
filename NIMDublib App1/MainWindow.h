#pragma once
#include "framework.h"

class MainWindow : public nim_comp::WindowEx
{
public:

	MainWindow();
	~MainWindow();
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;

	virtual void InitWindow() override;

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	static const LPCTSTR kClassName;

private:
	bool setting_wnd_btn_Click(ui::EventArgs* args);
	bool editbtn_Click(ui::EventArgs* args);
	bool addbtn_Click(ui::EventArgs* args);
	bool startbtn_Click(ui::EventArgs* args);
	bool stopbtn_Click(ui::EventArgs* args);
	bool checkgroup_Select(ui::EventArgs* args);
	bool checkgroup_UnSelect(ui::EventArgs* args);
	bool weeklist_SelectChanged(ui::EventArgs* args);

	void Disabled_checkBoxGroup();
	void Enabled_checkBoxGroup(CourseInfo& info);

	// UI 无关事务初始化，交给杂物线程来完成
	void InitTask();

	void SelectDayOfWeek(int dayofweek);

	CourseTime& GetCourseTime(int start, int end);
	ui::ListContainerElement* GetCourseListCurSel();

private:
	ui::TabBox* weeklist;
	ui::ListBox* list1;
	ui::ListBox* list2;
	ui::ListBox* list3;
	ui::ListBox* list4;
	ui::ListBox* list5;
	ui::ListBox* list6;
	ui::ListBox* list7;

	ui::Option* monday;
	ui::Option* tuesday;
	ui::Option* wednesday;
	ui::Option* thursday;
	ui::Option* friday;
	ui::Option* saturday;
	ui::Option* sunday;

	ui::Button* setting_wnd_btn;
	ui::Button* editbtn;
	ui::Button* addbtn;
	ui::Button* startbtn;
	ui::Button* stopbtn;
	ui::Box* checkgroup;

	ui::Control* disablectl;
	ui::CheckBox* autosign;
	ui::CheckBox* autoanswer;
	ui::CheckBox* multisign;
	ui::CheckBox* notfirstsign;

	ui::Label* course_name;
	ui::RichEdit* starttime;
	ui::RichEdit* endtime;
};

