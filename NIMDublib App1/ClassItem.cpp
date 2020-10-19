#include "ClassItem.h"
#include "TimeDialog.h"
#include "SettingForm.h"

ClassItem::ClassItem()
{
}

ClassItem::~ClassItem()
{
}

void ClassItem::InitSubControls(int index, CourseTime& time)
{
	indexlbl = dynamic_cast<ui::Label*>(FindSubControl(L"indexlbl"));
	timelbl = dynamic_cast<ui::Label *>(FindSubControl(L"timelbl"));
	itemeditbtn = dynamic_cast<ui::Button*>(FindSubControl(L"itemeditbtn"));

	itemeditbtn->AttachClick(nbase::Bind(&ClassItem::OnEditBtnClick, this, std::placeholders::_1));

	SetClassIndex(index);
	SetStartTime(time.starttime);
	SetEndTime(time.endtime);
}

void ClassItem::SetStartTime(nbase::Time::TimeStruct& time)
{
	time_.starttime = time;
	wstring str = nbase::StringPrintf(L"%02d:%02d-%02d:%02d", time_.starttime.hour_, time_.starttime.minute_, time_.endtime.hour_, time_.endtime.minute_);
	timelbl->SetText(str);
}

void ClassItem::SetEndTime(nbase::Time::TimeStruct& time)
{
	time_.endtime = time;
	wstring str = nbase::StringPrintf(L"%02d:%02d-%02d:%02d", time_.starttime.hour_, time_.starttime.minute_, time_.endtime.hour_, time_.endtime.minute_);
	timelbl->SetText(str);
}

const CourseTime& ClassItem::GetTime()
{
	// TODO: 在此处插入 return 语句
	return time_;
}

int ClassItem::GetClassIndex()
{
	return classIndex;
}

void ClassItem::SetClassIndex(int index)
{
	classIndex = index;
	wstring str = nbase::StringPrintf(L"第%02d节", classIndex);
	indexlbl->SetText(str);
}

bool ClassItem::OnEditBtnClick(ui::EventArgs* args)
{
	nim_comp::WindowEx* window= nim_comp::WindowsManager::GetInstance()->GetWindow(SettingForm::kClassName, SettingForm::kClassName);
	ShowTimeDlg(window->GetHWND(), time_, [this](CourseTime) {

	});
	return true;
}
