#pragma once

class StringHelper
{
public:
	static bool AnsiToWideCch(wchar_t* dest, const char* src, int charCount);
	static bool WideToAnsiCch(char* dest, const wchar_t* src, int charCount);
	static bool GenericToAnsiCch(char* dest, const TCHAR* src, int charCount);
	static bool GenericToWideCch(wchar_t* dest, const TCHAR* src, int charCount);
	static bool AnsiToGenericCch(TCHAR* dest, const char* src, int charCount);
	static bool WideToGenericCch(TCHAR* dest, const wchar_t* src, int charCount);

	static string ws2s(const wstring & s);
	static wstring s2ws(const string &s);
};