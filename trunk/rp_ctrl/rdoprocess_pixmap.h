#ifndef RDO_PROCESS_CTRL_PIXMAP_H
#define RDO_PROCESS_CTRL_PIXMAP_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RPPixmap
// ----------------------------------------------------------------------------
class RPPixmap
{
protected:
	RPPixmap( char* _xpm[] ): xpm( _xpm ) {}
	char** xpm;
public:
	virtual HBITMAP getBitmap() = 0;
	virtual HICON   getIcon() = 0;
	virtual int getWidth() = 0;
	virtual int getHeight() = 0;
	virtual void Draw( HDC hdc, int x, int y, int cx ) = 0;
};

#endif // RDO_PROCESS_CTRL_PIXMAP_H
