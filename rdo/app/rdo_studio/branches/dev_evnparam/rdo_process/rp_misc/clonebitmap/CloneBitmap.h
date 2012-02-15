// CloneBitmap.h: interface for the CCloneBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLONEBITMAP_H__55CEEA25_6B78_4EEC_B3DE_A9DB0AA52AC3__INCLUDED_)
#define AFX_CLONEBITMAP_H__55CEEA25_6B78_4EEC_B3DE_A9DB0AA52AC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IRGB(r,g,b)			((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))
#define INVERSECOLOR(x)		((COLORREF)(x&0xFF00)|((x&0xFF)<<16)|((x&0xFF0000)>>16))

class CCloneBitmap : public CBitmap  
{
public:
	COLORREF *m_lpData;
	HICON MakeIcon(COLORREF clrTransparent);
	int ChangeColor(COLORREF clrOld, COLORREF clrNew);
	int Clone(HBITMAP hSrc);
	CCloneBitmap();
	virtual ~CCloneBitmap();
};

#endif // !defined(AFX_CLONEBITMAP_H__55CEEA25_6B78_4EEC_B3DE_A9DB0AA52AC3__INCLUDED_)
