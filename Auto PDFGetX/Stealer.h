#include <iostream>
#include <windows.h>
#include <vector>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "log.h"
using namespace console_log;

class Stealer
{
public:
	Stealer();
	~Stealer();
	void stealHandleByPoint(POINT &p);
	bool allHandleStolen();
	HWND getSampleBtnHandle();
	HWND getResetBtnHandle();
	HWND getGetIBtnHandle();
	HWND getCalCBtnHandle();
	HWND getGetSBtnHandle();
	HWND getGetGBtnHandle();
	HWND getStatusTextHandle();

private:
	HWND hSampleBtn, hResetBtn, hGetIBtn, hCalCBtn, hGetSBtn, hGetGBtn, hStatusText;
};

Stealer::Stealer()
{
}

Stealer::~Stealer()
{
}

void Stealer::stealHandleByPoint(POINT &p)
{
	GetCursorPos(&p);
	HWND targetHWnd = WindowFromPoint(p);
	char szBuf[2048];
	LONG lResult;
	lResult = SendMessageA(targetHWnd, WM_GETTEXT, sizeof(szBuf) / sizeof(szBuf[0]), (LPARAM)szBuf);
	std::string text(szBuf);
	if (text.find("Sample:") != std::string::npos)
	{
		hSampleBtn = targetHWnd;
		log("Sample button got.");
	}
	else if (text.find("Reset Data") != std::string::npos)
	{
		hResetBtn = targetHWnd;
		log("Reset button got.");
	}
	else if (text.find("Get I") != std::string::npos)
	{
		hGetIBtn = targetHWnd;
		log("Get I button got.");
	}
	else if (text.find("Calc Correction") != std::string::npos)
	{
		hCalCBtn = targetHWnd;
		log("Calc Correction button got.");
	}
	else if (text.find("Get S") != std::string::npos)
	{
		hGetSBtn = targetHWnd;
		log("Get S button got.");
	}
	else if (text.find("Get G") != std::string::npos)
	{
		hGetGBtn = targetHWnd;
		log("Get G button got.");
	}
	else if (text.find("Ready for next") != std::string::npos || text.find("[") != std::string::npos)
	{
		hStatusText = targetHWnd;
		log("Status Box spied.");
	}
	else {
		return;
	}
	if (text.find(":-)") == std::string::npos){
		text += ":-)";
		::SendMessageA(targetHWnd, WM_SETTEXT, 0, (LPARAM)(text).c_str());
	}
}

bool Stealer::allHandleStolen()
{
	return hSampleBtn && hResetBtn && hGetIBtn && hCalCBtn && hGetSBtn && hGetGBtn && hStatusText;
}

HWND Stealer::getSampleBtnHandle()
{
	return hSampleBtn;
}
HWND Stealer::getResetBtnHandle()
{
	return hResetBtn;
}
HWND Stealer::getGetIBtnHandle()
{
	return hGetIBtn;
}
HWND Stealer::getCalCBtnHandle()
{
	return hCalCBtn;
}
HWND Stealer::getGetSBtnHandle()
{
	return hGetSBtn;
}
HWND Stealer::getGetGBtnHandle()
{
	return hGetGBtn;
}
HWND Stealer::getStatusTextHandle()
{
	return hStatusText;
}