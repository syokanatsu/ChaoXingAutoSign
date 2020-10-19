#pragma once
#include "framework.h"

class CourseItem : public ui::ListContainerElement
{
public:
	CourseItem();
	~CourseItem();

	// 提供外部调用来初始化 item 数据
	void InitSubControls(CourseInfo* ci);

	void SetAutoSign(bool autosign);
	void SetAutoAnswer(bool autoanswer);

private:
	bool OnRemove(ui::EventArgs* args);

private:
	ui::ListBox* list_box_;

	ui::Control* course_img_;
	ui::Label* course_name_;
	ui::Button* btn_del_;

	ui::Label* item_autosign;
	ui::Label* item_autoanswer;
};

