#include <iostream>
#include <windows.h>
#include <vector>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "log.h"
using namespace console_log;

class Robber
{
public:
	Robber(HWND _hSampleBtn, HWND _hResetBtn, HWND _hGetIBtn, HWND _hCalCBtn, HWND _hGetSBtn, HWND _hGetGBtn, HWND _hStatusText);
	~Robber();
	bool process(std::string file_path);

private:
	HWND hSampleBtn, hResetBtn, hGetIBtn, hCalCBtn, hGetSBtn, hGetGBtn, hStatusText;
	std::string GetStatusText();
};

Robber::Robber(HWND _hSampleBtn, HWND _hResetBtn, HWND _hGetIBtn, HWND _hCalCBtn, HWND _hGetSBtn, HWND _hGetGBtn, HWND _hStatusText)
{
	hSampleBtn = _hSampleBtn;
	hResetBtn = _hResetBtn;
	hGetIBtn = _hGetIBtn;
	hCalCBtn = _hCalCBtn;
	hGetSBtn = _hGetSBtn;
	hGetGBtn = _hGetGBtn;
	hStatusText = _hStatusText;
}

Robber::~Robber()
{
	
}


bool Robber::process(std::string file_path)
{
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
			hFilePathEdit = ::FindWindowEx(a, NULL, L"ComboBoxEx32", NULL);
			hFilePathEdit = ::FindWindowEx(hFilePathEdit, NULL, L"ComboBox", NULL);
			hFilePathEdit = ::FindWindowEx(hFilePathEdit, NULL, L"Edit", NULL);
			if (++count > 500)
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
		Sleep(200);
		log("Selected file.");
	} while (::FindWindow(NULL, L"Please select sample data file -->") != NULL);
	::SendMessage(hResetBtn, BM_CLICK, 0, 0);
	log("Trying to reset data.");
	while (GetStatusText().find("All data were reset") == std::string::npos) {
		Sleep(200);
		if (GetStatusText().find("fail") != std::string::npos) return false;
	}
	log("Reseted data.");
	::SendMessage(hGetIBtn, BM_CLICK, 0, 0);
	log("Trying to get I(Q).");
	while (GetStatusText().find("Start to Get I(q) ... successfully completed") == std::string::npos) {
		Sleep(200);
		if (GetStatusText().find("fail") != std::string::npos) return false;
	}
	log("I(Q) got.");
	::SendMessage(hCalCBtn, BM_CLICK, 0, 0);
	log("Trying to correct calculation.");
	while (GetStatusText().find("Start to calculate corrections ... successfully finished") == std::string::npos) {
		Sleep(200);
		if (GetStatusText().find("fail") != std::string::npos) return false;
	}
	log("Calculation corrected.");
	::SendMessage(hGetSBtn, BM_CLICK, 0, 0);
	log("Trying to get S(Q).");
	while (GetStatusText().find("Start to obtain S(q) ... successfully completed") == std::string::npos) {
		Sleep(200);
		if (GetStatusText().find("fail") != std::string::npos) return false;
	}
	log("S(Q) got.");
	::SendMessage(hGetGBtn, BM_CLICK, 0, 0);
	log("Trying to get G(R).");
	while (GetStatusText().find("Start to obtain G(r) ... successfully completed") == std::string::npos) {
		Sleep(200);
		if (GetStatusText().find("fail") != std::string::npos) return false;
	}
	log("G(R) got.");
	return true;
}

std::string Robber::GetStatusText()
{
	Sleep(50);
	char szBuf[2048];
	LONG lResult;
	lResult = SendMessageA(hStatusText, WM_GETTEXT, sizeof(szBuf) / sizeof(szBuf[0]), (LPARAM)szBuf);
	std::string text(szBuf);
	return text;
}