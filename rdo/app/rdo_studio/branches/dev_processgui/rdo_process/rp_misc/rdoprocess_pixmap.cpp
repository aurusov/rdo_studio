#include "app/rdo_studio_mfc/rdo_process/rp_misc/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_pixmap.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/mctranspblt/McTransparentBlit.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/clonebitmap/CloneBitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPPixmap
// --------------------------------------------------------------------------------
RPPixmap::RPPixmap( char* _xpm[] ):
	xpm( _xpm ),
	transparent( ::GetSysColor( COLOR_BTNFACE ) ),
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
	std::map< rp::string, RPPixmap::Pixel > colors;
	int line_index = 1;
	while ( true ) {
		line = xpm[line_index++];
		if ( line.find( " c ", char_per_pixel ) != tstring::npos || line.find( "\tc ", char_per_pixel ) != tstring::npos ) {
			rp::string color_name = getNextColor( line );
			getNextStr( line );
			Pixel color_value = getNextPixel( line );
			typedef std::map< rp::string, RPPixmap::Pixel > Colors;
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
	rbyte* data = new rbyte[ w_align * h * byte_per_pixel ];
	for ( int y = 0; y < h; y++ ) {
		line = xpm[ y + line_index ];
		for ( int x = 0; x < w; x++ ) {
			if ( color_data_per_pixel == 1 ) {
				Pixel pixel = colors[ line.substr( x * char_per_pixel, char_per_pixel ) ];
				rbyte r = char_per_r == 2 ? rbyte(pixel.r) : rbyte(pixel.r >> 8);
				rbyte g = char_per_r == 2 ? rbyte(pixel.g) : rbyte(pixel.g >> 8);
				rbyte b = char_per_r == 2 ? rbyte(pixel.b) : rbyte(pixel.b >> 8);
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 0 ] = b;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 1 ] = g;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 2 ] = r;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 3 ] = 0;
			} else if ( color_data_per_pixel == 3 ) {
				Pixel pixel = colors[ line.substr( x * char_per_pixel * color_data_per_pixel, char_per_pixel ) ];
				rbyte r = char_per_r == 2 ? rbyte(pixel.r) : rbyte(pixel.r >> 8);
				pixel = colors[ line.substr( x * char_per_pixel * color_data_per_pixel + char_per_pixel, char_per_pixel ) ];
				rbyte g = char_per_r == 2 ? rbyte(pixel.r) : rbyte(pixel.r >> 8);
				pixel = colors[ line.substr( x * char_per_pixel * color_data_per_pixel + char_per_pixel + char_per_pixel, char_per_pixel ) ];
				rbyte b = char_per_r == 2 ? rbyte(pixel.r) : rbyte(pixel.r >> 8);
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 0 ] = b;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 1 ] = g;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 2 ] = r;
				data[ y * w_align * byte_per_pixel + x * byte_per_pixel + 3 ] = 0;
			} else if ( color_data_per_pixel == 2 ) {
				Pixel pixel1 = colors[ line.substr( x * char_per_pixel * color_data_per_pixel, char_per_pixel ) ];
				Pixel pixel2 = colors[ line.substr( x * char_per_pixel * color_data_per_pixel + char_per_pixel, char_per_pixel ) ];
				rbyte byte1 = char_per_r == 2 ? rbyte(pixel1.r) : rbyte(pixel1.r >> 8);
				rbyte byte2 = char_per_r == 2 ? rbyte(pixel2.r) : rbyte(pixel2.r >> 8);
				rbyte r = byte2 & 0x7C;
				rbyte b = byte1 & 0x1F;
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

	BITMAPINFO bitmap_info;
	bitmap_info.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	bitmap_info.bmiHeader.biWidth         = w_align;
	bitmap_info.bmiHeader.biHeight        = -h;
	bitmap_info.bmiHeader.biPlanes        = 1;
	bitmap_info.bmiHeader.biBitCount      = WORD(byte_per_pixel * 8);
	bitmap_info.bmiHeader.biCompression   = BI_RGB;
	bitmap_info.bmiHeader.biSizeImage     = 0;
	bitmap_info.bmiHeader.biXPelsPerMeter = 0;
	bitmap_info.bmiHeader.biYPelsPerMeter = 0;
	bitmap_info.bmiHeader.biClrUsed       = 0;
	bitmap_info.bmiHeader.biClrImportant  = 0;
	::SetDIBits( desktopDC->m_hDC, static_cast<HBITMAP>(memBMP.m_hObject), 0, h, data, &bitmap_info, DIB_RGB_COLORS );
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

int RPPixmap::getNextInt( rp::string& line ) const
{
	line.trim_left();
	tstring::size_type pos = line.find( ' ' );
	rp::string str = line.substr( 0, pos );
	line.erase( 0, pos + 1 );
	return str.toint();
}

rp::string RPPixmap::getNextStr( rp::string& line ) const
{
	line.trim_left();
	tstring::size_type pos = line.find( ' ' );
	rp::string str = line.substr( 0, pos );
	line.erase( 0, pos + 1 );
	return str;
}

rp::string RPPixmap::getNextColor( rp::string& line ) const
{
	rp::string str = line.substr( 0, char_per_pixel );
	line.erase( 0, char_per_pixel + 1 );
	return str;
}

RPPixmap::Pixel RPPixmap::getNextPixel( rp::string& line )
{
	line.trim_left();
//	tstring::size_type pos = line.find( ' ' );
//	rp::string str = line.substr( 0, pos );
//	line.erase( 0, pos + 1 );
	rp::string str = line;
	if ( str == "None" ) {
		return Pixel( GetRValue(transparent) + (GetRValue(transparent) << 8), GetGValue(transparent) + (GetGValue(transparent) << 8), GetBValue(transparent) + (GetBValue(transparent) << 8) );
	} else {
		str.erase( 0, 1 );
		if ( !char_per_r ) char_per_r = str.size() / 3;
		rp::string r = str.substr( char_per_r * 0, char_per_r );
		rp::string g = str.substr( char_per_r * 1, char_per_r );
		rp::string b = str.substr( char_per_r * 2, char_per_r );
		return Pixel( r.tohex(), g.tohex(), b.tohex() );
	}
}

RPPixmap::RPPixmap( HICON icon ):
	xpm( NULL ),
	transparent( ::GetSysColor( COLOR_BTNFACE ) )
{
	bmp_info.bmWidth  = -1;
	bmp_info.bmHeight = -1;
	CDC* desktopDC = CWnd::GetDesktopWindow()->GetDC();
	CDC dc;
	dc.CreateCompatibleDC( desktopDC );
	if ( dc ) {
		ICONINFO IconInfo;
		::GetIconInfo( icon, &IconInfo );
		BITMAP bm;
		::GetObject( IconInfo.hbmColor, sizeof(bm), &bm );
		bmp.CreateCompatibleBitmap( desktopDC, bm.bmWidth, bm.bmHeight );
		HGDIOBJ bmp_old = dc.SelectObject( &bmp );
		dc.DrawIcon( 0, 0, icon );
		dc.SelectObject( bmp_old );
	}
	CWnd::GetDesktopWindow()->ReleaseDC( desktopDC );
}

RPPixmap::RPPixmap( ruint resource, COLORREF _transparent ):
	xpm( NULL ),
	transparent( _transparent )
{
	bmp_info.bmWidth  = -1;
	bmp_info.bmHeight = -1;
	bmp.LoadBitmap( resource );
}

int RPPixmap::getWidth()
{
	if ( bmp_info.bmWidth == -1 ) {
		if ( !bmp.GetBitmap( &bmp_info ) ) {
			bmp_info.bmWidth = -1;
		}
	}
	return bmp_info.bmWidth;
}

int RPPixmap::getHeight()
{
	if ( bmp_info.bmHeight == -1 ) {
		if ( !bmp.GetBitmap( &bmp_info ) ) {
			bmp_info.bmHeight = -1;
		}
	}
	return bmp_info.bmHeight;
}

HICON RPPixmap::getIcon()
{
	CCloneBitmap clone_bmp;
	clone_bmp.Clone( bmp );
	return clone_bmp.MakeIcon( IRGB(GetRValue(transparent),GetGValue(transparent),GetBValue(transparent)) );
}

void RPPixmap::Draw( HDC hdc, int x, int y, int cx )
{
	HDC dc = ::CreateCompatibleDC( hdc );
	if ( dc ) {
		HGDIOBJ bmp_old = ::SelectObject( dc, getBitmap() );
		int w = getWidth();
		int h = getWidth();
		if ( cx > w || !cx ) cx = w;
		McTransparentBlt( hdc, x, y, cx, h, dc, 0, 0, cx, h, transparent );
		::SelectObject( dc, bmp_old );
		::DeleteDC( dc );
	}
}

void RPPixmap::Clone( HDC hdc )
{
	HDC dc = ::CreateCompatibleDC( hdc );
	if ( dc ) {
		HGDIOBJ bmp_old = ::SelectObject( dc, getBitmap() );
		int w = getWidth();
		int h = getHeight();
		::BitBlt( hdc, 0, 0, w, h, dc, 0, 0, SRCCOPY );
		::SelectObject( dc, bmp_old );
		::DeleteDC( dc );
	}
}

void RPPixmap::CloneScale( HDC hdc, int new_w, int new_h )
{
	HDC dc = ::CreateCompatibleDC( hdc );
	if ( dc ) {
		HGDIOBJ bmp_old = ::SelectObject( dc, getBitmap() );
		int w = getWidth();
		int h = getHeight();
		::StretchBlt( hdc, 0, 0, new_w, new_h, dc, 0, 0, w, h, SRCCOPY );
		::SelectObject( dc, bmp_old );
		::DeleteDC( dc );
	}
}
