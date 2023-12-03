#pragma once

class CFont
{
public:

	CFont();

	virtual ~CFont();

	void Init();

	void ReloadFont();

private:

	static HFONT MyCreateFont(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName);
	
	static HFONT MyCreateFontBold(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName);
	
	static HFONT MyCreateFontBig(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName);

	void GetFonts();

	static int CALLBACK MyEnumFontFamExProc(const LOGFONT* lpelfe, const TEXTMETRIC* lpntme, DWORD FontType, LPARAM lParam);

public:

	int Bold;

	int Italic;

	int Charset;

	int Width;

	int UnderLine;

	int Quality;

	int StrikeOut;

	char MyFontFaceName[32];

	std::vector<std::string> AllFonts;
	int CurrentFontNumber;
};

extern CFont gFont;