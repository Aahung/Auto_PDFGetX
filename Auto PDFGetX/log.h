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
}