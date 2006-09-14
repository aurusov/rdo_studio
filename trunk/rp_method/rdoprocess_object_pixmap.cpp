#include "stdafx.h"
#include "rdoprocess_object_pixmap.h"
#include <rdoprocess_pixmap.h>
#include <rdoprocess_xml.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RPObjectPixmap
// ----------------------------------------------------------------------------
RPObjectPixmap::RPObjectPixmap( RPObject* _parent, RPPixmap* _pixmap ):
	RPShape( _parent, _T("pixmap") ),
	pixmap( _pixmap )
{
	int w = pixmap->getWidth();
	int h = pixmap->getHeight();
	pa_src.push_back( rp::point( -w/2, -h/2 ) );
	pa_src.push_back( rp::point( w/2, -h/2 ) );
	pa_src.push_back( rp::point( w/2, h/2 ) );
	pa_src.push_back( rp::point( -w/2, h/2 ) );
	pa_src.push_back( rp::point( -w/2, -h/2 ) );
}

RPObjectPixmap::~RPObjectPixmap()
{
	if ( pixmap ) {
		delete pixmap;
		pixmap = NULL;
	}
}

void RPObjectPixmap::load( rp::RPXMLNode* node )
{
}

rp::RPXMLNode* RPObjectPixmap::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = parent_node->makeChild( "pixmap" );
	return obj_node;
}

void RPObjectPixmap::setPosition( double posx, double posy )
{
	RPShape::setPosition( posx, posy );
}

// Исходники поворота картинки взяты с
// http://www.codeguru.com/cpp/g-m/gdi/article.php/c3693/
//
// Helper function for getting the minimum of 4 floats
float min4(float a, float b, float c, float d)
{
	if (a < b) {
		if (c < a) {
			if (d < c)
				return d;
			else
				return c;
		} else {
			if (d < a)
				return d;
			else
				return a;
		}
	} else {
		if (c < b) {
			if (d < c)
				return d;
			else
				return c;
		} else {
			if (d < b)
				return d;
			else
				return b;
		}
	}
}

// Helper function for getting the maximum of 4 floats
float max4(float a, float b, float c, float d)
{
	if (a > b) {
		if (c > a) {
			if (d > c)
				return d;
			else
				return c;
		} else {
			if (d > a)
				return d;
			else
				return a;
		}
	} else {
		if (c > b) {
			if (d > c)
				return d;
			else
				return c;
		} else {
			if (d > b)
				return d;
			else
				return b;
		}
	}
}

// The Representation of a 32 bit color table entry
#pragma pack(push)
#pragma pack(1)
typedef struct ssBGR {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char pad;
} sBGR;

typedef sBGR *pBGR;
#pragma pack(pop)


// Returns the DI (Device Independent) bits of the Bitmap
// Here I use 32 bit since it's easy to adress in memory and no
// padding of the horizontal lines is required.
pBGR MyGetDibBits(HDC hdcSrc, HBITMAP hBmpSrc, int nx, int ny)
{
	BITMAPINFO bi;
	BOOL bRes;
	pBGR buf;

	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = nx;
	bi.bmiHeader.biHeight = - ny;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = nx * 4 * ny;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;
	
	buf = (pBGR) malloc(nx * 4 * ny);
	bRes = GetDIBits(hdcSrc, hBmpSrc, 0, ny, buf, &bi, DIB_RGB_COLORS);
	if (!bRes) {
		free(buf);
		buf = 0;
	}
	return buf;
}

// RotateMemoryDC rotates a memory DC and returns the rotated DC as well as its dimensions
void RotateMemoryDC(HBITMAP hBmpSrc, HDC hdcSrc, int SrcX, int SrcY, float angle, HDC &hdcDst, int &dstX, int &dstY)
{
	HBITMAP hBmpDst;
	float x1, x2, x3, x4, y1, y2, y3, y4, cA, sA;
	float CtX, CtY, orgX, orgY, divisor;
	int OfX, OfY;
	int stepX, stepY;
	int iorgX, iorgY;
	RECT rt;
	pBGR src, dst, dstLine;
	BITMAPINFO bi;

	// Rotate the bitmap around the center
	CtX = ((float) SrcX) / 2;
	CtY = ((float) SrcY) / 2;

	// First, calculate the destination positions for the four courners to get dstX and dstY
	cA = (float) cos(angle);
	sA = (float) sin(angle);

	x1 = CtX + (-CtX) * cA - (-CtY) * sA;
	x2 = CtX + (SrcX - CtX) * cA - (-CtY) * sA;
	x3 = CtX + (SrcX - CtX) * cA - (SrcY - CtY) * sA;
	x4 = CtX + (-CtX) * cA - (SrcY - CtY) * sA;

	y1 = CtY + (-CtY) * cA + (-CtX) * sA;
	y2 = CtY + (SrcY - CtY) * cA + (-CtX) * sA;
	y3 = CtY + (SrcY - CtY) * cA + (SrcX - CtX) * sA;
	y4 = CtY + (-CtY) * cA + (SrcX - CtX) * sA;

	OfX = ((int) floor(min4(x1, x2, x3, x4)));
	OfY = ((int) floor(min4(y1, y2, y3, y4)));
	
	dstX = ((int) ceil(max4(x1, x2, x3, x4))) - OfX;
	dstY = ((int) ceil(max4(y1, y2, y3, y4))) - OfY;

	// Create the new memory DC
	hdcDst = CreateCompatibleDC(hdcSrc);
	hBmpDst = CreateCompatibleBitmap(hdcSrc, dstX, dstY);
	SelectObject(hdcDst, hBmpDst);

	// Fill the new memory DC with the current Window color
	rt.left = 0;
	rt.top = 0;
	rt.right = dstX;
	rt.bottom = dstY;
	FillRect(hdcDst, &rt, GetSysColorBrush(COLOR_WINDOW));

	// Get the bitmap bits for the source and destination
	src = MyGetDibBits(hdcSrc, hBmpSrc, SrcX, SrcY);
	dst = MyGetDibBits(hdcDst, hBmpDst, dstX, dstY);

	dstLine = dst;
	divisor = cA*cA + sA*sA;
	// Step through the destination bitmap
	for (stepY = 0; stepY < dstY; stepY++) {
		for (stepX = 0; stepX < dstX; stepX++) {
			// Calculate the source coordinate
			orgX = (cA * (((float) stepX + OfX) + CtX * (cA - 1)) + sA * (((float) stepY + OfY) + CtY * (sA - 1))) / divisor;
			orgY = CtY + (CtX - ((float) stepX + OfX)) * sA + cA *(((float) stepY + OfY) - CtY + (CtY - CtX) * sA);
			iorgX = (int) orgX;
			iorgY = (int) orgY;
			if ((iorgX >= 0) && (iorgY >= 0) && (iorgX < SrcX) && (iorgY < SrcY)) {
				// Inside the source bitmap -> copy the bits
				dstLine[dstX - stepX - 1] = src[iorgX + iorgY * SrcX];
			} else {
				// Outside the source -> set the color to light grey
				dstLine[dstX - stepX - 1].b = 240;
				dstLine[dstX - stepX - 1].g = 240;
				dstLine[dstX - stepX - 1].r = 240;
			}
		}
		dstLine = dstLine + dstX;
	}

	// Set the new Bitmap
	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = dstX;
	bi.bmiHeader.biHeight = dstY;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = dstX * 4 * dstY;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;
	SetDIBits(hdcDst, hBmpDst, 0, dstY, dst, &bi, DIB_RGB_COLORS);
	DeleteObject(hBmpDst);

	// Free the color arrays
	free(src);
	free(dst);
}

void RPObjectPixmap::draw( CDC& dc )
{
	RPShape::draw( dc );

	// Перевод фигуры в глобальные координаты
	transformToGlobal();

	// Вывод картинки
	CDC dc_rotate_src;
	dc_rotate_src.CreateCompatibleDC( &dc );
	CBitmap* old_bitmap = dc_rotate_src.SelectObject( &pixmap->getCBitmap() );
	HDC dc_rotate_dest;
	int x = 32;
	int y = 32;
	RotateMemoryDC( pixmap->getBitmap(), dc_rotate_src.m_hDC, 32, 32, (getRotation() + 180) *  rp::math::pi / 180, dc_rotate_dest, x, y );
	::BitBlt( dc.m_hDC, pa_global[0].x, pa_global[0].y, 32, 32, dc_rotate_dest, 0, 0, SRCCOPY );
	dc_rotate_src.SelectObject( old_bitmap );

//	pixmap->Draw( dc.m_hDC, pa_global[0].x, pa_global[0].y );
}
