#include <iostream>
#include <windows.h>
#include <vector>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "log.h"

#include "Stealer.h"

using namespace console_log;

Stealer::Stealer()
{
	hSampleBtn = NULL;
	hSampleBackgroundBtn = NULL;
	hResetBtn = NULL; 
	hGetIBtn = NULL;
	hCalCBtn = NULL;
	hGetSBtn = NULL;
	hGetGBtn = NULL; 
	hStatusText = NULL;
}

Stealer::~Stealer()
{
}

bool Stealer::stealHandleAutomatically(HWND mainWindow) {
	log("trying to get steal the button automatically...");
	HWND IDLWindow = ::FindWindowEx(mainWindow, NULL, L"IDLTLBWidget", L"IDL");
	HWND subIDLWindow = ::FindWindowEx(IDLWindow, NULL, NULL, NULL);
	subIDLWindow = ::FindWindowEx(IDLWindow, subIDLWindow, NULL, NULL);
	subIDLWindow = ::FindWindowEx(IDLWindow, subIDLWindow, NULL, NULL);
	HWND subsubIDLWindow = ::FindWindowEx(subIDLWindow, NULL, NULL, NULL);
	subsubIDLWindow = ::FindWindowEx(subIDLWindow, subsubIDLWindow, NULL, NULL);
	HWND subsubsubIDLWindow = ::FindWindowEx(subsubIDLWindow, NULL, NULL, NULL);
	subsubsubIDLWindow = ::FindWindowEx(subsubIDLWindow, subsubsubIDLWindow, NULL, NULL);
	HWND btn = ::FindWindowEx(subsubsubIDLWindow, NULL, NULL, NULL); 
	if (getTextFromHandle(btn).find("Sample:") != std::string::npos) {
		hSampleBtn = btn;
		log("Sample button got.");
	} else return false;
	// start to get bkg button
	btn = ::FindWindowEx(subsubsubIDLWindow, btn, NULL, NULL);
	btn = ::FindWindowEx(subsubsubIDLWindow, btn, NULL, NULL);
	if (getTextFromHandle(btn).find("Sam. Bkg.:") != std::string::npos) {
		hSampleBackgroundBtn = btn;
		log("Sample Background button got.");
	} else return false;
	//start to get other buttons
	subsubIDLWindow = ::FindWindowEx(subIDLWindow, subsubIDLWindow, NULL, NULL);
	subsubIDLWindow = ::FindWindowEx(subIDLWindow, subsubIDLWindow, NULL, NULL);
	subsubIDLWindow = ::FindWindowEx(subIDLWindow, subsubIDLWindow, NULL, NULL);
	subsubsubIDLWindow = ::FindWindowEx(subsubIDLWindow, NULL, NULL, NULL);
	btn = ::FindWindowEx(subsubsubIDLWindow, NULL, NULL, NULL);
	if (getTextFromHandle(btn).find("Reset Data") != std::string::npos) {
		hResetBtn = btn;
		log("Reset button got.");
	} else return false;
	//move to next button
	btn = ::FindWindowEx(subsubsubIDLWindow, btn, NULL, NULL);
	if (getTextFromHandle(btn).find("Get I") != std::string::npos) {
		hGetIBtn = btn;
		log("Get I button got.");
	} else return false;
	//move to next button
	btn = ::FindWindowEx(subsubsubIDLWindow, btn, NULL, NULL);
	if (getTextFromHandle(btn).find("Calc Correction") != std::string::npos) {
		hCalCBtn = btn;
		log("Calc Correction button got.");
	} else return false;
	//move to next button
	btn = ::FindWindowEx(subsubsubIDLWindow, btn, NULL, NULL);
	if (getTextFromHandle(btn).find("Get S") != std::string::npos) {
		hGetSBtn = btn;
		log("Get S button got.");
	} else return false;
	//move to next button
	btn = ::FindWindowEx(subsubsubIDLWindow, btn, NULL, NULL);
	if (getTextFromHandle(btn).find("Get G") != std::string::npos) {
		hGetGBtn = btn;
		log("Get G button got.");
	} else return false;
	//start to get text box
	subsubIDLWindow = ::FindWindowEx(subIDLWindow, subsubIDLWindow, NULL, NULL);
	HWND edit = ::FindWindowEx(subsubIDLWindow, NULL, NULL, NULL);
	edit = ::FindWindowEx(subsubIDLWindow, edit, L"Edit", NULL);
	if (edit != NULL) {
		hStatusText = edit;
		log("Status Box spied.");
	} else return false;
	log("All the objects under controlled.");
	log(":-)");
	return true;
}

std::string Stealer::getTextFromHandle(HWND h) {
	char szBuf[2048];
	LONG lResult;
	lResult = SendMessageA(h, WM_GETTEXT, sizeof(szBuf) / sizeof(szBuf[0]), (LPARAM)szBuf);
	std::string text(szBuf);
	return text;
}


bool Stealer::allHandleStolen()
{
	bool ready = (hSampleBtn && hSampleBackgroundBtn && hResetBtn && hGetIBtn && hCalCBtn && hGetSBtn && hGetGBtn && hStatusText);
	return ready;
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