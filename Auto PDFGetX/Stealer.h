class Stealer
{
public:
	Stealer();
	~Stealer();
	bool stealHandleAutomatically(HWND);
	void stealHandleByPoint(POINT &p);
	bool allHandleStolen();
	HWND getSampleBtnHandle();
	HWND getSampleBackgroundBtnHandle();
	HWND getResetBtnHandle();
	HWND getGetIBtnHandle();
	HWND getCalCBtnHandle();
	HWND getGetSBtnHandle();
	HWND getGetGBtnHandle();
	HWND getStatusTextHandle();
	std::string getTextFromHandle(HWND);

private:
	HWND hSampleBtn, hSampleBackgroundBtn, hResetBtn, hGetIBtn, hCalCBtn, hGetSBtn, hGetGBtn, hStatusText;
};
