#include "stdafx.h"
#include "rdoprocess_bitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPBitmap
// ----------------------------------------------------------------------------
RPBitmapMFC::RPBitmapMFC( char* _xpm[] ):
	RPBitmap( _xpm ),
	char_per_pixel( 0 ),
	char_per_r( 0 )
{
	bmp_info.bmWidth  = -1;
	bmp_info.bmHeight = -1;
	if ( !xpm ) return;
	rp::string line( xpm[0] );
	const int w         = getNextInt( line );
	const int h         = getNextInt( line );
	const int color_cnt = getNextInt( line );
	char_per_pixel      = getNextInt( line );
	std::map< rp::string, RPBitmapMFC::Pixel > colors;
	int line_index = 1;
	while ( true ) {
		line = xpm[line_index++];
		if ( line.find( " c ", char_per_pixel ) != std::string::npos || line.find( "\tc ", char_per_pixel ) != std::string::npos ) {
			rp::string color_name = getNextColor( line );
			getNextStr( line );
			Pixel color_value = getNextPixel( line );
			typedef std::map< rp::string, RPBitmapMFC::Pixel > Colors;
			colors.insert( Colors::value_type(color_name, color_value) );
		} else {
			line_index--;
			break;
		}
	}
	line = xpm[ line_index ];
	int color_data_per_pixel = line.size() / w / char_per_pixel;
	int byte_per_pixel;
	if ( color_cnt == 65535 ) {
		// 16-бит
		byte_per_pixel = 2;
	} else {
		// Всё остальное 32-бита
		byte_per_pixel = 4;
	}
	int w_align = !(w % 2) ? w : w + 1;
	unsigned char* data = new unsigned char[ w_align * h * byte_per_pixel ];
	for ( int y = 0; y < h; y++ ) {
		line = xpm[ y + line_index ];
		for ( int x = 0; x < w; x++ ) {
			if ( color_data_per_pixel == 1 ) {
				Pixel pixel = colors[ line.substr( x * char_per_pixel, char_per_pixel ) ];
				unsigned char r = char_per_r == 2 ? pixel.r : pixel.r >> 8;
				unsigned char g = char_per_r == 2 ? pixel.g : pixel.g >> 8;
				unsigned char b = char_per_r == 2 ? pixel.b : pixel.b >> 8;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 0 ] = b;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 1 ] = g;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 2 ] = r;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 3 ] = 0;
			} else if ( color_data_per_pixel == 3 ) {
				Pixel pixel = colors[ line.substr( x * char_per_pixel * color_data_per_pixel, char_per_pixel ) ];
				unsigned char r = char_per_r == 2 ? pixel.r : pixel.r >> 8;
				pixel = colors[ line.substr( x * char_per_pixel * color_data_per_pixel + char_per_pixel, char_per_pixel ) ];
				unsigned char g = char_per_r == 2 ? pixel.r : pixel.r >> 8;
				pixel = colors[ line.substr( x * char_per_pixel * color_data_per_pixel + char_per_pixel + char_per_pixel, char_per_pixel ) ];
				unsigned char b = char_per_r == 2 ? pixel.r : pixel.r >> 8;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 0 ] = b;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 1 ] = g;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 2 ] = r;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 3 ] = 0;
			} else if ( color_data_per_pixel == 2 ) {
				Pixel pixel1 = colors[ line.substr( x * char_per_pixel * color_data_per_pixel, char_per_pixel ) ];
				Pixel pixel2 = colors[ line.substr( x * char_per_pixel * color_data_per_pixel + char_per_pixel, char_per_pixel ) ];
				unsigned char byte1 = char_per_r == 2 ? pixel1.r : pixel1.r >> 8;
				unsigned char byte2 = char_per_r == 2 ? pixel2.r : pixel2.r >> 8;
				unsigned char r = byte2 & 0x7C;
				unsigned char b = byte1 & 0x1F;
				byte1 &= ~0x1F;
				byte2 &= ~0x7C;
				byte1 |= (r >> 2);
				byte2 |= (b << 2 );
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 0 ] = byte1;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 1 ] = byte2;
			}
			// Надо выровнять каждую строку картинки до DWORD, актуально только для 16-бит
			if ( color_data_per_pixel == 2 && w_align != w ) {
				data[ y * w_align * byte_per_pixel + (w_align - 1) * byte_per_pixel + 0 ] = 0;
				data[ y * w_align * byte_per_pixel + (w_align - 1) * byte_per_pixel + 1 ] = 0;
			}
		}
	}

	CDC* desktopDC = CWnd::GetDesktopWindow()->GetDC();
	CDC memDC;
	memDC.CreateCompatibleDC( desktopDC );
	CBitmap memBMP;
	memBMP.CreateCompatibleBitmap( desktopDC, w_align, h );

	BITMAPINFO bmp_info;
	bmp_info.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	bmp_info.bmiHeader.biWidth         = w_align;
	bmp_info.bmiHeader.biHeight        = -h;
	bmp_info.bmiHeader.biPlanes        = 1;
	bmp_info.bmiHeader.biBitCount      = byte_per_pixel * 8;
	bmp_info.bmiHeader.biCompression   = BI_RGB;
	bmp_info.bmiHeader.biSizeImage     = 0;
	bmp_info.bmiHeader.biXPelsPerMeter = 0;
	bmp_info.bmiHeader.biYPelsPerMeter = 0;
	bmp_info.bmiHeader.biClrUsed       = 0;
	bmp_info.bmiHeader.biClrImportant  = 0;
	::SetDIBits( desktopDC->m_hDC, static_cast<HBITMAP>(memBMP.m_hObject), 0, h, data, &bmp_info, DIB_RGB_COLORS );
	CBitmap* hOldBitmap1 = memDC.SelectObject( &memBMP );

	CDC dc;
	dc.CreateCompatibleDC( desktopDC );
	bmp.CreateCompatibleBitmap( desktopDC, w, h );
	CBitmap* hOldBitmap2 = dc.SelectObject( &bmp );
	dc.BitBlt( 0, 0, w, h, &memDC, 0, 0, SRCCOPY );

	memDC.SelectObject( hOldBitmap1 );
	dc.SelectObject( hOldBitmap2 );
	if ( data ) delete[] data;
	if ( desktopDC ) CWnd::GetDesktopWindow()->ReleaseDC( desktopDC );
}

int RPBitmapMFC::getNextInt( rp::string& line ) const
{
	line.trim_left();
	std::string::size_type pos = line.find( ' ' );
	rp::string str = line.substr( 0, pos );
	line.erase( 0, pos + 1 );
	return str.toint();
}

rp::string RPBitmapMFC::getNextStr( rp::string& line ) const
{
	line.trim_left();
	std::string::size_type pos = line.find( ' ' );
	rp::string str = line.substr( 0, pos );
	line.erase( 0, pos + 1 );
	return str;
}

rp::string RPBitmapMFC::getNextColor( rp::string& line ) const
{
	rp::string str = line.substr( 0, char_per_pixel );
	line.erase( 0, char_per_pixel + 1 );
	return str;
}

RPBitmapMFC::Pixel RPBitmapMFC::getNextPixel( rp::string& line )
{
	line.trim_left();
	std::string::size_type pos = line.find( ' ' );
	rp::string str = line.substr( 0, pos );
	line.erase( 0, pos + 1 );
	if ( str == "None" ) {
		COLORREF color = ::GetSysColor( COLOR_BTNFACE );
		return Pixel( GetRValue(color) + (GetRValue(color) << 8), GetGValue(color) + (GetGValue(color) << 8), GetBValue(color) + (GetBValue(color) << 8) );
	} else {
		str.erase( 0, 1 );
		if ( !char_per_r ) char_per_r = str.size() / 3;
		rp::string r = str.substr( char_per_r * 0, char_per_r );
		rp::string g = str.substr( char_per_r * 1, char_per_r );
		rp::string b = str.substr( char_per_r * 2, char_per_r );
		return Pixel( r.tohex(), g.tohex(), b.tohex() );
	}
}

int RPBitmapMFC::getWidth()
{
	if ( bmp_info.bmWidth == -1 ) {
		if ( !bmp.GetBitmap( &bmp_info ) ) {
			bmp_info.bmWidth = -1;
		}
	}
	return bmp_info.bmWidth;
}

int RPBitmapMFC::getHeight()
{
	if ( bmp_info.bmHeight == -1 ) {
		if ( !bmp.GetBitmap( &bmp_info ) ) {
			bmp_info.bmHeight = -1;
		}
	}
	return bmp_info.bmHeight;
}
