#include "app/rdo_studio_mfc/rdo_process/rp_method/stdafx.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object_pixmap.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/mctranspblt/McTransparentBlit.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_pixmap.h"
#include "app/rdo_studio_mfc/rdo_process/rp_misc/rdoprocess_xml.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RPObjectPixmap
// --------------------------------------------------------------------------------
RPObjectPixmap::RPObjectPixmap( RPObject* _parent, RPPixmap* _pixmap ):
	RPShape( _parent, _T("pixmap") ),
	pixmap( _pixmap )
{
	if ( pixmap ) {
		int w = pixmap->getWidth();
		int h = pixmap->getHeight();
		pa_src.push_back( rp::point( -w/2, -h/2 ) );
		pa_src.push_back( rp::point( w/2, -h/2 ) );
		pa_src.push_back( rp::point( w/2, h/2 ) );
		pa_src.push_back( rp::point( -w/2, h/2 ) );
		pa_src.push_back( rp::point( -w/2, -h/2 ) );
	}
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
	RPShape::load( node );
}

rp::RPXMLNode* RPObjectPixmap::save( rp::RPXMLNode* parent_node )
{
	rp::RPXMLNode* obj_node = RPShape::save( parent_node );
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
	rbyte b;
	rbyte g;
	rbyte r;
	rbyte pad;
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
HGDIOBJ RotateMemoryDC(HBITMAP hBmpSrc, CDC& hdcSrc, int SrcW, int SrcH, float angle, CDC& hdcDst, int &dstX, int &dstY, COLORREF transparent )
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
	CtX = ((float) SrcW) / 2;
	CtY = ((float) SrcH) / 2;

	// First, calculate the destination positions for the four courners to get dstX and dstY
	cA = (float) cos(angle);
	sA = (float) -sin(angle);

	x1 = CtX + (-CtX) * cA - (-CtY) * sA;
	x2 = CtX + (SrcW - CtX) * cA - (-CtY) * sA;
	x3 = CtX + (SrcW - CtX) * cA - (SrcH - CtY) * sA;
	x4 = CtX + (-CtX) * cA - (SrcH - CtY) * sA;

	y1 = CtY + (-CtY) * cA + (-CtX) * sA;
	y2 = CtY + (SrcH - CtY) * cA + (-CtX) * sA;
	y3 = CtY + (SrcH - CtY) * cA + (SrcW - CtX) * sA;
	y4 = CtY + (-CtY) * cA + (SrcW - CtX) * sA;

	OfX = ((int) floor(min4(x1, x2, x3, x4)));
	OfY = ((int) floor(min4(y1, y2, y3, y4)));
	
	dstX = ((int) ceil(max4(x1, x2, x3, x4))) - OfX;
	dstY = ((int) ceil(max4(y1, y2, y3, y4))) - OfY;

	// Create the new memory DC
	hBmpDst = CreateCompatibleBitmap(hdcSrc.m_hDC, dstX, dstY);
	HGDIOBJ old_pixmap = ::SelectObject( hdcDst.m_hDC, hBmpDst );

	// Fill the new memory DC with the current Window color
	rt.left = 0;
	rt.top = 0;
	rt.right = dstX;
	rt.bottom = dstY;
	hdcDst.FillSolidRect( &rt, transparent );

	// Get the bitmap bits for the source and destination
	src = MyGetDibBits(hdcSrc.m_hDC, hBmpSrc, SrcW, SrcH);
	dst = MyGetDibBits(hdcDst.m_hDC, hBmpDst, dstX, dstY);

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
			if ((iorgX >= 0) && (iorgY >= 0) && (iorgX < SrcW) && (iorgY < SrcH)) {
				// Inside the source bitmap -> copy the bits
				dstLine[stepX] = src[iorgX + iorgY * SrcW];
			} else {
				// Outside the source -> set the color to light grey
				dstLine[stepX].b = GetBValue(transparent);
				dstLine[stepX].g = GetGValue(transparent);
				dstLine[stepX].r = GetRValue(transparent);
			}
		}
		dstLine = dstLine + dstX;
	}

	// Set the new Bitmap
	bi.bmiHeader.biSize         = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth        = dstX;
	bi.bmiHeader.biHeight       = -dstY;
	bi.bmiHeader.biPlanes       = 1;
	bi.bmiHeader.biBitCount     = 32;
	bi.bmiHeader.biCompression  = BI_RGB;
	bi.bmiHeader.biSizeImage    = dstX * 4 * dstY;
	bi.bmiHeader.biClrUsed      = 0;
	bi.bmiHeader.biClrImportant = 0;
	SetDIBits(hdcDst, hBmpDst, 0, dstY, dst, &bi, DIB_RGB_COLORS);
	DeleteObject(hBmpDst);

	// Free the color arrays
	free(src);
	free(dst);

	return old_pixmap;
}

void RPObjectPixmap::draw( CDC& dc )
{
	RPObjectMatrix::draw( dc );

	// Перевод фигуры в глобальные координаты
	transformToGlobal();
	rp::rect rect = pa_global.getBoundingRect();

	CDC original_dc;
	original_dc.CreateCompatibleDC( &dc );
	CBitmap* original_old_bitmap = original_dc.SelectObject( &pixmap->getCBitmap() );
	int w = pixmap->getWidth();
	int h = pixmap->getHeight();
	int rotated_w;
	int rotated_h;
	CDC rotated_dc;
	rotated_dc.CreateCompatibleDC( &dc );
	HGDIOBJ rotated_old_pixmap = RotateMemoryDC( pixmap->getBitmap(), original_dc, w, h, float(getRotation() *  rp::math::pi / 180), rotated_dc, rotated_w, rotated_h, RGB(0,-1,0)/*pixmap->getTransparent()*/ );
	McTransparentBlt( dc.m_hDC, int(rect.p0().x), int(rect.p0().y), int(rotated_w * getScaleX()), int(rotated_h * getScaleY()), rotated_dc.m_hDC, 0, 0, rotated_w, rotated_h, pixmap->getTransparent() );
	::SelectObject( rotated_dc.m_hDC, rotated_old_pixmap );
	original_dc.SelectObject( original_old_bitmap );
	rect.draw( dc );


/*
	// Вывод картинки
	int w = pixmap->getWidth();
	int h = pixmap->getHeight();
	int new_w = w * getScaleX();
	int new_h = h * getScaleY();
	CDC dc_rotate_src;
	dc_rotate_src.CreateCompatibleDC( &dc );
	CBitmap pixmap_scale;
	pixmap_scale.CreateCompatibleBitmap( &dc, new_w, new_h );
	CBitmap* old_bitmap_src = dc_rotate_src.SelectObject( &pixmap_scale );
	pixmap->CloneScale( dc_rotate_src.m_hDC, new_w, new_h );

	CDC dc_rotate_dest;
	dc_rotate_dest.CreateCompatibleDC( &dc );

	int rotated_w;
	int rotated_h;
	HGDIOBJ old_pixmap_dst = RotateMemoryDC( pixmap_scale, dc_rotate_src, new_w, new_h, getRotation() *  rp::math::pi / 180, dc_rotate_dest, rotated_w, rotated_h, pixmap->getTransparent() );
	McTransparentBlt( dc.m_hDC, rect.p0().x, rect.p0().y, rotated_w, rotated_h, dc_rotate_dest.m_hDC, 0, 0, rotated_w, rotated_h, pixmap->getTransparent() );
//	::BitBlt( dc.m_hDC, rect.p0().x, rect.p0().y, new_w, new_h, dc_rotate_src.m_hDC, 0, 0, SRCCOPY );
	dc_rotate_src.SelectObject( old_bitmap_src );
	::SelectObject( dc_rotate_dest.m_hDC, old_pixmap_dst );
*/
}
