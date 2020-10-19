#pragma once
#include "framework.h"

class ClassItem :public ui::ListContainerElement
{
public:
	ClassItem();
	~ClassItem();

	void InitSubControls(int index, CourseTime& time);

	void SetStartTime(nbase::Time::TimeStruct& time);
	void SetEndTime(nbase::Time::TimeStruct& time);

	const CourseTime& GetTime();

	int GetClassIndex();
	void SetClassIndex(int index);

	bool OnEditBtnClick(ui::EventArgs* args);

private:
	CourseTime time_;
	int classIndex;
	ui::Label* indexlbl;
	ui::Label* timelbl;
	ui::Button* itemeditbtn;
};

