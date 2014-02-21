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

		std::ofstream fl;
		fl.open(LOG_FILE, std::ios_base::app | std::ios_base::out);
		fl << now << ": " << text << std::endl;
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

}