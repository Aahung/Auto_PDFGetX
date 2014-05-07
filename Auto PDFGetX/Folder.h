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
#include <map>
#include <queue> 
#include "Utility.h"

using namespace std;
using namespace console_log;

struct file {
	std::string file_path;
	std::string bg_file_path;
	int status;
};
struct numberOfFiles {
	int sample;
	int bg;
};
class Folder
{
public:
	Folder();
	~Folder();
	bool selectFolder(char *);
	numberOfFiles loadFileList();
	file getNextFile(bool);
	void setPrefix(char *, char *);

private:
	char * file_list, * bg_file_list, * samplePrefix, * bgPrefix;
	void TraverseDirectory(wchar_t Dir[MAX_PATH]);
	std::map<std::string, file> fileFolders;
	std::queue<std::string> fileIdentities;
	bool firstFile;
};

Folder::Folder()
{
	file_list = "file_list.txt";
	locale loc("chs");                // chinese support
	wcout.imbue(loc);
	firstFile = true;
	ofstream tmp(file_list); //create a file
}

Folder::~Folder()
{
}

void Folder::setPrefix(char * _samplePrefix, char * _bgPrefix) {
	samplePrefix = _samplePrefix;
	bgPrefix = _bgPrefix;
	cout << "Sample file prefix: " << samplePrefix << std::endl;
	cout << "Background file prefix: " << bgPrefix << std::endl;
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
					locale loc("chs");                // chinese support
					fl.imbue(loc);
					fl.open(file_list, std::ios_base::app | std::ios_base::out);
					if (path.find(samplePrefix) != std::string::npos) {
						std::vector<std::string> tokens = Utility::split_str(path, samplePrefix);
						if (tokens.size() > 0) {
							tokens = Utility::split_str(tokens[0], ".chi");
							std::string identity = tokens[0];
							wcout << "sample file of " << identity.c_str() << ": " << Dir << "\\" << FindFileData.cFileName << std::endl;
							fl << "S " << identity.c_str() << " " << Dir << "\\" << FindFileData.cFileName; 
							fl << std::endl;
						}
					}
					else if (path.find(bgPrefix) != std::string::npos) {
						std::vector<std::string> tokens = Utility::split_str(path, bgPrefix);
						if (tokens.size() > 0) {
							tokens = Utility::split_str(tokens[0], ".chi");
							std::string identity = tokens[0];
							wcout << "background file of " << identity.c_str() << ": " << Dir << "\\" << FindFileData.cFileName << std::endl;
							fl << "B " << identity.c_str() << " " << Dir << "\\" << FindFileData.cFileName; 
							fl << std::endl;
						}
					}
					else {
						continue;
					}
					fl.close();
				}
			}
		}
		FindClose(hFind);
	}
}

numberOfFiles Folder::loadFileList()
{
	numberOfFiles num = {0, 0};
	//
	fileFolders.clear();
	std::ifstream fl("file_list.txt");
	if (!fl.is_open()) {
		return{ -1, -1 };
	}
	std::string file_info;
	while (std::getline(fl, file_info))
	{
		std::stringstream ss(file_info);
		char type;
		std::string identity;
		file aFile = {
			"",
			"",
			0
		};
		ss >> type >> identity;
		auto pos = fileFolders.find(identity);
		if (pos == fileFolders.end()) {
			// create if no such identity
			fileFolders[identity] = aFile;
		}
		if (type == 'S') {
			ss >> fileFolders[identity].file_path;
			num.sample++;
		}
		else if (type == 'B') {
			ss >> fileFolders[identity].bg_file_path;
			num.bg++;
		}
	}
	firstFile = true;
	// check whether sample and bg are equal
	if (num.bg != num.sample) {
		log("warning!!! sample file number and background file number not equal!!! \r\n following files have problems: ");
	}
	// move all keys to identities and print troublesome files
	for (auto i = fileFolders.begin(); i != fileFolders.end(); i++) {
		if ((*i).second.file_path == "") {
			log(string2char((*i).first + " has no samle file, will be ignored."));
		}
		else if ((*i).second.bg_file_path == "") {
			log(string2char((*i).first + " has no background file, will be ignored."));
		}
		else {
			fileIdentities.push((*i).first);
		}
	}
	return num;
}

file Folder::getNextFile(bool last_success)
{
	if (fileIdentities.size() <= 0) {
		return{"", "", NULL};
	}
	if (!firstFile) {
		if (!last_success) {
			if (++ fileFolders[fileIdentities.front()].status > 5) {
				fileFolders[fileIdentities.front()].status = -1;
				std::string _log = fileFolders[fileIdentities.front()].file_path;
				_log += " failed to be processed for 5 tries, passed.";
				log(string2char(_log));
			}
			else {
				fileIdentities.push(fileIdentities.front());
				fileIdentities.pop();
			}
		}
		else {
			std::string _log = fileFolders[fileIdentities.front()].file_path;
			_log += " successfully processed in ";
			_log += to_string(fileFolders[fileIdentities.front()].status);
			_log += " try(tries).";
			log(string2char(_log));
			fileIdentities.pop();
		}
	}
	firstFile = false;
	return fileFolders[fileIdentities.front()];
}
