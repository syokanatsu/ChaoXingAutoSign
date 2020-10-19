#include "BasicForm.h"
#include "LoginForm.h"
#include "about_form.h"
#include "MainWindow.h"

const std::wstring BasicForm::kClassName = L"Basic";
log4cpp::Category& root = log4cpp::Category::getRoot();
log4cpp::Category& sub2 = log4cpp::Category::getInstance(std::string("sub2"));

BasicForm::BasicForm():
	notQuit(false),
	spytimesl(0),
	nsuccesstimes(0)
{
}

BasicForm::~BasicForm()
{
}

std::wstring BasicForm::GetSkinFolder()
{
	return L"basic";
}

std::wstring BasicForm::GetSkinFile()
{
	return L"basic.xml";
}

std::wstring BasicForm::GetWindowClassName() const
{
	return kClassName;
}

std::wstring BasicForm::GetWindowId() const
{
	return kClassName;
}

void BasicForm::InitWindow()
{
	this->SetTaskbarTitle(L"�����Զ�ǩ��");
	// ��ʼ���ؼ�ָ��

	delaysld = static_cast<ui::Slider*>(FindControl(L"delaysld"));
	delaytxt = static_cast<ui::RichEdit*>(FindControl(L"delaytxt"));
	courseslist = static_cast<ui::ListBox*>(FindControl(L"courseslist"));
	selectlist = static_cast<ui::ListBox*>(FindControl(L"selectlist"));
	addbtn = static_cast<ui::Button*>(FindControl(L"addbtn"));
	removebtn = static_cast<ui::Button*>(FindControl(L"removebtn"));
	spystatustxt = static_cast<ui::Label*>(FindControl(L"spystatustxt"));
	startbtn = static_cast<ui::Button*>(FindControl(L"startbtn"));
	endbtn = static_cast<ui::Button*>(FindControl(L"endbtn"));
	multitimeschk = static_cast<ui::CheckBox*>(FindControl(L"multitimeschk"));
	notfirstsignchk = static_cast<ui::CheckBox*>(FindControl(L"notfirstsignchk"));
	settings = static_cast<ui::Button*>(FindControl(L"settings"));
	ranktxt = static_cast<ui::RichEdit*>(FindControl(L"ranktxt"));

	spyontimes = static_cast<ui::Label*>(FindControl(L"spyontimes"));
	successtimes = static_cast<ui::Label*>(FindControl(L"successtimes"));

	///////////////////////////////
	// ��ʼ���ӳ�ʱ��
	delaytime = GetPrivateProfileInt(L"config", L"delay", 60, (nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str());
	delaysld->SetValue(delaytime);
	delaytxt->SetText(nbase::StringPrintf(L"%d", delaytime));

	nbase::ThreadManager::PostRepeatedTask(kThreadUI, ToWeakCallback([this]() {

		// ͬ�� Slider --> RichEdit
		int value = static_cast<int>(delaysld->GetValue());
		if (value != delaytime) {
			delaytxt->SetText(to_wstring(value));
			delaytime = value;
			WritePrivateProfileString(L"config", L"delay", nbase::StringPrintf(L"%d", value).c_str(),
				(nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str());
		}

		// ͬ�� RichEdit --> Slider
		auto txt = delaytxt->GetText();
		auto minvalue = delaysld->GetMinValue();
		auto tvalue = _wtoi(txt.c_str());
		if (tvalue != delaytime && tvalue >= minvalue) {
			delaysld->SetValue(static_cast<double>(tvalue));
			delaytime = tvalue;
		}
	}), nbase::TimeDelta::FromMilliseconds(200));

	// ������������Խ��
	delaytxt->AttachKillFocus(nbase::Bind([this](ui::EventArgs* e) -> bool {
		auto txt = static_cast<ui::RichEdit*>(e->pSender)->GetText();
		auto minvalue = delaysld->GetMinValue();
		auto maxvalue = delaysld->GetMaxValue();
		auto value = _wtoi(txt.c_str());
		if (value < minvalue) {
			value = minvalue;
			static_cast<ui::RichEdit*>(e->pSender)->SetText(to_wstring(minvalue));
		}
		else if (value > maxvalue) {
			value = maxvalue;
			static_cast<ui::RichEdit*>(e->pSender)->SetText(to_wstring(maxvalue));
		}
		WritePrivateProfileString(L"config", L"delay", nbase::StringPrintf(L"%d", value).c_str(),
			(nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str());
		return true;
	}, std::placeholders::_1));
	
	// ��ȡ�γ�
	nbase::ThreadManager::PostTask(kThreadGlobalMisc, nbase::Bind([this]() {
		int n = GetCourses();
		nbase::ThreadManager::PostDelayedTask(kThreadUI, nbase::Bind([this](int n) {
			nim_comp::Toast::ShowToast(nbase::StringPrintf(L"����ȡ��%d�ڿγ�", n), 1000, this->GetHWND());
		}, n), nbase::TimeDelta::FromMilliseconds(1000));
		// ��ӵ�list��
		nbase::ThreadManager::PostTask(kThreadUI, nbase::Bind([this]() {
			for (auto course : courses) {
				ui::ListContainerElement* element = new ui::ListContainerElement();
				element->SetClass(L"listitem");
				element->SetFixedHeight(20);
				element->SetText(TextUtfEncoding::FromUtf8(course.name));
				CourseInfo* database = new CourseInfo(course);
				element->SetUserDataBase(database);
				element->AttachDoubleClick(ToWeakCallback([this](ui::EventArgs* e)->bool {
					// ˫��ȫ���γ��б���¼�
					// �жϿγ��Ƿ��Ѵ���
					if (IsCourseExistInSelectList(static_cast<ui::ListContainerElement*>(e->pSender))) return true;

					ui::ListContainerElement* element = new ui::ListContainerElement();
					element->SetClass(L"listitem");
					element->SetFixedHeight(20);
					element->SetText(static_cast<ui::ListContainerElement*>(e->pSender)->GetText());
					auto database = new CourseInfo(*static_cast<CourseInfo*>(
						static_cast<ui::ListContainerElement*>(e->pSender)->GetUserDataBase()));
					element->SetUserDataBase(database);
					element->AttachDoubleClick(ToWeakCallback([this](ui::EventArgs* e)->bool {
						// ˫����ѡ��γ��б���¼�
						auto element = static_cast<ui::ListContainerElement*>(e->pSender);
						auto courseinfo = static_cast<CourseInfo*>(element->GetUserDataBase());
						/*nim_comp::Toast::ShowToast(nbase::StringPrintf(L"ѡ��Ŀγ����ƣ�%s\n�γ�ID��%s\n�༶ID��%s",
							TextUtfEncoding::FromUtf8(courseinfo->name).c_str(),
							TextUtfEncoding::FromUtf8(courseinfo->courceid).c_str(),
							TextUtfEncoding::FromUtf8(courseinfo->classid).c_str()), 1000, GetHWND());*/
						static_cast<ui::ListBox*>(element->GetOwner())->Remove(element);
						return true;
						}));
					selectlist->Add(element);
					return true;
					}));
				courseslist->Add(element);
			}
		}));
	}));
	
	// ��Ӱ�ťClick����Ӧ
	addbtn->AttachClick(ToWeakCallback([this](ui::EventArgs* e)->bool {
		auto sel = courseslist->GetCurSel();
		if (sel == -1)return true;
		auto elementsel = static_cast<ui::ListContainerElement*>(
			courseslist->GetItemAt(sel));
		// �ж��Ƿ�γ��Ѵ���
		if (IsCourseExistInSelectList(elementsel))return true;

		ui::ListContainerElement* element = new ui::ListContainerElement();
		element->SetClass(L"listitem");
		element->SetFixedHeight(20);
		element->SetText(elementsel->GetText());
		auto database = new CourseInfo(*static_cast<CourseInfo*>(elementsel->GetUserDataBase()));
		element->SetUserDataBase(database);
		element->AttachDoubleClick(ToWeakCallback([this](ui::EventArgs* e)->bool {
			// ˫����ѡ��γ��б���¼�
			auto element = static_cast<ui::ListContainerElement*>(e->pSender);
			static_cast<ui::ListBox*>(element->GetOwner())->Remove(element);
			return true;
			}));
		selectlist->Add(element);
		return true;
	}));

	// ɾ����ťClick����Ӧ
	removebtn->AttachClick(ToWeakCallback([this](ui::EventArgs* e)->bool {
		auto sel = selectlist->GetCurSel();
		if (sel == -1)return true;
		selectlist->RemoveAt(sel);
		return true;
	}));

	//// ȫ���γ��б����¼���
	//courseslist->AttachAllEvents(ToWeakCallback([this](ui::EventArgs* e)->bool {

	//	return true;
	//}));

	//// ��ѡ��γ��б����¼���
	//selectlist->AttachAllEvents(ToWeakCallback([this](ui::EventArgs* e)->bool {

	//	return true;
	//}));

	// ��ʼ��ťClick����Ӧ
	startbtn->AttachClick(ToWeakCallback([this](ui::EventArgs* e)->bool {
		if (selectlist->GetCount() == 0) {
			nim_comp::Toast::ShowToast(L"δѡ��γ�", 1000, GetHWND());
			return true;
		}

		startbtn->SetEnabled(false);
		endbtn->SetEnabled(true);
		selectlist->SetEnabled(false);
		courseslist->SetEnabled(false);
		delaysld->SetEnabled(false);
		delaytxt->SetEnabled(false);
		addbtn->SetEnabled(false);
		removebtn->SetEnabled(false);
		multitimeschk->SetEnabled(false);
		notfirstsignchk->SetEnabled(false);
		ranktxt->SetEnabled(false);
		spystatustxt->SetText(L"���ڼ���");
		auto timer = ui::TimerManager::GetInstance();
		timer->AddCancelableTimer(timerflag.GetWeakFlag(), [this]() {
			// ǩ������
			spyontimes->SetText(nbase::StringPrintf(L"%d", ++spytimesl));
			for (size_t i = 0; i < selectlist->GetCount(); i++)
			{
				auto info = static_cast<CourseInfo*>(selectlist->GetItemAt(i)->GetUserDataBase());
				auto activities = GetActivities(info->courceid, info->classid);
				sub2.info("��ѯ���γ̡�%s���� %d ��", info->name, activities.size());
				for (auto active : activities)
				{
					if (active.status == 1 && active.activeType == "2" && active.attendNum == 0)
					{
						// ��ʱǩ��
						nbase::ThreadManager::PostTask(kThreadGlobalMisc, 
							nbase::Bind(&BasicForm::DelaySign, this, active, 0UL));
					}
				}
			}
		}, static_cast<int>(delaysld->GetValue()) * 1000, ui::TimerManager::REPEAT_FOREVER);
		return true;
	}));

	// ������ťClick����Ӧ
	endbtn->AttachClick(ToWeakCallback([this](ui::EventArgs* e)->bool {
		startbtn->SetEnabled(true);
		endbtn->SetEnabled(false);
		selectlist->SetEnabled(true);
		courseslist->SetEnabled(true);
		delaysld->SetEnabled(true);
		delaytxt->SetEnabled(true);
		addbtn->SetEnabled(true);
		removebtn->SetEnabled(true);
		multitimeschk->SetEnabled(true);
		notfirstsignchk->SetEnabled(true);
		ranktxt->SetEnabled(true);
		spystatustxt->SetText(L"�ȴ�����");
		timerflag.Cancel();
		return true;
	}));

	// ���ò˵���ť��Ӧ�¼�
	settings->AttachClick([this](ui::EventArgs* args) {
		RECT rect = args->pSender->GetPos();
		ui::CPoint point;
		point.x = rect.left - 175;
		point.y = rect.top + 10;
		::ClientToScreen(m_hWnd, &point);

		nim_comp::CMenuWnd* sub_menu = new nim_comp::CMenuWnd(NULL);
		ui::STRINGorID xml(L"settings_menu.xml");
		sub_menu->Init(xml, _T("xml"), point);

		//���� menu 
		auto menu_setting = static_cast<nim_comp::CMenuElementUI*>(sub_menu->FindControl(L"setting"));
		menu_setting->AttachClick([this](ui::EventArgs* args) -> bool {
			MainWindow* mainwindow = (MainWindow*)(nim_comp::WindowsManager::GetInstance()->GetWindow(MainWindow::kClassName, MainWindow::kClassName));
			if (!mainwindow)
			{
				mainwindow = new MainWindow();
				mainwindow->Create(GetHWND(), MainWindow::kClassName, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
				mainwindow->CenterWindow();
				mainwindow->ShowWindow();
			}
			else {
				mainwindow->ActiveWindow();
			}
			return true;
		});

		/* About menu */
		nim_comp::CMenuElementUI* menu_about = static_cast<nim_comp::CMenuElementUI*>(sub_menu->FindControl(L"about"));
		menu_about->AttachClick([this](ui::EventArgs* args) {
			AboutForm* about_form = (AboutForm*)(nim_comp::WindowsManager::GetInstance()->GetWindow(AboutForm::kClassName, AboutForm::kClassName));
			if (!about_form)
			{
				about_form = new AboutForm();
				about_form->Create(GetHWND(), AboutForm::kClassName, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
				about_form->CenterWindow();
				about_form->ShowWindow();
			}
			else
			{
				about_form->ActiveWindow();
			}
			return true;
		});

		/* ע�� menu */
		auto menu_signout = static_cast<nim_comp::CMenuElementUI*>(sub_menu->FindControl(L"signout"));
		menu_signout->AttachClick(ToWeakCallback([this](ui::EventArgs* args)->bool {
			notQuit = true;	// ����������
			Close();
			nbase::ThreadManager::PostDelayedTask(kThreadUI, nbase::Bind([this]() {
				LoginForm* loginwnd = new LoginForm();
				loginwnd->Create(NULL, LoginForm::kClassName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
				loginwnd->CenterWindow();
				loginwnd->ShowWindow();
			}), nbase::TimeDelta::FromMilliseconds(200));
			return true;
		}));
		return true;
	});

	// ��ʼ��checkbox
	multitimeschk->Selected(GetPrivateProfileInt(L"config", L"multitimes", 1,
		(nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str()) == 1);
	notfirstsignchk->Selected(GetPrivateProfileInt(L"config", L"notfirstsign", 1,
		(nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str()) == 1);
	multitimeschk->AttachSelect([this](ui::EventArgs* args)->bool {
		WritePrivateProfileString(L"config", L"multitimes", L"1", (nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str());
		return true;
	});
	multitimeschk->AttachUnSelect([this](ui::EventArgs* args)->bool {
		WritePrivateProfileString(L"config", L"multitimes", L"0", (nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str());
		return true;
	});
	notfirstsignchk->AttachSelect([this](ui::EventArgs* args)->bool {
		WritePrivateProfileString(L"config", L"notfirstsign", L"1", (nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str());
		return true;
	});
	notfirstsignchk->AttachUnSelect([this](ui::EventArgs* args)->bool {
		WritePrivateProfileString(L"config", L"notfirstsign", L"0", (nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str());
		return true;
	});

	// ǩ������
	ranktxt->SetText(nbase::StringPrintf(L"%d", GetPrivateProfileInt(
		L"config", L"signrank", 10, 
		(nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str())));
	ranktxt->AttachTextChange(ToWeakCallback([this](ui::EventArgs* args)->bool {
		auto txt = ranktxt->GetText();
		if (txt[0] == '0') {
			ranktxt->SetText(txt.substr(1));
		}
		return true;
	}));
	ranktxt->AttachKillFocus(ToWeakCallback([this](ui::EventArgs* args)->bool {
		auto txt = ranktxt->GetText();
		if (txt.empty()) { txt = L"10"; ranktxt->SetText(txt); }
		WritePrivateProfileString(L"config", L"signrank", ranktxt->GetText().c_str(), (nbase::win32::GetCurrentModuleDirectory() + PROFILENAME).c_str());
		return true;
	}));
}

LRESULT BasicForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!notQuit)
		PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}


bool BasicForm::IsCourseExistInSelectList(ui::ListContainerElement* element)
{
	for (auto i = 0; i < selectlist->GetCount(); ++i) {
		if (static_cast<CourseInfo*>(element->GetUserDataBase())->courceid ==
			static_cast<CourseInfo*>(selectlist->GetItemAt(i)->GetUserDataBase())->courceid)
		{
			wstring name = TextUtfEncoding::FromUtf8(static_cast<CourseInfo*>(
				element->GetUserDataBase())->name);
			nim_comp::Toast::ShowToast(nbase::StringPrintf(
				L"�γ� %s �ѱ����\n�������ظ���ӣ�",
				name.c_str()), 1000, GetHWND());
			return true;
		}
	}
	return false;
}

int BasicForm::GetCourses()
{
	int n = 0;
	cpr::Url url{ "http://mooc1-api.chaoxing.com/mycourse/backclazzdata?view=json&rss=1" };
	cpr::Header header = { {"User-Agent",USERAGENT},{"Cookie",LoginForm::cookies} };
	auto res = cpr::Get(url, header);
	Value root;
	Reader reader;
	reader.parse(res.text, root);
	if (root["result"].asBool()) {
		courses.clear();
		for (auto c : root["channelList"]) {
			if (c["content"]["roletype"].asInt() != 3)continue;
			CourseInfo course;
			course.name = c["content"]["course"]["data"][(unsigned int)0]["name"].asString();
			course.classid = to_string(c["content"]["id"].asInt());
			course.courceid = to_string(c["content"]["course"]["data"][(unsigned int)0]["id"].asInt());
			courses.push_back(course);
			n++;
		}
	}
	return n;
}

int BasicForm::GetSignCount(string courseId)
{
	cpr::Url url{ "https://mobilelearn.chaoxing.com/widget/sign/pcTeaSignController/getCount" };
	cpr::Header header = { {"User-Agent",USERAGENT} };
	cpr::Parameters params = { { "activeId",courseId},{"appType","15"} };
	auto res = cpr::Get(url, header, params, cpr::VerifySsl(false));
	if (res.status_code != 200)return -1;
	Value root;
	Reader reader;
	reader.parse(res.text, root);
	if (root["result"].asInt() == 1) {
		auto yiqian = root["data"]["yiqian"].asInt();
		return yiqian;
	}
	return -1;
}

vector<BasicForm::ActiveInfo> BasicForm::GetActivities(string courseid, string classid)
{
	vector<ActiveInfo> rev;
	cpr::Url url{ "https://mobilelearn.chaoxing.com/ppt/activeAPI/taskactivelist" };
	cpr::Header header = { {"User-Agent", USERAGENT},{"Cookie", LoginForm::cookies} };
	cpr::Parameters params = {
			{"courseId",courseid},
			{"classId",classid},
			{"uid",LoginForm::uid}
	};
	auto res = cpr::Get(url, header, params, cpr::VerifySsl(false));
	if (res.status_code != 200) {
		sub2.error("��ȡ��б�ʧ��");
		nim_comp::Toast::ShowToast(L"��ȡ�γ̻�б�ʧ�ܣ�", 0, GetHWND());
		return rev;
	}

	//nbase::WriteFile(L"res.txt", res.text);
	Value root;
	Reader reader;
	reader.parse(res.text, root);
	for (auto ac : root["activeList"])
	{
		ActiveInfo ai;
		if (ac.isMember("status"))
			ai.status = ac["status"].asInt();
		if (ac.isMember("activeType"))
			ai.activeType = ac["activeType"].asString();
		if (ac.isMember("attendNum"))
			ai.attendNum = ac["attendNum"].asInt();
		if (ac.isMember("url")) {
			ai.url = ac["url"].asString();
			int pos = ai.url.find("activePrimaryId=");
			if (pos != string::npos) {
				pos += 16;
				int endpos = ai.url.find_first_of('&', pos);
				ai.id = ai.url.substr(pos, endpos - pos);
				rev.push_back(ai);
			}
		}
	}
	return rev;
}

bool BasicForm::ActivitySign(BasicForm::ActiveInfo ai)
{
	cpr::Url url{ "https://mobilelearn.chaoxing.com/pptSign/stuSignajax" };
	cpr::Parameters params{
		{"activeId",ai.id},
		{"uid",LoginForm::uid},
		{"clientip",""},
		{"latitude","-1"},
		{"longitude","-1"},
		{"appType","15"},
		{"fid","0"}
	};
	cpr::Header header = { {"User-Agent",USERAGENT},{"Cookie",LoginForm::cookies} };
	auto res = cpr::Get(url, params, header, cpr::VerifySsl(false));
	if (res.status_code != 200) {
		nim_comp::Toast::ShowToast(L"�γ�ǩ��ʧ�ܣ�", 0, GetHWND());
		return false;
	}
	if (res.text == "success") {
		successtimes->SetText(nbase::StringPrintf(L"%d", ++nsuccesstimes));
		return true;
	}
	else
		nim_comp::Toast::ShowToast(TextUtfEncoding::FromUtf8(res.text), 2000, GetHWND());
	return false;
}

void BasicForm::DelaySign(BasicForm::ActiveInfo ai, ULONG times)
{
	if (startbtn->IsEnabled()) {
		return;
	}
	// ����ӳ�300�Σ�����5����
	if (times >= 300) { 
		bool re = this->ActivitySign(ai); 
		sub2.info("�ǩ�� %s , ����ơ�%s��", re ? "�ɹ�" : "ʧ��", ai.name);
		return; 
	}
	auto n = GetSignCount(ai.id);
	if ((n >= atoi(TextUtfEncoding::ToUtf8(ranktxt->GetText()).c_str()) - 1)
		|| !notfirstsignchk->IsSelected()) {
		bool re = this->ActivitySign(ai);
		sub2.info("�ǩ�� %s , ����ơ�%s��", re ? "�ɹ�" : "ʧ��", ai.name);
		return;
	}
	nbase::ThreadManager::PostDelayedTask(kThreadGlobalMisc, nbase::Bind(&BasicForm::DelaySign, this, ai, ++times),
		nbase::TimeDelta::FromMilliseconds(1000));
}
