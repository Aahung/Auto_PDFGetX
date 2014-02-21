// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
#include <iostream>
#include <windows.h>
#include <vector>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "log.h"
#include "Robber.h"
#include "Stealer.h"
#include "Folder.h"

#include <fstream>
#include <sstream>


using namespace console_log;

Stealer aStealer;
Folder aFolder;
HHOOK MouseHook;
HWND hwnd;

#define IDC_MAIN_BUTTON 101
#define IDC_GENERATE_FILE_LIST_BUTTON 102
#define IDC_LOAD_FILE_LIST_BUTTON 103

HWND hPDFGetXWnd;
std::vector<std::string> file_paths;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam){
        PKBDLLHOOKSTRUCT k = (PKBDLLHOOKSTRUCT)(lParam);
        POINT p;
        if(wParam == WM_RBUTTONDOWN)
        { 
			// right button clicked 
			aStealer.stealHandleByPoint(p);
        }
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{

	initializeLog();

    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"Auto PDFGetX";
    
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    hwnd = CreateWindowEx(
        0,                                  // Optional window styles.
        CLASS_NAME,                         // Window class
        L"Auto PDFGetX",    // Window text
        WS_OVERLAPPEDWINDOW,                // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 315, 160,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

	MouseHook = SetWindowsHookEx(WH_MOUSE_LL,MouseHookProc,hInstance,0);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
	case WM_CREATE:
		{
					  
			aStealer = Stealer();
			log("Please select chi files.");
			log("----------------------------\r\nWelcome to use \"Auto PDFGetX\", \r\nPlease open the PDFGetX window and right click on the following:\r\nSample: button, \r\nReset Data button, \r\nGet I(Q) button, \r\nCalc Correction button, \r\nGet S(Q) button, \r\nGet G(Q) button. \r\n");

			hPDFGetXWnd = FindWindow(NULL, L"PDFgetX2 v1.0 build 20050312");
			if (!hPDFGetXWnd)
			{
				//MessageBox(hwnd, L"Please open PDFgetX2 v1.0 build 20050312 first.", L"Warning", MB_OK);
				//PostQuitMessage(0);  3442 2752 lijing ¿ÓΩı
				//return 0;
			}
			HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);
			HWND hWndButton=CreateWindowEx(NULL,
				L"BUTTON",
				L"Start",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON,
				100,
				20,
				100,
				24,
				hwnd,
				(HMENU)IDC_MAIN_BUTTON,
				GetModuleHandle(NULL),
				NULL
			);
			SendMessage(hWndButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0)
			);
			HWND hWndGenerateButton = CreateWindowEx(NULL,
				L"BUTTON",
				L"Generate file list",
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD | BS_DEFPUSHBUTTON,
				100,
				50,
				100,
				24,
				hwnd,
				(HMENU)IDC_GENERATE_FILE_LIST_BUTTON,
				GetModuleHandle(NULL),
				NULL
				);
			SendMessage(hWndGenerateButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE, 0)
				);
			HWND hWndLoadingButton = CreateWindowEx(NULL,
				L"BUTTON",
				L"Load file list",
				WS_TABSTOP | WS_VISIBLE |
				WS_CHILD | BS_DEFPUSHBUTTON,
				100,
				80,
				100,
				24,
				hwnd,
				(HMENU)IDC_LOAD_FILE_LIST_BUTTON,
				GetModuleHandle(NULL),
				NULL
				);
			SendMessage(hWndLoadingButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE, 0)
				);
		}
		return 0;
	case WM_COMMAND:
			switch(LOWORD(wParam))
            {
				case IDC_MAIN_BUTTON:
				{
					if (aStealer.allHandleStolen())
					{
						Robber aRobber = Robber(aStealer.getSampleBtnHandle(), aStealer.getResetBtnHandle(), aStealer.getGetIBtnHandle(), 
							aStealer.getCalCBtnHandle(), aStealer.getGetSBtnHandle(), aStealer.getGetGBtnHandle(), 
							aStealer.getStatusTextHandle());
						for (int i = 0; true; i ++)
						{
							if (i == file_paths.size())
							{
								break;
							}
							if (aRobber.process(file_paths[i]))
							{
								std::cout << " finished" << std::endl;
							
							} else {
								file_paths.push_back(file_paths[i]);
								std::cout << " failed" << std::endl;
							}
							std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
						}
						::MessageBox(hwnd, L"Finished~.", L"Success", MB_OK);
					}
					else 
					{
						::MessageBox(hwnd, L"Please make sure all controls has been marked.", L"Warning", MB_OK);
					}
				}
				return 0;
				case IDC_GENERATE_FILE_LIST_BUTTON:
					aFolder = Folder();
					aFolder.selectFolder();
					break;
				case IDC_LOAD_FILE_LIST_BUTTON:
					std::ifstream fl("file_list.txt");
					if (!fl) {
						::MessageBox(hwnd, L"Please put file list with this program", L"Warning", MB_OK);
					}
					std::string line;
					while (std::getline(fl, line))
					{
						file_paths.push_back(line);
					}
					::MessageBox(hwnd, L"Loaded file list", L"Warning", MB_OK);
					break;
			}
			return 0;
    case WM_DESTROY:
		UnhookWindowsHookEx(MouseHook);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

 