#pragma once
#include "framework.h"
class BasicForm :
	public nim_comp::WindowEx
{
public:
	BasicForm();
	~BasicForm();

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;
	virtual std::wstring GetWindowId() const override;
	
	virtual void InitWindow() override;

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	static const std::wstring kClassName;

	class CourseInfo : public ui::UserDataBase
	{
	public:
		string name = "";
		string classid = "";
		string courceid = "";
		CourseInfo() {}
		CourseInfo(const CourseInfo& other) 
		{
			this->name = other.name;
			this->classid = other.classid;
			this->courceid = other.courceid;
		}
	};

	class ActiveInfo
	{
	public:
		string name;
		string url;
		string id;
		int status;
		string activeType;
		int attendNum;
	};

private:

	// 判断是否课程已经添加到选择列表中
	bool IsCourseExistInSelectList(ui::ListContainerElement* element);

	int GetCourses();

	int GetSignCount(string courseId);



	vector<ActiveInfo> GetActivities(string courseid, string classid);
	bool ActivitySign(ActiveInfo ai);
	void DelaySign(ActiveInfo ai, ULONG times);


	vector<CourseInfo> courses;

	ui::ListBox* courseslist;
	ui::ListBox* selectlist;
	ui::Button* addbtn;
	ui::Button* removebtn;
	ui::Button* startbtn;
	ui::Button* endbtn;
	ui::Label* spystatustxt;
	ui::Slider* delaysld;
	ui::RichEdit* delaytxt;
	ui::CheckBox* multitimeschk;
	ui::CheckBox* notfirstsignchk;
	ui::Button* settings;
	ui::RichEdit* ranktxt;
	ui::Label* spyontimes;
	ui::Label* successtimes;
	ULONG spytimesl;
	UINT nsuccesstimes;
	int delaytime;
	bool notQuit;

	nbase::WeakCallbackFlag timerflag;
};

