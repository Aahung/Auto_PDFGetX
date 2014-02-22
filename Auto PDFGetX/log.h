#include <iostream>
#include <windows.h>
#include <vector>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

namespace console_log
{

	bool initializeLog();

	void log(char text[]);
	void logStr(std::string);
	char * string2char(std::string);
	std::wstring string2ws(const std::string& s);
	std::string ws2string(WCHAR ws[]);
}