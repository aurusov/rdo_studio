/* 
typedef const char *LPCSTR, *PCSTR;
#ifndef AFX_CDECL
	#define AFX_CDECL __cdecl
#endif

void* AFX_CDECL operator new(size_t nSize, LPCSTR lpszFileName, int nLine);
void AFX_CDECL operator delete(void* p, LPCSTR lpszFileName, int nLine);
#define DEBUG_NEW new(THIS_FILE, __LINE__)

//#include <afxwin.h>
*/

#include <afx.h>




/////////////////////////////////////////////////////////////////////////////
 

#include <new>
#include <memory>
#include <fstream>

