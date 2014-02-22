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
	bool processStep(unsigned int step);
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
	
	for (int i = 1; i <= 5; i ++)
	{
		if (!processStep(i))
		{
			return false;
		}
	}
	return true;
}

std::string Robber::GetStatusText()
{
	Sleep(100);
	char szBuf[2048];
	LONG lResult;
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
	while (GetStatusText().find(string2char(searchSuccessStr)) == std::string::npos) {
		Sleep(200);
		if (GetStatusText().find("fail") != std::string::npos) return false;
		if (tried ++ > 9) return false;
	}
	log(string2char(successStr));
	return true;
}