#include "MainWindow.h"
#include "Item.h"
#include "SettingForm.h"

const LPCTSTR MainWindow::kClassName = L"MainWindow";

MainWindow::MainWindow()
{
}

MainWindow::~MainWindow()
{
}

std::wstring MainWindow::GetSkinFolder()
{
	return L"mainwindow";
}

std::wstring MainWindow::GetSkinFile()
{
	return L"mainwindow.xml";
}

std::wstring MainWindow::GetWindowClassName() const
{
	return kClassName;
}

std::wstring MainWindow::GetWindowId() const
{
	return kClassName;
}

void MainWindow::InitWindow()
{
	SetTaskbarTitle(L"超星自动签到");

	weeklist	= dynamic_cast<ui::TabBox*>(FindControl(L"weeklist"));
	list1		= dynamic_cast<ui::ListBox*>(FindControl(L"list1"));
	list2		= dynamic_cast<ui::ListBox*>(FindControl(L"list2"));
	list3		= dynamic_cast<ui::ListBox*>(FindControl(L"list3"));
	list4		= dynamic_cast<ui::ListBox*>(FindControl(L"list4"));
	list5		= dynamic_cast<ui::ListBox*>(FindControl(L"list5"));
	list6		= dynamic_cast<ui::ListBox*>(FindControl(L"list6"));
	list7		= dynamic_cast<ui::ListBox*>(FindControl(L"list7"));

	monday		= dynamic_cast<ui::Option*>(FindControl(L"monday"));
	tuesday		= dynamic_cast<ui::Option*>(FindControl(L"tuesday"));
	wednesday	= dynamic_cast<ui::Option*>(FindControl(L"wednesday"));
	thursday	= dynamic_cast<ui::Option*>(FindControl(L"thursday"));
	friday		= dynamic_cast<ui::Option*>(FindControl(L"friday"));
	saturday	= dynamic_cast<ui::Option*>(FindControl(L"saturday"));
	sunday		= dynamic_cast<ui::Option*>(FindControl(L"sunday"));
	
	setting_wnd_btn = dynamic_cast<ui::Button*>(FindControl(L"setting_wnd_btn"));
	editbtn			= dynamic_cast<ui::Button*>(FindControl(L"editbtn"));
	addbtn			= dynamic_cast<ui::Button*>(FindControl(L"addbtn"));
	startbtn		= dynamic_cast<ui::Button*>(FindControl(L"startbtn"));
	stopbtn			= dynamic_cast<ui::Button*>(FindControl(L"stopbtn"));
	checkgroup		= dynamic_cast<ui::Box*>(FindControl(L"checkgroup"));

	disablectl		= FindControl(L"disablectl");
	autosign		= dynamic_cast<ui::CheckBox*>(FindControl(L"autosign"));
	multisign		= dynamic_cast<ui::CheckBox*>(FindControl(L"multisign"));
	autoanswer		= dynamic_cast<ui::CheckBox*>(FindControl(L"autoanswer"));
	notfirstsign	= dynamic_cast<ui::CheckBox*>(FindControl(L"notfirstsign"));

	course_name		= dynamic_cast<ui::Label*>(FindControl(L"course_name"));
	starttime		= dynamic_cast<ui::RichEdit*>(FindControl(L"starttime"));
	endtime			= dynamic_cast<ui::RichEdit*>(FindControl(L"endtime"));


	weeklist->AttachBubbledEvent(ui::kEventSelect, nbase::Bind(&MainWindow::weeklist_SelectChanged, this, std::placeholders::_1));
	setting_wnd_btn->AttachClick(nbase::Bind(&MainWindow::setting_wnd_btn_Click, this, std::placeholders::_1));
	editbtn->AttachClick(nbase::Bind(&MainWindow::editbtn_Click, this, std::placeholders::_1));
	addbtn->AttachClick(nbase::Bind(&MainWindow::addbtn_Click, this, std::placeholders::_1));
	startbtn->AttachClick(nbase::Bind(&MainWindow::startbtn_Click, this, std::placeholders::_1));
	stopbtn->AttachClick(nbase::Bind(&MainWindow::stopbtn_Click, this, std::placeholders::_1));
	checkgroup->AttachBubbledEvent(ui::kEventSelect, nbase::Bind(&MainWindow::checkgroup_Select, this, std::placeholders::_1));
	checkgroup->AttachBubbledEvent(ui::kEventUnSelect, nbase::Bind(&MainWindow::checkgroup_UnSelect, this, std::placeholders::_1));

	Disabled_checkBoxGroup();

	for (auto i = 0; i < 3; i++)
	{
		CourseItem* item = new CourseItem;
		ui::GlobalManager::FillBoxWithCache(item, L"mainwindow/item.xml");

		std::wstring title = nbase::StringPrintf(L"课程 [%02d]", i + 1);
		CourseInfo* ci = new CourseInfo();
		ci->name = title;
		ci->IsAutoAnswer = true;
		item->InitSubControls(ci);
		list1->Add(item);
	}

	// 向杂务线程发送初始化任务
	nbase::ThreadManager::PostTask(kThreadGlobalMisc, nbase::Bind(&MainWindow::InitTask, this));
}

LRESULT MainWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

bool MainWindow::setting_wnd_btn_Click(ui::EventArgs* args)
{
	RECT rect = args->pSender->GetPos();
	ui::CPoint point;
	point.x = rect.left - 175;
	point.y = rect.top + 10;
	::ClientToScreen(m_hWnd, &point);

	nim_comp::CMenuWnd* sub_menu = new nim_comp::CMenuWnd(NULL);
	ui::STRINGorID xml(L"settings_menu.xml");
	sub_menu->Init(xml, _T("xml"), point);

	//设置 menu 
	auto menu_setting = static_cast<nim_comp::CMenuElementUI*>(sub_menu->FindControl(L"setting"));
	menu_setting->AttachClick([this](ui::EventArgs* args) -> bool {
		SettingForm* setting_wnd = dynamic_cast<SettingForm*>(nim_comp::WindowsManager::GetInstance()->GetWindow(SettingForm::kClassName, SettingForm::kClassName));
		if (!setting_wnd) 
		{
			setting_wnd = new SettingForm();
			setting_wnd->Create(this->GetHWND(), SettingForm::kClassName, WS_POPUP, 0);
			setting_wnd->CenterWindow();
			setting_wnd->ShowModalFake(this->GetHWND());
		}
		else
		{
			setting_wnd->ActiveWindow();
		}
		return true;
	});
	return true;
}

bool MainWindow::editbtn_Click(ui::EventArgs* args)
{
	return true;
}

bool MainWindow::addbtn_Click(ui::EventArgs* args)
{
	return true;
}

bool MainWindow::startbtn_Click(ui::EventArgs* args)
{
	return true;
}

bool MainWindow::stopbtn_Click(ui::EventArgs* args)
{
	return true;
}

bool MainWindow::checkgroup_Select(ui::EventArgs* args)
{
	OutputDebugString(L"checkgroup_Select\n");
	auto selet = (CourseItem*)GetCourseListCurSel();
	if (args->pSender->GetName() == L"autosign")
	{
		selet->SetAutoSign(true);
	}
	else if (args->pSender->GetName() == L"autoanswer")
	{
		selet->SetAutoAnswer(true);
	}
	return true;
}

bool MainWindow::checkgroup_UnSelect(ui::EventArgs* args)
{
	OutputDebugString(L"checkgroup_UnSelect\n");
	auto selet = (CourseItem*)GetCourseListCurSel();
	if (args->pSender->GetName() == L"autosign")
	{
		selet->SetAutoSign(false);
	}
	else if (args->pSender->GetName() == L"autoanswer") 
	{
		selet->SetAutoAnswer(false);
	}
	return true;
}

bool MainWindow::weeklist_SelectChanged(ui::EventArgs* args)
{
	list<wstring> weekstring = { L"list1",L"list2",L"list3", L"list4", L"list5", L"list6", L"list7" };
	int sel = args->wParam;
	int oldSel = args->lParam;
	OutputDebugString(nbase::StringPrintf(L"Sender: %s\nsel: %d\noldSel: %d\n", args->pSender->GetName().c_str(), sel, oldSel).c_str());
	if (std::find(weekstring.begin(), weekstring.end(), args->pSender->GetName()) != weekstring.end())
	{
		ui::ListContainerElement* selet = dynamic_cast<ui::ListContainerElement*>((dynamic_cast<ui::ListBox*>(args->pSender))->GetItemAt(sel));
		Enabled_checkBoxGroup(dynamic_cast<CourseInfo&>(*selet->GetUserDataBase()));
	}
	return true;
}

void MainWindow::Disabled_checkBoxGroup()
{
	disablectl->SetVisible();
	autosign->Selected(false);
	autoanswer->Selected(false);
	multisign->Selected(false);
	notfirstsign->Selected(false);
	course_name->SetText(L"未选择课程");
	starttime->SetText(L"00:00");
	endtime->SetText(L"23:59");
}

void MainWindow::Enabled_checkBoxGroup(CourseInfo& info)
{
	autosign->Selected(info.IsAutoSign);
	autoanswer->Selected(info.IsAutoAnswer);
	multisign->Selected(info.IsMultiSign);
	notfirstsign->Selected(info.IsNotFirstSign);
	course_name->SetText(info.name);
	CourseTime ct = GetCourseTime(info.start, info.end);
	starttime->SetText(nbase::StringPrintf(L"%02d:%02d", ct.starttime.hour_, ct.starttime.minute_));
	endtime->SetText(nbase::StringPrintf(L"%02d:%02d", ct.endtime.hour_, ct.endtime.minute_));
	disablectl->SetVisible(false);
}

void MainWindow::InitTask()
{
	auto time = nbase::Time::Now().ToTimeStruct(true);
	auto dayofweek = time.day_of_week();
	nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(&MainWindow::SelectDayOfWeek, this, dayofweek));
}

void MainWindow::SelectDayOfWeek(int dayofweek)
{
	ui::Option* sel = NULL;
	switch (dayofweek)
	{
	case 0: sel = sunday; break;
	case 1: sel = monday; break;
	case 2: sel = tuesday; break;
	case 3: sel = wednesday; break;
	case 4: sel = thursday; break;
	case 5: sel = friday; break;
	case 6: sel = saturday; break;
	default:
		break;
	}
	if (sel)
	{
		nbase::ThreadManager::PostDelayedTask(kThreadUI, nbase::Bind([this](ui::Option* sel) {
			sel->Selected(true, true);
			sel->Invalidate();
		}, sel), nbase::TimeDelta::FromMilliseconds(100));
	}
}

CourseTime& MainWindow::GetCourseTime(int start, int end)
{
	// TODO: 在此处插入 return 语句
	CourseTime ct;
	return ct;
}

ui::ListContainerElement* MainWindow::GetCourseListCurSel()
{
	ui::ListBox* listbox = dynamic_cast<ui::ListBox*>(weeklist->GetItemAt(weeklist->GetCurSel()));
	ui::ListContainerElement* selet = dynamic_cast<ui::ListContainerElement*>(listbox->GetItemAt(listbox->GetCurSel()));

	return selet;
}
