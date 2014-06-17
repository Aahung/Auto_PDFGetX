#include <iostream>
#include <windows.h>
#include <vector>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "log.h"

#include "Robber.h"
using namespace console_log;

Robber::Robber(HWND _hSampleBtn, HWND _hSampleBackgroundBtn, HWND _hResetBtn, HWND _hGetIBtn, HWND _hCalCBtn, HWND _hGetSBtn, HWND _hGetGBtn, HWND _hAdvancedBtn, HWND _hStatusText)
{
	hSampleBtn = _hSampleBtn;
	hSampleBackgroundBtn = _hSampleBackgroundBtn;
	hResetBtn = _hResetBtn;
	hGetIBtn = _hGetIBtn;
	hCalCBtn = _hCalCBtn;
	hGetSBtn = _hGetSBtn;
	hGetGBtn = _hGetGBtn;
	hAdvancedBtn = _hAdvancedBtn;
	hStatusText = _hStatusText;
}

Robber::~Robber()
{
	
}


bool Robber::process(std::string file_path, std::string bg_file_path)
{
	std::cout << std::endl << std::endl << std::endl << std::endl;


	// load sample file
	log("loading sample file");
	SendMessage(hSampleBtn, BM_CLICK, 0, 0);
	HWND a;
	do {
		do {
			Sleep(200);
			a = ::FindWindow(NULL, L"Please select sample data file -->");
		} while (!a);
		Sleep(10);
		HWND hOpenBtn, hFilePathEdit;
		int count = 0;
		do {
			Sleep(100);
			hOpenBtn = ::FindWindowEx(a, NULL, L"Button", L"&Open");
			while (hOpenBtn == NULL) {
				Sleep(50);
				log("Fail to get \"Open\" button, try again.");
				hOpenBtn = ::FindWindowEx(a, NULL, NULL, NULL);
				hOpenBtn = ::FindWindowEx(a, hOpenBtn, NULL, NULL);
				hOpenBtn = ::FindWindowEx(a, hOpenBtn, NULL, NULL);
				hOpenBtn = ::FindWindowEx(a, hOpenBtn, NULL, NULL);
				hOpenBtn = ::FindWindowEx(a, hOpenBtn, NULL, NULL);
				hOpenBtn = ::FindWindowEx(a, hOpenBtn, NULL, NULL);
			}
			hFilePathEdit = ::FindWindowEx(a, NULL, L"ComboBoxEx32", NULL);
			hFilePathEdit = ::FindWindowEx(hFilePathEdit, NULL, L"ComboBox", NULL);
			hFilePathEdit = ::FindWindowEx(hFilePathEdit, NULL, L"Edit", NULL);
			if (++count > 5)
			{
				HWND hCancelBtn = ::FindWindowEx(a, NULL, L"Button", L"Cancel");
				SendMessage(hCancelBtn, BM_CLICK, 0, 0);
				return false;
			}
		} while (!hFilePathEdit);
		log("File path box Found.");
		std::string text;
		count = 0;
		do {
			::SendMessageA(hFilePathEdit, WM_SETTEXT, 0, (LPARAM)file_path.c_str());
			char szBuf[2048];
			LONG lResult;
			lResult = SendMessageA(hFilePathEdit, WM_GETTEXT, sizeof(szBuf) / sizeof(szBuf[0]), (LPARAM)szBuf);
			log("Trying to fill file path into box.");
			text = std::string(szBuf);
			Sleep(200);
			if (text.find(".chi") != std::string::npos)
			{
				log(string2char(file_path));
				log("Checked file path successfully filled.");
				break;
			}
			else
			{
				if (++count > 10)
				{
					log("Skip this file.");
					return false;
				}
				log("Checked file path not filled, trying again.");
			}
		} while (1);

		::SendMessage(hOpenBtn, BM_CLICK, 0, 0);
		// debug
		if (hOpenBtn == NULL) log("Open button not got.");
		// end debug
		Sleep(200);
		log("Selected file.");
	} while (::FindWindow(NULL, L"Please select sample data file -->") != NULL);



	// load bg file
	log("loading bg file");
	SendMessage(hSampleBackgroundBtn, BM_CLICK, 0, 0);
	do {
		do {
			Sleep(200);
			a = ::FindWindow(NULL, L"Please select sample background data file -->");
		} while (!a);
		Sleep(10);
		HWND hOpenBtn, hFilePathEdit;
		int count = 0;
		do {
			Sleep(100);
			hOpenBtn = ::FindWindowEx(a, NULL, L"Button", L"&Open");
			while (hOpenBtn == NULL) {
				Sleep(50);
				log("Fail to get \"Open\" button, try again.");
				hOpenBtn = ::FindWindowEx(a, NULL, NULL, NULL);
				hOpenBtn = ::FindWindowEx(a, hOpenBtn, NULL, NULL);
				hOpenBtn = ::FindWindowEx(a, hOpenBtn, NULL, NULL);
				hOpenBtn = ::FindWindowEx(a, hOpenBtn, NULL, NULL);
				hOpenBtn = ::FindWindowEx(a, hOpenBtn, NULL, NULL);
				hOpenBtn = ::FindWindowEx(a, hOpenBtn, NULL, NULL);
			}
			hFilePathEdit = ::FindWindowEx(a, NULL, L"ComboBoxEx32", NULL);
			hFilePathEdit = ::FindWindowEx(hFilePathEdit, NULL, L"ComboBox", NULL);
			hFilePathEdit = ::FindWindowEx(hFilePathEdit, NULL, L"Edit", NULL);
			if (++count > 5)
			{
				HWND hCancelBtn = ::FindWindowEx(a, NULL, L"Button", L"Cancel");
				SendMessage(hCancelBtn, BM_CLICK, 0, 0);
				return false;
			}
		} while (!hFilePathEdit);
		log("Background file path box Found.");
		std::string text;
		count = 0;
		do {
			::SendMessageA(hFilePathEdit, WM_SETTEXT, 0, (LPARAM)bg_file_path.c_str());
			char szBuf[2048];
			LONG lResult;
			lResult = SendMessageA(hFilePathEdit, WM_GETTEXT, sizeof(szBuf) / sizeof(szBuf[0]), (LPARAM)szBuf);
			log("Trying to fill bg file path into box.");
			text = std::string(szBuf);
			Sleep(200);
			if (text.find(".chi") != std::string::npos)
			{
				log(string2char(bg_file_path));
				log("Checked bg file path successfully filled.");
				break;
			}
			else
			{
				if (++count > 10)
				{
					log("Skip this file.");
					return false;
				}
				log("Checked bg file path not filled, trying again.");
			}
		} while (1);

		::SendMessage(hOpenBtn, BM_CLICK, 0, 0);
		// debug
		if (hOpenBtn == NULL) log("Open button not got.");
		// end debug
		Sleep(200);
		log("Selected file.");
	} while (::FindWindow(NULL, L"Please select sample background data file -->") != NULL);
	
	for (int i = 1; i <= 5; i ++)
	{
		if (!processStep(i))
		{
			return false;
		}
	}
	// Optimize G(r) {
	log("Trying to optimize G(r)...");
	::PostMessage(hAdvancedBtn, BM_CLICK, 0, 0);
	HWND hMenu = NULL;
	HMENU menu = NULL;
	log("Trying to get menu handle...");
	Sleep(50);
	hMenu = ::FindWindow(L"#32768", L"");
	while (hMenu == NULL) {
		log("Trying to get menu handle...");
		Sleep(50);
		hMenu = ::FindWindow(L"#32768", L"");
	} 
	::SendMessage(hMenu, BM_CLICK, 0, 0);
	menu = (HMENU)::SendMessage(hMenu, MN_GETHMENU, 0, 0);
	UINT firstBtn = GetMenuItemID(menu, 0);
	TCHAR buf[64];
	GetMenuString(menu, firstBtn, buf, 20, MF_BYCOMMAND);
	if (buf[0] == 'O' && buf[1] == 'p') {
		// save current cursor
		POINT p;
		GetCursorPos(&p);
		// get "Optimize G(r) pos
		RECT Rect;
		GetWindowRect(hMenu, &Rect);
		int x = (Rect.left + Rect.right) / 2;
		int y = Rect.top + 10;
		// set cursor and click at button
		SetCursorPos(x, y);
		mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
		mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);
		// recovery the previous cursor pos
		SetCursorPos(p.x, p.y);
		log("Optimized G(r).");
	}
	else {
		log("Fail to Optimized G(r).");
	}
	// }
	if (!processStep(5))
	{
		return false;
	}
	return true;
}

std::string Robber::GetStatusText()
{
	char szBuf[2048];
	LONG lResult;
	//lResult = SendMessageTimeoutA(hStatusText, WM_GETTEXT, sizeof(szBuf) / sizeof(szBuf[0]), (LPARAM)szBuf, 0x0000, 5000, NULL);
	lResult = SendMessageA(hStatusText, WM_GETTEXT, sizeof(szBuf) / sizeof(szBuf[0]), (LPARAM)szBuf);
	std::string text(szBuf);
	return text;
}

bool Robber::processStep(unsigned int step)
{
	std::string tryStr, searchSuccessStr, successStr;
	HWND h;
	switch (step)
	{
	case 1:
		tryStr = "Trying to reset data...";
		searchSuccessStr = "All data were reset";
		successStr = "Data reset.";
		h = hResetBtn;
		break;
	case 2:
		tryStr = "Trying to get I(q)...";
		searchSuccessStr = "Start to Get I(q) ... successfully completed";
		successStr = "I(q) got.";
		h = hGetIBtn;
		break;
	case 3:
		tryStr = "Trying to correct calculation...";
		searchSuccessStr = "Start to calculate corrections ... successfully finished";
		successStr = "Calculation corrected.";
		h = hCalCBtn;
		break;
	case 4:
		tryStr = "Trying to get S(q)...";
		searchSuccessStr = "Start to obtain S(q) ... successfully completed";
		successStr = "S(q) got.";
		h = hGetSBtn;
		break;
	case 5:
		tryStr = "Trying to get G(r)...";
		searchSuccessStr = "Start to obtain G(r) ... successfully completed";
		successStr = "G(r) got.";
		h = hGetGBtn;
		break;
	default:
		return true;
		break;
	}
	::SendMessage(h, BM_CLICK, 0, 0);
	
	log(string2char(tryStr));
	int tried = 0;
	while (tried ++ <= 5) {
		try {
			std::string statusText = GetStatusText();
			if (statusText.find(string2char(searchSuccessStr)) != std::string::npos) break;
			if (statusText.find("fail") != std::string::npos) return false;
			
		} catch(int e) {

		} 
		if (tried > 5) return false;
		Sleep(250);
	}
	log(string2char(successStr));
	return true;
}