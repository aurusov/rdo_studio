#ifndef RDO_PROCESS_CTRL_PIXMAP_H
#define RDO_PROCESS_CTRL_PIXMAP_H

#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_string.h"

// --------------------------------------------------------------------------------
// -------------------- RPPixmap
// --------------------------------------------------------------------------------
class RPPixmap
{
protected:
	char**   xpm;
	CBitmap  bmp;
	BITMAP   bmp_info;
	COLORREF transparent;
	struct Pixel
	{
		ruint r;
		ruint g;
		ruint b;

		Pixel()
			: r(0)
			, g(0)
			, b(0)
		{}
		Pixel(ruint _r, ruint _g, ruint _b)
			: r(_r)
			, g(_g)
			, b(_b)
		{}
		Pixel( const Pixel& copy )
			: r(copy.r)
			, g(copy.g)
			, b(copy.b)
		{}
	};

	int char_per_pixel;
	int char_per_r;
	int        getNextInt( rp::string& line ) const;
	rp::string getNextStr( rp::string& line ) const;
	rp::string getNextColor( rp::string& line ) const;
	Pixel      getNextPixel( rp::string& line );

public:
	RPPixmap( char* _xpm[] );
	RPPixmap( HICON icon );
	RPPixmap( ruint resource, COLORREF _transparent );
	HBITMAP getBitmap()   { return reinterpret_cast<HBITMAP>(bmp.m_hObject); }
	CBitmap& getCBitmap() { return bmp;                                      }
	int getWidth();
	int getHeight();
	HICON getIcon();
	void Draw( HDC hdc, int x, int y, int cx = 0 );
	void Clone( HDC hdc );
	void CloneScale( HDC hdc, int new_w, int new_h );
	COLORREF getTransparent() const { return transparent; }
};

#endif // RDO_PROCESS_CTRL_PIXMAP_H
