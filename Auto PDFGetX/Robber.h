class Robber
{
public:
	Robber(HWND _hSampleBtn, HWND _hSampleBackgroundBtn, HWND _hResetBtn,
		HWND _hGetIBtn, HWND _hCalCBtn, 
		HWND _hGetSBtn, HWND _hGetGBtn, HWND _hAdvancedBtn,
		HWND _hStatusText);
	~Robber();
	bool process(std::string file_path, std::string bg_file_path);

private:
	HWND hSampleBtn, hSampleBackgroundBtn, hResetBtn, hGetIBtn, hCalCBtn, hGetSBtn, hGetGBtn, hAdvancedBtn, hStatusText;
	std::string GetStatusText();
	bool processStep(unsigned int step);
};