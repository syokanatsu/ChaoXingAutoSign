#include "SettingForm.h"
#include "ClassItem.h"

const LPCTSTR SettingForm::kClassName = L"SettingForm";

SettingForm::SettingForm()
{
}

SettingForm::~SettingForm()
{
}

std::wstring SettingForm::GetSkinFolder()
{
	return L"setting";
}

std::wstring SettingForm::GetSkinFile()
{
	return L"setting.xml";
}

std::wstring SettingForm::GetWindowClassName() const
{
	return kClassName;
}

std::wstring SettingForm::GetWindowId() const
{
	return kClassName;
}

void SettingForm::InitWindow()
{
	CurWeeksCmb				= dynamic_cast<ui::Combo*>(FindControl(L"CurWeeksCmb"));
	TotalWeeksCmb			= dynamic_cast<ui::Combo*>(FindControl(L"TotalWeeksCmb"));
	PerClassTimeCmb			= dynamic_cast<ui::Combo*>(FindControl(L"PerClassTimeCmb"));
	PerBreakTimeCmb			= dynamic_cast<ui::Combo*>(FindControl(L"PerBreakTimeCmb"));
	ForenoonCoursesCmb		= dynamic_cast<ui::Combo*>(FindControl(L"ForenoonCoursesCmb"));
	AfternoonCoursesCmb		= dynamic_cast<ui::Combo*>(FindControl(L"AfternoonCoursesCmb"));
	NightCoursesCmb			= dynamic_cast<ui::Combo*>(FindControl(L"NightCoursesCmb"));
	ForenoonCoursesList		= dynamic_cast<ui::ListBox*>(FindControl(L"ForenoonCoursesList"));
	AfternoonCoursesList	= dynamic_cast<ui::ListBox*>(FindControl(L"AfternoonCoursesList"));
	NightCoursesList		= dynamic_cast<ui::ListBox*>(FindControl(L"NightCoursesList"));

	for (auto i = 1; i <= 24; i++)
	{
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetClass(L"listitem");
		element->SetFixedHeight(30);
		element->SetBkColor(L"white");
		element->SetTextPadding({ 6,0,6,0 });
		element->SetText(nbase::StringPrintf(L"%d", i));
		TotalWeeksCmb->Add(element);
	}
	TotalWeeksCmb->SelectItem(17);
	SetCurWeeksCmb(18);
	CurWeeksCmb->SelectItem(9);
	for (auto i = 0; i <= 120; i += 5) 
	{
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetClass(L"listitem");
		element->SetFixedHeight(30);
		element->SetBkColor(L"white");
		element->SetTextPadding({ 6,0,6,0 });
		element->SetText(nbase::StringPrintf(L"%02d min", i));
		PerClassTimeCmb->Add(element);
	}
	PerClassTimeCmb->SelectItem(9);
	for (auto i = 0; i <= 120; i += 5) 
	{
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetClass(L"listitem");
		element->SetFixedHeight(30);
		element->SetBkColor(L"white");
		element->SetTextPadding({ 6,0,6,0 });
		element->SetText(nbase::StringPrintf(L"%02d min", i));
		PerBreakTimeCmb->Add(element);
	}
	PerBreakTimeCmb->SelectItem(1);
	for (auto i = 0; i <= 6; i++) 
	{
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetClass(L"listitem");
		element->SetFixedHeight(30);
		element->SetBkColor(L"white");
		element->SetTextPadding({ 6,0,6,0 });
		element->SetText(nbase::StringPrintf(L"%d 节", i));
		ForenoonCoursesCmb->Add(element);
	}
	ForenoonCoursesCmb->SelectItem(5);
	for (auto i = 0; i <= 6; i++) 
	{
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetClass(L"listitem");
		element->SetFixedHeight(30);
		element->SetBkColor(L"white");
		element->SetTextPadding({ 6,0,6,0 });
		element->SetText(nbase::StringPrintf(L"%d 节", i));
		AfternoonCoursesCmb->Add(element);
	}
	AfternoonCoursesCmb->SelectItem(5);
	for (auto i = 0; i <= 6; i++) 
	{
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetClass(L"listitem");
		element->SetFixedHeight(30);
		element->SetBkColor(L"white");
		element->SetTextPadding({ 6,0,6,0 });
		element->SetText(nbase::StringPrintf(L"%d 节", i));
		NightCoursesCmb->Add(element);
	}
	NightCoursesCmb->SelectItem(3);
	
	int looptimes;
	nbase::StringToInt(ForenoonCoursesCmb->GetText(), &looptimes);
	for (auto i = 0; i < looptimes; i++)
		AddClass(ForenoonCoursesList);
	nbase::StringToInt(AfternoonCoursesCmb->GetText(), &looptimes);
	for (auto i = 0; i < looptimes; i++)
		AddClass(AfternoonCoursesList);
	nbase::StringToInt(NightCoursesCmb->GetText(), &looptimes);
	for (auto i = 0; i < looptimes; i++)
		AddClass(NightCoursesList);
	
	TotalWeeksCmb->AttachSelect(nbase::Bind(&SettingForm::OnTotalWeeksCmbSelChange, this, std::placeholders::_1));
}

LRESULT SettingForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

bool SettingForm::SetCurWeeksCmb(int n)
{
	int oldSel = CurWeeksCmb->GetCurSel();
	if (oldSel > n - 1)	// 总周数不能小于当前周数
	{
		return false;
	}
	CurWeeksCmb->RemoveAll();
	for (auto i = 1; i <= n; i++)
	{
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetClass(L"listitem");
		element->SetFixedHeight(30);
		element->SetBkColor(L"white");
		element->SetTextPadding({ 6,0,6,0 });
		element->SetText(nbase::StringPrintf(L"%d", i));
		CurWeeksCmb->Add(element);
	}
	CurWeeksCmb->SelectItem(oldSel);
	CurWeeksCmb->Invalidate();
	return true;
}

bool SettingForm::OnTotalWeeksCmbSelChange(ui::EventArgs* args)
{
	OutputDebugString(nbase::StringPrintf(L"sender: %s\nwParam: %d\nlParam: %d\n", args->pSender->GetName().c_str(),
		args->wParam, args->lParam).c_str());
	int sel = args->wParam;
	int oldSel = args->lParam;
	if (!SetCurWeeksCmb(sel + 1))
	{
		TotalWeeksCmb->SelectItem(oldSel);
		TotalWeeksCmb->Invalidate();
		nim_comp::Toast::ShowToast(L"不允许总周数小于当前周数", 1000, this->GetHWND());
	}
	return true;
}

void SettingForm::AddClass(ui::ListBox* listbox)
{
	int sum = listbox->GetCount();
	CourseTime ct;
	auto str = PerClassTimeCmb->GetText();
	str = str.substr(0, str.size() - 4);
	int classtime, breaktime;
	nbase::StringToInt(str, &classtime);
	str = PerBreakTimeCmb->GetText();
	str = str.substr(0, str.size() - 4);
	nbase::StringToInt(str, &breaktime);
	if (sum != 0)
	{
		auto itembef = dynamic_cast<ClassItem*>(listbox->GetItemAt(sum - 1));
		ct = itembef->GetTime();
		nbase::Time starttime = nbase::Time::FromTimeStruct(true, ct.endtime) + nbase::TimeDelta::FromMinutes(breaktime);
		nbase::Time endtime = starttime + nbase::TimeDelta::FromMinutes(classtime);
		ct.starttime = starttime.ToTimeStruct(true);
		ct.endtime = endtime.ToTimeStruct(true);
	}
	else 
	{
		if (listbox->GetName() == L"ForenoonCoursesList")	// 上午的课节数
		{
			ct.starttime.year_ = 1601;
			ct.starttime.month_ = 1;
			ct.starttime.day_of_week_ = 1;
			ct.starttime.day_of_month_ = 1;
			ct.starttime.hour_ = 7;
			ct.starttime.minute_ = 50;
		}
		else if (listbox->GetName() == L"AfternoonCoursesList")	// 下午的课节数
		{
			ct.starttime.year_ = 1601;
			ct.starttime.month_ = 1;
			ct.starttime.day_of_week_ = 1;
			ct.starttime.day_of_month_ = 1;
			ct.starttime.hour_ = 13;
			ct.starttime.minute_ = 50;
		}
		else if (listbox->GetName() == L"NightCoursesList")		// 晚上的课节数
		{
			ct.starttime.year_ = 1601;
			ct.starttime.month_ = 1;
			ct.starttime.day_of_week_ = 1;
			ct.starttime.day_of_month_ = 1;
			ct.starttime.hour_ = 19;
			ct.starttime.minute_ = 0;
		}
		nbase::Time endtime = nbase::Time::FromTimeStruct(true, ct.starttime) + nbase::TimeDelta::FromMinutes(classtime);
		ct.endtime = endtime.ToTimeStruct(true);
	}

	ClassItem* item = new ClassItem();
	ui::GlobalManager::FillBoxWithCache(item, L"setting/classitem.xml");
	
	item->InitSubControls(sum + 1, ct);
	listbox->Add(item);
}
