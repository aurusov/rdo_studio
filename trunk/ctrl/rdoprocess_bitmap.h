#ifndef RDO_PROCESS_BITMAP_H
#define RDO_PROCESS_BITMAP_H

#if _MSC_VER > 1000
#pragma once
#endif

#include <rdoprocess_bitmap.h>

// ----------------------------------------------------------------------------
// ---------- RPBitmap
// ----------------------------------------------------------------------------
class RPBitmapMFC: public RPBitmap
{
private:
	CBitmap bmp;
	BITMAP  bmp_info;
	struct Pixel {
		unsigned int r;
		unsigned int g;
		unsigned int b;
		Pixel():
			r(0),
			g(0),
			b(0)
		{
		}
		Pixel( unsigned int _r, unsigned int _g, unsigned int _b ):
			r(_r),
			g(_g),
			b(_b)
		{
		}
		Pixel( const Pixel& copy ):
			r(copy.r),
			g(copy.g),
			b(copy.b)
		{
		}
	};

	int char_per_pixel;
	int char_per_r;
	int        getNextInt( rp::string& line ) const;
	rp::string getNextStr( rp::string& line ) const;
	rp::string getNextColor( rp::string& line ) const;
	Pixel      getNextPixel( rp::string& line );

public:
	RPBitmapMFC( char* xpm[] );
	virtual HBITMAP getBitmap()   { return reinterpret_cast<HBITMAP>(bmp.m_hObject); }
	virtual CBitmap& getCBitmap() { return bmp;                                      }
	virtual int getWidth();
	virtual int getHeight();
};

#endif // RDO_PROCESS_BITMAP_H
