#include "Item.h"

CourseItem::CourseItem()
{
}

CourseItem::~CourseItem()
{
}

void CourseItem::InitSubControls(CourseInfo* ci)
{
	// 查找 Item 下的控件
	course_img_ = dynamic_cast<ui::Control*>(FindSubControl(L"course_img"));
	course_name_ = dynamic_cast<ui::Label*>(FindSubControl(L"course_name"));
	btn_del_ = dynamic_cast<ui::Button*>(FindSubControl(L"btn_del"));
	item_autosign = dynamic_cast<ui::Label*>(FindSubControl(L"item_autosign"));
	item_autoanswer = dynamic_cast<ui::Label*>(FindSubControl(L"item_autoanswer"));
	course_name_->SetText(ci->name);

	item_autoanswer->SetVisible(ci->IsAutoAnswer);
	item_autosign->SetVisible(ci->IsAutoSign);

	this->SetUserDataBase(ci);
	// 绑定删除任务处理函数
	btn_del_->AttachClick(nbase::Bind(&CourseItem::OnRemove, this, std::placeholders::_1));
}

bool CourseItem::OnRemove(ui::EventArgs* args)
{
	ui::ListBox* parent = dynamic_cast<ui::ListBox*>(this->GetParent());
	return parent->Remove(this);
}

void CourseItem::SetAutoSign(bool autosign)
{
	item_autosign->SetVisible(autosign);
	CourseInfo* ci = (CourseInfo*)GetUserDataBase();
	ci->IsAutoSign = autosign;
}

void CourseItem::SetAutoAnswer(bool autoanswer)
{
	item_autoanswer->SetVisible(autoanswer);
	CourseInfo* ci = (CourseInfo*)GetUserDataBase();
	ci->IsAutoAnswer = autoanswer;
}
