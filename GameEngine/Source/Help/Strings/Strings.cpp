#include "Core.h"

#include "Strings.h"

// The following function was taken from http://xoomer.virgilio.it/acantato/dev/wildcard/wildmatch.html, where it was attributed to 
// the C/C++ Users Journal, written by Mike Cornelison.

bool StringHelper::WildcardMatch(const char *pat, const char *str)
{
	int i, star;

new_segment:

	star = 0;
	if (*pat == '*') {
		star = 1;
		do { pat++; } while (*pat == '*'); /* enddo */
	} /* endif */

test_match:

	for (i = 0; pat[i] && (pat[i] != '*'); i++) {
		//if (mapCaseTable[str[i]] != mapCaseTable[pat[i]]) {
		if (str[i] != pat[i]) {
			if (!str[i]) return 0;
			if ((pat[i] == '?') && (str[i] != '.')) continue;
			if (!star) return 0;
			str++;
			goto test_match;
		}
	}
	if (pat[i] == '*') {
		str += i;
		pat += i;
		goto new_segment;
	}
	if (!str[i]) return 1;
	if (i && pat[i - 1] == '*') return 1;
	if (!star) return 0;
	str++;
	goto test_match;
};

//-----------------------------------------------------------------------------
// Name: AnsiToWideCch()
// Desc: This is a UNICODE conversion utility to convert a CHAR string into a
//       WCHAR string. 
//       cchDestChar is the size in TCHARs of wstrDestination.  Be careful not to 
//       pass in sizeof(strDest) 
//-----------------------------------------------------------------------------
bool StringHelper::AnsiToWideCch(wchar_t* wstrDestination, const char* strSource,
	int cchDestChar)
{
	if (wstrDestination == NULL || strSource == NULL || cchDestChar < 1)
		return false;

	int nResult = MultiByteToWideChar(CP_ACP, 0, strSource, -1,
		wstrDestination, cchDestChar);
	wstrDestination[cchDestChar - 1] = 0;

	if (nResult == 0)
		return false;
	return true;
}



//-----------------------------------------------------------------------------
// Name: WideToAnsi()
// Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//       CHAR string. 
//       cchDestChar is the size in TCHARs of strDestination
//-----------------------------------------------------------------------------
bool StringHelper::WideToAnsiCch(char* strDestination, const wchar_t* wstrSource,
	int cchDestChar)
{
	if (strDestination == NULL || wstrSource == NULL || cchDestChar < 1)
		return false;

	int nResult = WideCharToMultiByte(CP_ACP, 0, wstrSource, -1, strDestination,
		cchDestChar*sizeof(char), NULL, NULL);
	strDestination[cchDestChar - 1] = 0;

	if (nResult == 0)
		return false;
	return true;
}

//-----------------------------------------------------------------------------
// Name: GenericToAnsi()
// Desc: This is a UNICODE conversion utility to convert a TCHAR string into a
//       CHAR string. 
//       cchDestChar is the size in TCHARs of strDestination
//-----------------------------------------------------------------------------
bool StringHelper::GenericToAnsiCch(char* strDestination, const TCHAR* tstrSource,
	int cchDestChar)
{
	if (strDestination == NULL || tstrSource == NULL || cchDestChar < 1)
		return false;

#ifdef _UNICODE
	return WideToAnsiCch(strDestination, tstrSource, cchDestChar);
#else
	strncpy(strDestination, tstrSource, cchDestChar);
	strDestination[cchDestChar - 1] = '\0';
	return true;
#endif   
}

//-----------------------------------------------------------------------------
// Name: GenericToWide()
// Desc: This is a UNICODE conversion utility to convert a TCHAR string into a
//       WCHAR string. 
//       cchDestChar is the size in TCHARs of wstrDestination.  Be careful not to 
//       pass in sizeof(strDest) 
//-----------------------------------------------------------------------------
bool StringHelper::GenericToWideCch(wchar_t* wstrDestination, const TCHAR* tstrSource,
	int cchDestChar)
{
	if (wstrDestination == NULL || tstrSource == NULL || cchDestChar < 1)
		return false;

#ifdef _UNICODE
	wcsncpy(wstrDestination, tstrSource, cchDestChar);
	wstrDestination[cchDestChar - 1] = L'\0';
	return true;
#else
	return AnsiToWideCch(wstrDestination, tstrSource, cchDestChar);
#endif    
}



//-----------------------------------------------------------------------------
// Name: AnsiToGeneric()
// Desc: This is a UNICODE conversion utility to convert a CHAR string into a
//       TCHAR string. 
//       cchDestChar is the size in TCHARs of tstrDestination.  Be careful not to 
//       pass in sizeof(strDest) on UNICODE builds
//-----------------------------------------------------------------------------
bool StringHelper::AnsiToGenericCch(TCHAR* tstrDestination, const char* strSource,
	int cchDestChar)
{
	if (tstrDestination == NULL || strSource == NULL || cchDestChar < 1)
		return false;

#ifdef _UNICODE
	return AnsiToWideCch(tstrDestination, strSource, cchDestChar);
#else
	strncpy(tstrDestination, strSource, cchDestChar);
	tstrDestination[cchDestChar - 1] = '\0';
	return true;
#endif    
}



//-----------------------------------------------------------------------------
// Name: AnsiToGeneric()
// Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//       TCHAR string. 
//       cchDestChar is the size in TCHARs of tstrDestination.  Be careful not to 
//       pass in sizeof(strDest) on UNICODE builds
//-----------------------------------------------------------------------------
bool StringHelper::WideToGenericCch(TCHAR* tstrDestination, const wchar_t* wstrSource,
	int cchDestChar)
{
	if (tstrDestination == NULL || wstrSource == NULL || cchDestChar < 1)
		return false;

#ifdef _UNICODE
	wcsncpy(tstrDestination, wstrSource, cchDestChar);
	tstrDestination[cchDestChar - 1] = L'\0';
	return true;
#else
	return WideToAnsiCch(tstrDestination, wstrSource, cchDestChar);
#endif
}



string StringHelper::ws2s(const wstring & s)
{
	int slength = (int)s.length() + 1;
	int len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0) - 1;
	string r(len, '\0');
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}


wstring StringHelper::s2ws(const string & s)
{
	int slength = (int)s.length() + 1;
	int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0) - 1;
	wstring r(len, '\0');
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
	return r;
}