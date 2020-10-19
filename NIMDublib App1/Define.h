#pragma once

#define USERAGENT ("Mozilla/5.0 (iPad; CPU OS 13_3_1 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Mobile/15E148 ChaoXingStudy/ChaoXingStudy_3_4.3.2_ios_phone_201911291130_27 (@Kalimdor)_11391565702936108810")
#define PROFILENAME (_T("./Config.ini"))

enum ThreadId
{
	kThreadUI,
	kThreadGlobalMisc
};

enum class Week {
	Sunday,
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday
};

class CourseInfo : public ui::UserDataBase
{
public:
	wstring name = L"";
	wstring classid = L"";
	wstring courceid = L"";
	Week week = Week::Sunday;
	int start = 0;
	int end = 0;
	bool IsAutoSign = false;
	bool IsAutoAnswer = false;
	bool IsMultiSign = false;
	bool IsNotFirstSign = false;
	CourseInfo() {}
	CourseInfo(const CourseInfo& other)
	{
		this->name = other.name;
		this->classid = other.classid;
		this->courceid = other.courceid;
		this->week = other.week;
		this->start = other.start;
		this->end = other.end;
	}
};

class CourseTime 
{
public:
	nbase::Time::TimeStruct starttime;
	nbase::Time::TimeStruct endtime;
};