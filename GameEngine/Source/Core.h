#pragma once

#include <D3D11.h>
#include <d3dx11.h>
#include <d3d11shader.h>
#include <D3Dcommon.h>
//#include <D3DX10.h>

#include <assert.h>
#include <basetsd.h>
#include <memory>
#include <math.h>
#include <string>
#include <wchar.h>
#include <tchar.h>

#include <winerror.h>
#include <Windows.h>
#include <windowsx.h>

#include <vector>
#include <queue>
#include <map>
#include <list>

using namespace std;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define SAMPLES_COUNT 8
#define SAMPLES_QUALITY 1

#define INPUT_LAYOUT D3D11_INPUT_ELEMENT_DESC

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

#define PI 3.14159265

#define fOPAQUE (1.0f)
#define iOPAQUE (1)
#define fTRANSPARENT (0.0f)
#define iTRANSPARENT (0)

#define MEGABYTE (1024 * 1024)
#define SIXTY_HERTZ (16.66f)

#define ASSERTIONS_ENABLED 1

#define DebugBrake() __asm { int 3 }

#ifdef ASSERTIONS_ENABLED
#define ASSERT(expr) \
if (expr)	\
{ }		\
		else \
{ \
	/*reportAssertionFailure(#expr, __FILE__, __LINE__); \
DebugBrake(); */ \
}
#else 
#define ASSERT(expr)
#endif		 

#ifdef _DEBUG 
#define Check(x) \
	OutputDebugStringA(x); \
	DebugBrake();
#else
#define Check(x) \
	OutputDebugStringA(x)
#endif

#ifdef _DEBUG
#define CheckHR(x) \
	switch (x) \
{ \
	case S_OK: \
	break; \
	\
	case S_FALSE: \
	OutputDebugStringA("Completed without error, but only partial results were obtained. \
							If a buffer is not large enough to hold the information that is returned \
							to it, the returned information is often truncated to fit into the buffer \
							and S_FALSE is returned from the method."); \
							break; \
							\
	case E_FAIL: \
	OutputDebugStringA("Operation couldn't be performed."); \
	break; \
	\
	case E_INVALIDARG: \
	OutputDebugStringA("One of the arguments passed in was invalid."); \
	break; \
	\
	case E_NOINTERFACE: \
	OutputDebugStringA("The object searched for was not found."); \
	break; \
	\
	case E_OUTOFMEMORY: \
	OutputDebugStringA("A memory allocation attempt failed."); \
	break; \
	\
	case E_UNEXPECTED: \
	OutputDebugStringA("The target was not accessible, ot the engine was not in a state \
							where the function or method could be processed."); \
							break; \
							\
	case E_NOTIMPL: \
	OutputDebugStringA("Not implemented."); \
	break; \
	\
	default: \
	OutputDebugStringA("File not found."); \
	break; \
}; \
	DebugBrake();
#else

#define CheckHR(x) {}

#endif

#define VALID(hr) \
if (hr == S_OK) \
	return true; \
	else \
{ \
	CheckHR(hr); \
	return false; \
};

typedef unsigned int EntityId;
typedef unsigned int EntityRepresentationId;
typedef unsigned char Key;
typedef unsigned int StateId;
typedef map<string, int> ContentMap;

enum SystemMessageType
{
	SMT_Quit,
	SMT_KeyDown,
	SMT_KeyUp,
	SMT_LMouseDown,
	SMT_LMouseUp,
	SMT_RMouseDown,
	SMT_RMouseUp,
	SMT_MouseMove,
};

enum ShaderType
{
	ST_Vertex,
	ST_Hull,
	ST_Domain,
	ST_Geometry,
	ST_Pixel,
	ST_Compute
};