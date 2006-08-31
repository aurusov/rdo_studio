// McTransparentBlit.h: interface for the McTransparentBlit function.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTRANSPARENTBLIT_H__A357B200_25C5_41E1_8DDC_E483069D0936__INCLUDED_)
#define AFX_MCTRANSPARENTBLIT_H__A357B200_25C5_41E1_8DDC_E483069D0936__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

//////////////////////////////////////////////////////////////////////

BOOL McTransparentBlt(IN HDC hdcDest,IN int nXOriginDest,IN int nYOriginDest,
					  IN int nWidthDest,IN int nHeightDest,IN HDC hdcSrc,
					  IN int nXOriginSrc,IN int nYOriginSrc,IN int nWidthSrc,
					  IN int nHeightSrc,IN UINT crTransparent);

//////////////////////////////////////////////////////////////////////					  

#endif // !defined(AFX_MCTRANSPARENTBLIT_H__A357B200_25C5_41E1_8DDC_E483069D0936__INCLUDED_)
