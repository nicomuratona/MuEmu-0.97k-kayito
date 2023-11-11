#include "stdafx.h"
#include "Font.h"

CFont gFont;

CFont::CFont()
{
	GetPrivateProfileString("Font", "FontName", "Verdana", this->MyFontFaceName, 32, ".\\Config.ini");

	this->Height = GetPrivateProfileInt("Font", "FontHeight", 13, ".\\Config.ini");

	this->Italic = GetPrivateProfileInt("Font", "FontItalic", 0, ".\\Config.ini");

	this->Charset = GetPrivateProfileInt("Font", "FontCharset", DEFAULT_CHARSET, ".\\Config.ini");

	this->Width = GetPrivateProfileInt("Font", "FontWidth", 0, ".\\Config.ini");

	this->UnderLine = GetPrivateProfileInt("Font", "FontUnderline", 0, ".\\Config.ini");

	this->Quality = GetPrivateProfileInt("Font", "FontQuality", NONANTIALIASED_QUALITY, ".\\Config.ini");

	this->StrikeOut = GetPrivateProfileInt("Font", "FontStrikeOut", 0, ".\\Config.ini");
}

CFont::~CFont()
{
	char Text[33] = { 0 };

	WritePrivateProfileString("Font", "FontName", this->MyFontFaceName, ".\\Config.ini");

	wsprintf(Text, "%d", this->Height);

	WritePrivateProfileString("Font", "FontHeight", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->Italic);

	WritePrivateProfileString("Font", "FontItalic", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->Charset);

	WritePrivateProfileString("Font", "FontCharset", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->Width);

	WritePrivateProfileString("Font", "FontWidth", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->UnderLine);

	WritePrivateProfileString("Font", "FontUnderline", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->Quality);

	WritePrivateProfileString("Font", "FontQuality", Text, ".\\Config.ini");

	wsprintf(Text, "%d", this->StrikeOut);

	WritePrivateProfileString("Font", "FontStrikeOut", Text, ".\\Config.ini");
}

void CFont::Init()
{
	SetCompleteHook(0xE8, 0x0041F0CD, &this->MyCreateFont);
	SetByte(0x0041F0CD + 5, 0x90);

	SetCompleteHook(0xE8, 0x0041F10E, &this->MyCreateFontBold);
	SetByte(0x0041F10E + 5, 0x90);

	SetCompleteHook(0xE8, 0x0041F151, &this->MyCreateFontBig);
	SetByte(0x0041F151 + 5, 0x90);
}

HFONT CFont::MyCreateFont(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName)
{
	return CreateFont(gFont.Height, gFont.Width, 0, 0, FW_NORMAL, gFont.Italic, gFont.UnderLine, gFont.StrikeOut, gFont.Charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, gFont.Quality, DEFAULT_PITCH, gFont.MyFontFaceName);
}

HFONT CFont::MyCreateFontBold(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName)
{
	return CreateFont(gFont.Height, gFont.Width, 0, 0, FW_BOLD, gFont.Italic, gFont.UnderLine, gFont.StrikeOut, gFont.Charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, gFont.Quality, DEFAULT_PITCH, gFont.MyFontFaceName);
}

HFONT CFont::MyCreateFontBig(int cHeight, int cWidth, int cEscapement, int cOrientation, int cWeight, DWORD bItalic, DWORD bUnderline, DWORD bStrikeOut, DWORD iCharSet, DWORD iOutPrecision, DWORD iClipPrecision, DWORD iQuality, DWORD iPitchAndFamily, LPCSTR pszFaceName)
{
	return CreateFont(gFont.Height * 2, gFont.Width, 0, 0, FW_BOLD, gFont.Italic, gFont.UnderLine, gFont.StrikeOut, gFont.Charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, gFont.Quality, DEFAULT_PITCH, gFont.MyFontFaceName);
}

void CFont::ReloadFont()
{
	g_hFont = CreateFont(this->Height, this->Width, 0, 0, FW_NORMAL, this->Italic, this->UnderLine, this->StrikeOut, this->Charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, this->Quality, DEFAULT_PITCH, this->MyFontFaceName);

	g_hFontBold = CreateFont(this->Height, this->Width, 0, 0, FW_BOLD, this->Italic, this->UnderLine, this->StrikeOut, this->Charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, this->Quality, DEFAULT_PITCH, this->MyFontFaceName);

	g_hFontBig = CreateFont(this->Height * 2, this->Width, 0, 0, FW_BOLD, this->Italic, this->UnderLine, this->StrikeOut, this->Charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, this->Quality, DEFAULT_PITCH, this->MyFontFaceName);

	FontHeight = this->Height;

	((void(_cdecl*)())0x0050F690)(); // OpenFont();
}