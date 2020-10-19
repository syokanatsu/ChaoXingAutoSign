#include "TimeDialog.h"

const LPCTSTR TimeDialog::kClassName = _T("timedialog");

TimeDialog::TimeDialog(CourseTime& time)
{
	course_time_ = time;
}

TimeDialog::~TimeDialog()
{
}

std::wstring TimeDialog::GetSkinFolder()
{
	return L"timedialog";
}

std::wstring TimeDialog::GetSkinFile()
{
	return L"timedialog.xml";
}

std::wstring TimeDialog::GetWindowClassName() const
{
	return kClassName;
}

std::wstring TimeDialog::GetWindowId() const
{
	return kClassName;
}

void TimeDialog::InitWindow()
{
	m_pRoot->AttachBubbledEvent(ui::kEventClick, nbase::Bind(&TimeDialog::OnClicked, this, std::placeholders::_1));

	title_ = dynamic_cast<ui::Label*>(FindControl(L"title"));
	btn_yes_ = dynamic_cast<ui::Button*>(FindControl(L"btn_yes"));
	btn_no_ = dynamic_cast<ui::Button*>(FindControl(L"btn_no"));
	begin_hour_cmb_ = dynamic_cast<ui::Combo*>(FindControl(L"begin_hour_cmb"));
	begin_minute_cmb_ = dynamic_cast<ui::Combo*>(FindControl(L"begin_minute_cmb"));
	end_hour_cmb_ = dynamic_cast<ui::Combo*>(FindControl(L"end_hour_cmb"));
	end_minute_cmb_ = dynamic_cast<ui::Combo*>(FindControl(L"end_minute_cmb"));

	for (int i = 0; i < 24; i++)
	{
		ui::ListContainerElement* item = new ui::ListContainerElement();
		item->SetClass(L"listitem");
		item->SetFixedHeight(30);
		item->SetBkColor(L"white");
		item->SetTextPadding({ 6,0,6,0 });
		item->SetText(nbase::StringPrintf(L"%02d", i));
		begin_hour_cmb_->Add(item);
	}

	for (int i = 0; i < 60; i += 5)
	{
		ui::ListContainerElement* item = new ui::ListContainerElement();
		item->SetClass(L"listitem");
		item->SetFixedHeight(30);
		item->SetBkColor(L"white");
		item->SetTextPadding({ 6,0,6,0 });
		item->SetText(nbase::StringPrintf(L"%02d", i));
		begin_minute_cmb_->Add(item);
	}

	for (int i = 0; i < 24; i++)
	{
		ui::ListContainerElement* item = new ui::ListContainerElement();
		item->SetClass(L"listitem");
		item->SetFixedHeight(30);
		item->SetBkColor(L"white");
		item->SetTextPadding({ 6,0,6,0 });
		item->SetText(nbase::StringPrintf(L"%02d", i));
		end_hour_cmb_->Add(item);
	}

	for (int i = 0; i < 60; i += 5)
	{
		ui::ListContainerElement* item = new ui::ListContainerElement();
		item->SetClass(L"listitem");
		item->SetFixedHeight(30);
		item->SetBkColor(L"white");
		item->SetTextPadding({ 6,0,6,0 });
		item->SetText(nbase::StringPrintf(L"%02d", i));
		end_minute_cmb_->Add(item);
	}

	begin_hour_cmb_->SelectItem(course_time_.starttime.hour_);
	begin_minute_cmb_->SelectItem(course_time_.starttime.minute_ / 5);
	end_hour_cmb_->SelectItem(course_time_.endtime.hour_);
	end_minute_cmb_->SelectItem(course_time_.endtime.minute_ / 5);
}

void TimeDialog::Close(UINT nRet)
{
	// 提示框关闭之前先Enable父窗口，防止父窗口隐到后面去。
	HWND hWndParent = GetWindowOwner(m_hWnd);
	if (hWndParent)
	{
		::EnableWindow(hWndParent, TRUE);
		::SetFocus(hWndParent);
	}

	__super::Close(nRet);
}

bool TimeDialog::OnClicked(ui::EventArgs* msg)
{
	std::wstring name = msg->pSender->GetName();
	if (name == L"btn_yes")
	{
		EndTimeDlg(nim_comp::MB_YES);
	}
	else if (name == L"btn_no")
	{
		EndTimeDlg(nim_comp::MB_NO);
	}
	else if (name == L"closebtn")
	{
		EndTimeDlg(nim_comp::MB_NO);
	}
	return true;
}

void TimeDialog::SetTitle(const std::wstring& str)
{
	title_->SetText(str);
}

void TimeDialog::Show(TimeDlgCallback cb)
{
	timedialog_callback_ = cb;
	::EnableWindow(GetWindowOwner(m_hWnd), false);
	CenterWindow();
	ShowWindow(true);
}

void TimeDialog::EndTimeDlg(nim_comp::MsgBoxRet ret)
{
	this->Close(0);
	if (ret == nim_comp::MB_YES)
	{
		if (timedialog_callback_)
		{
			nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind(timedialog_callback_, course_time_));
		}
	}
}

void ShowTimeDlg(HWND hwnd, CourseTime& defaulttime, TimeDlgCallback cb, const std::wstring& title)
{
	TimeDialog* timedlg = new TimeDialog(defaulttime);
	HWND hWnd = timedlg->Create(hwnd, L"", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	if (hWnd == NULL)
		return;
	timedlg->SetTitle(title);
	timedlg->Show(cb);
}
