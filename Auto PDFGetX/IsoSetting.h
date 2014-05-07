#include <iostream>
#include <fstream>
#include <map>
#include "Utility.h"

const char * SETTINGFILE = "Auto PDFGetX.setting";
const std::map<char *, char *> DEFAULTSETTINGS = {
		{"sample-prefix", "Silan_sample01_withflux_PDFrun01cool-"},
		{"background-prefix", "Silan_holder_PDF_350mm_cool-"},
		{"default-directory", "D:"}
};

class IsoSetting
{
public:
	static char * get (char *);

private:
	static void ensureSettingFile();
	static char * getSettings(char * key);
};

char * IsoSetting::get(char * key) {
	char * value = getSettings(key);
	return value;
}

void IsoSetting::ensureSettingFile() {
	std::ifstream ifs(SETTINGFILE, std::ifstream::in);
	if (!ifs) {
		std::fstream fs(SETTINGFILE, std::ofstream::out);
		for (auto i = DEFAULTSETTINGS.begin(); i != DEFAULTSETTINGS.end(); i++) {
			char * key = (*i).first;
			char * value = (*i).second;
			fs << key << " = " << value << std::endl;
		}
		fs.close();
	}
	ifs.close();
}

char * IsoSetting::getSettings(char * key) {
	ensureSettingFile();
	std::ifstream ifs(SETTINGFILE, std::ifstream::in);
	std::string line;
	while (std::getline(ifs, line)) {
		std::vector<std::string> tokens = Utility::split_str(line, " = ");
		if (tokens.size() != 2) continue;
		if (strcmp(key, Utility::string2char(tokens[0])) == 0){
			char * value = Utility::string2char(tokens[1]);
			return value;
		}
	}
}
