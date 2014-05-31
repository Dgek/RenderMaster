#pragma once

#include <D3D11.h>
#include <d3dx11.h>
//#include <D3DX10.h>

#include <assert.h>
#include <memory>
#include <string>
#include <wchar.h>
#include <tchar.h>

#include <winerror.h>
#include <Windows.h>

#include <vector>
#include <map>

using namespace std;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define SAMPLES_COUNT 8
#define SAMPLES_QUALITY 1

#define INPUT_LAYOUT D3D11_INPUT_ELEMENT_DESC

typedef unsigned int EntityId;
typedef unsigned int EntityRepresentationId;
typedef unsigned char Key;
typedef map<string, int> ContentMap;

/////////////////////////////////////////////////////
//Bitwise macroses
/////////////////////////////////////////////////////
#define Bit_Left(x) (1 << x)

/////////////////////////////////////////////////////
//Memory management
/////////////////////////////////////////////////////
#if !defined(SAFE_DELETE)
#define SAFE_DELETE(x) if(x) delete x; x=NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=NULL; 
#endif

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(x) if(x) x->Release(); x=NULL;
#endif

#define ALIGN16 _declspec(align(16))

/////////////////////////////////////////////////////
//Other defines
/////////////////////////////////////////////////////
#if !defined INFINITE
#define INFINITE = 0xffffffff;
#endif

#if !defined INFINITY
#define INFINITY FLT_MAX 
#endif

#define PI		 = 3.14159265;

#define fOPAQUE (1.0f)
#define iOPAQUE (1)
#define fTRANSPARENT (0.0f)
#define iTRANSPARENT (0)

#define MEGABYTE (1024 * 1024)
#define SIXTY_HERTZ (16.66f)