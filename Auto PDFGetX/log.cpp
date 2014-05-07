#include "log.h"
#include <ios>
#include <fstream>
#include <ctime>

namespace console_log
{
	const char * LOG_FILE = "log.txt";
	bool initializeLog()
	{
		std::ofstream log_file(LOG_FILE);//initial log file
		AllocConsole();
		// set the window to top
		HWND hwnd = GetConsoleWindow();
		//SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 600, 700, SWP_NOREDRAW | SWP_SHOWWINDOW);

		HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
		int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
		FILE* hf_out = _fdopen(hCrt, "w");
		setvbuf(hf_out, NULL, _IONBF, 1);
		*stdout = *hf_out;

		HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
		hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
		FILE* hf_in = _fdopen(hCrt, "r");
		setvbuf(hf_in, NULL, _IONBF, 128);
		*stdin = *hf_in;

		return true;
	}

	void log(char * text)
	{
		time_t now;
		time(&now);
		struct tm timeinfo;
		localtime_s(&timeinfo, &now);
		char buffer[32];
		// Format: Mo, 15.06.2009 20:20:00
		std::strftime(buffer, 32, "%d.%m.%Y %H:%M:%S", &timeinfo);  

		std::ofstream fl;
		fl.open(LOG_FILE, std::ios_base::app | std::ios_base::out);
		fl << buffer << ": " << text << std::endl;
		std::cout << text << std::endl;
		fl.close();
	}

	void logStr(std::string string)
	{
		const char * text = string.c_str();
		time_t now;
		time(&now);

		std::ofstream fl;
		fl.open(LOG_FILE, std::ios_base::app | std::ios_base::out);
		fl << now << ": " << text << std::endl;
		std::cout << text << std::endl;
		fl.close();
	}

	char * string2char(std::string str)
	{
		size_t size = str.size();
		char * ch = new char[size + 1];
		for (int i = 0; i < size; i ++)
		{
			ch[i] = str[i];
		}
		ch[size] = '\0';
		return ch;
	}

	std::wstring string2ws(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}

	std::string ws2string(WCHAR ws[])
	{
		//convert from wide char to narrow char array
		char ch[260];
		char DefChar = ' ';
		WideCharToMultiByte(CP_ACP,0,ws,-1, ch,260,&DefChar, NULL);
    
		//A std:string  using the char* constructor.
		std::string ss(ch);
		return ss;
	}
}