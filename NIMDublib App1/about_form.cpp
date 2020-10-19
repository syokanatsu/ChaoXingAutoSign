#include "framework.h"
#include "about_form.h"
#include <shellapi.h>

const LPCTSTR AboutForm::kClassName = L"About";

AboutForm::AboutForm()
{
}


AboutForm::~AboutForm()
{
}

std::wstring AboutForm::GetSkinFolder()
{
	return L"basic";
}

std::wstring AboutForm::GetSkinFile()
{
	return L"about.xml";
}

std::wstring AboutForm::GetWindowClassName() const
{
	return kClassName;
}

std::wstring AboutForm::GetWindowId() const
{
	return kClassName;
}

void AboutForm::InitWindow()
{
	
}

