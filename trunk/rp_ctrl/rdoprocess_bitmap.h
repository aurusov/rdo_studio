#ifndef RDO_PROCESS_CTRL_BITMAP_H
#define RDO_PROCESS_CTRL_BITMAP_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RPBitmap
// ----------------------------------------------------------------------------
class RPBitmap
{
protected:
	RPBitmap( char* _xpm[] ): xpm( _xpm ) {}
	char** xpm;
public:
	virtual HBITMAP getBitmap() = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
};

#endif // RDO_PROCESS_CTRL_BITMAP_H
