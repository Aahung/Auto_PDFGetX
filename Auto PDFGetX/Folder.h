#include "stdafx.h"
#include "log.h"  
#include <iostream>  
#include <ios>
#include <fstream>
#include <sstream>
#include "windows.h"  
#include <string.h>  
#include <Strsafe.h>  

#include <vector>

using namespace std;
using namespace console_log;

class Folder
{
public:
	Folder();
	~Folder();
	bool selectFolder(char *);
	int loadFileList();
	std::string getNextFilePath(bool);

private:
	char * file_list;
	void TraverseDirectory(wchar_t Dir[MAX_PATH]);
	struct file {
		std::string file_path;
		int status;
	};
	std::vector< file > fileFolder;
	int index;
};

Folder::Folder()
{
	file_list = "file_list.txt";
	locale loc("chs");                // chinese support
	wcout.imbue(loc);

	ofstream tmp(file_list); //create a file
	fileFolder = std::vector< file >(0);
}

Folder::~Folder()
{
}

bool Folder::selectFolder(char * dir)
{
	wchar_t  ws[100];
	swprintf(ws, 100, L"%hs", dir);
	TraverseDirectory(ws);
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
				std::string path = ws2string(FindFileData.cFileName);
				if (path.find(".chi") != std::string::npos)
				{
					std::wofstream fl; 
					fl.open(file_list, std::ios_base::app | std::ios_base::out);
					wcout << Dir << "\\" << FindFileData.cFileName << " found." << endl;
					fl << Dir << "\\" << FindFileData.cFileName << endl;
					fl.close();
				}
			}
		}
		FindClose(hFind);
	}
}

int Folder::loadFileList()
{
	fileFolder.clear();
	std::ifstream fl("file_list.txt");
	if (!fl.is_open()) {
		return -1;
	}
	std::string _file_path;
	while (std::getline(fl, _file_path))
	{
		file aFile;
		aFile.file_path = _file_path;
		aFile.status = 0;
		fileFolder.push_back(aFile);
	}
	index = 0;
	return fileFolder.size();
}

std::string Folder::getNextFilePath(bool last_success)
{
	if (index > 0)
	{
		if (!last_success)
		{
			if (++ fileFolder[index - 1].status > 5)
			{
				fileFolder[index - 1].status = -1;
				std::string _log = fileFolder[index - 1].file_path;
				_log += " failed to be processed for 5 tries, passed.";
				log(string2char(_log));
			} else 
			{
				fileFolder.push_back(fileFolder[index - 1]);
				fileFolder.erase(fileFolder.begin() + index - 1);
				index --;
			}
		}
		else 
		{
			std::string _log = fileFolder[index - 1].file_path;
			_log += " successfully processed in ";
			_log += to_string( ++ fileFolder[index - 1].status);
			_log += " try(tries).";
			log(string2char(_log));
		}
	}
	if (index >= fileFolder.size())
	{
		return "";
	}
	return fileFolder[index ++].file_path;
}
