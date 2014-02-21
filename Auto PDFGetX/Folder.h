#include "stdafx.h"
#include "log.h"  
#include <iostream>  
#include <ios>
#include <fstream>
#include "windows.h"  
#include <string.h>  
#include <Strsafe.h>  

using namespace std;
using namespace console_log;

class Folder
{
public:
	Folder();
	~Folder();
	bool selectFolder();


private:
	char * file_list = "file_list.txt";
	void TraverseDirectory(wchar_t Dir[MAX_PATH]);

};

Folder::Folder()
{
	locale loc("chs");                // chinese support
	wcout.imbue(loc);

	ofstream tmp(file_list);
}

Folder::~Folder()
{
}

bool Folder::selectFolder()
{
	TraverseDirectory(L"C:\\Windows\\SysWOW64\\1033");
	return true;
}

void Folder::TraverseDirectory(wchar_t Dir[MAX_PATH])
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	wchar_t DirSpec[MAX_PATH];                
	DWORD dwError;
	StringCchCopy(DirSpec, MAX_PATH, Dir);
	StringCchCat(DirSpec, MAX_PATH, TEXT("\\*"));   

	hFind = FindFirstFile(DirSpec, &FindFileData);          //find first file  

	if (hFind == INVALID_HANDLE_VALUE)                               //error   
	{
		FindClose(hFind);
		return;
	}
	else
	{
		while (FindNextFile(hFind, &FindFileData) != 0)                            //if folder exsits
		{
			if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0 && wcscmp(FindFileData.cFileName, L".") == 0 || wcscmp(FindFileData.cFileName, L"..") == 0)        //判断是文件夹&&表示为"."||表示为"."  
			{
				continue;
			}
			if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0)      //if folder  
			{
				wchar_t DirAdd[MAX_PATH];
				StringCchCopy(DirAdd, MAX_PATH, Dir);
				StringCchCat(DirAdd, MAX_PATH, TEXT("\\"));
				StringCchCat(DirAdd, MAX_PATH, FindFileData.cFileName);       //absolute path 
				TraverseDirectory(DirAdd);                                  //recursion
			}
			if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) == 0)    // if not folder
			{
				std::wofstream fl; 
				fl.open(file_list, std::ios_base::app | std::ios_base::out);
				wcout << Dir << "\\" << FindFileData.cFileName << " found." << endl;
				fl << Dir << "\\" << FindFileData.cFileName << endl;
				fl.close();
			}
		}
		FindClose(hFind);
	}
}

