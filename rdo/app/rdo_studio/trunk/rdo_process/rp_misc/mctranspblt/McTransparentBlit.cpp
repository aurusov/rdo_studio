// McTransparentBlit.cpp: implementation of the McTransparentBlit class.
//
//////////////////////////////////////////////////////////////////////

#include "app/rdo_studio_mfc/rdo_process/rp_misc/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/mctranspblt/McTransparentBlit.h"

//////////////////////////////////////////////////////////////////////
// Name	  :	McTransparentBlt
// Version: 1.0 [9/29/2001]
// Author :	Zhuk Oleg, Russian Federation, Kaliningrad city
// Email  :	zhuk@mediachase.com
//////////////////////////////////////////////////////////////////////
// Info:	The TransparentBlt function performs a bit-block transfer of 
//			the color data corresponding to a rectangle of pixels from the 
//			specified source device context into a destination device context.
//////////////////////////////////////////////////////////////////////
//  Requirements:
//			Windows NT/2000: Requires Windows NT.
//			Windows 95/98: Requires Windows 95.
//////////////////////////////////////////////////////////////////////

/*
BOOL McTransparentBlt(
					HDC hdcDest,        // handle to destination DC
					int nXOriginDest,   // x-coord of destination upper-left corner
					int nYOriginDest,   // y-coord of destination upper-left corner
					int nWidthDest,     // width of destination rectangle
					int hHeightDest,    // height of destination rectangle
					HDC hdcSrc,         // handle to source DC
					int nXOriginSrc,    // x-coord of source upper-left corner
					int nYOriginSrc,    // y-coord of source upper-left corner
					int nWidthSrc,      // width of source rectangle
					int nHeightSrc,     // height of source rectangle
					UINT crTransparent  // color to make transparent
					);
Parameters
	hdcDest			[in] Handle to the destination device context. 
	nXOriginDest	[in] Specifies the x-coordinate, in logical units, of the upper-left corner of the destination rectangle. 
	nYOriginDest	[in] Specifies the y-coordinate, in logical units, of the upper-left corner of the destination rectangle. 
	nWidthDest		[in] Specifies the width, in logical units, of the destination rectangle. 
	hHeightDest		[in] Handle to the height, in logical units, of the destination rectangle. 
	hdcSrc			[in] Handle to the source device context. 
	nXOriginSrc		[in] Specifies the x-coordinate, in logical units, of the source rectangle. 
	nYOriginSrc		[in] Specifies the y-coordinate, in logical units, of the source rectangle. 
	nWidthSrc		[in] Specifies the width, in logical units, of the source rectangle. 
	nHeightSrc		[in] Specifies the height, in logical units, of the source rectangle. 
	crTransparent	[in] The RGB color in the source bitmap to treat as transparent. 

Return Values:
	If the function succeeds, the return value is TRUE.

	If the function fails, the return value is FALSE. 
*/
//////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL McTransparentBlt(IN HDC hdcDest,IN int nXOriginDest,IN int nYOriginDest,
					  IN int nWidthDest,IN int nHeightDest,IN HDC hdcSrc,
					  IN int nXOriginSrc,IN int nYOriginSrc,IN int nWidthSrc,
					  IN int nHeightSrc,IN UINT crTransparent)
{
	
	BOOL bExitCode	=	TRUE;
	if(!hdcDest||!hdcSrc)
		return FALSE;

	HDC			hdcMask			=	NULL;
	HDC			hdcTmpSrc		=	NULL;
	HBITMAP		hbmTransMask	=	NULL;
	HBITMAP		oldhbmTransMask =	NULL;
	HBITMAP		hbmTmpSrc		=	NULL;
	HBITMAP		oldhbmTmpSrc	=	NULL;
	

	COLORREF	OldBkColor		=	::SetBkColor(hdcDest, RGB(255,255,255));
	COLORREF	OldTextColor	=	::SetTextColor(hdcDest,RGB(0,0,0));	
	
	// Step 0. Подготовка [9/28/2001]
	if((hdcMask = ::CreateCompatibleDC(hdcDest))==NULL)
	{
		bExitCode	=	FALSE;
		goto ClearUp;
	}

	if((hdcTmpSrc = ::CreateCompatibleDC(hdcSrc))==NULL)
	{
		bExitCode	=	FALSE;
		goto ClearUp;
	}

	if((hbmTmpSrc = ::CreateCompatibleBitmap(hdcDest,nWidthSrc,nHeightSrc))==NULL)
	{
		bExitCode	=	FALSE;
		goto ClearUp;
	}

	oldhbmTmpSrc = (HBITMAP)::SelectObject(hdcTmpSrc,hbmTmpSrc);
		
	if(!::BitBlt(hdcTmpSrc,0,0,nWidthSrc,nHeightSrc,hdcSrc,nXOriginSrc,nYOriginSrc,SRCCOPY))
	{
		bExitCode	=	FALSE;
		goto ClearUp;
	}

	if((hbmTransMask = ::CreateBitmap(nWidthSrc,nHeightSrc,1,1,NULL))==NULL)
	{
		bExitCode	=	FALSE;
		goto ClearUp;
	}

	oldhbmTransMask = (HBITMAP)::SelectObject(hdcMask,hbmTransMask);
	
	// Step 1.  Построение маски [9/28/2001]
	::SetBkColor(hdcTmpSrc,crTransparent);
	if(!::BitBlt(hdcMask, 0, 0, nWidthSrc, nHeightSrc, hdcTmpSrc, 0, 0, SRCCOPY))
	{
		bExitCode	=	FALSE;
		goto ClearUp;
	}

	// Step 3. Модифицирование Источника, если прозрачный цвет отличен от черного [9/28/2001]
	if(crTransparent!=RGB(0,0,0))
	{
		::SetBkColor(hdcTmpSrc, RGB(0,0,0));			// все 1 --> черный (0x000000) 
		::SetTextColor(hdcTmpSrc,RGB(255,255,255));	// все 0 --> белый (0xFFFFFF)
		
		if(!::BitBlt(hdcTmpSrc, 0, 0, nWidthSrc, nHeightSrc, hdcMask, 0, 0, SRCAND))
		{
			bExitCode	=	FALSE;
			goto ClearUp;
		}
	}
	// Step 4. Копирование Изображения.

	if((nWidthDest==nWidthSrc)&&(nHeightDest==nHeightSrc))
	{
		if(!::BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthSrc, nHeightSrc, hdcMask, 0, 0, SRCAND))
		{
			bExitCode	=	FALSE;
			goto ClearUp;
		}
		if(!::BitBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthSrc, nHeightSrc, hdcTmpSrc, 0, 0, SRCPAINT))
		{
			bExitCode	=	FALSE;
			goto ClearUp;
		}
	}
	else
	{
		if(!::StretchBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hdcMask, 0, 0, nWidthSrc, nHeightSrc,SRCAND))
		{
			bExitCode	=	FALSE;
			goto ClearUp;
		}
		if(!::StretchBlt(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, hdcTmpSrc, 0, 0,  nWidthSrc, nHeightSrc,SRCPAINT))
		{
			bExitCode	=	FALSE;
			goto ClearUp;
		}
	}
	
	// Free Resourse [9/28/2001]
ClearUp:
	if(hdcMask)
	{
		if(oldhbmTransMask)
			::SelectObject(hdcMask,oldhbmTransMask);
		::DeleteDC(hdcMask);
	}

	if(hbmTmpSrc)
	{
		if(oldhbmTmpSrc)
			::SelectObject(hdcTmpSrc,oldhbmTmpSrc);
		::DeleteObject(hbmTmpSrc);
	}

	if(hdcTmpSrc)
		::DeleteDC(hdcTmpSrc);
	
	if(hbmTransMask)
		::DeleteObject(hbmTransMask);

	::SetBkColor(hdcDest, OldBkColor);
	::SetTextColor(hdcDest,OldTextColor);	
	
	return bExitCode;
}
