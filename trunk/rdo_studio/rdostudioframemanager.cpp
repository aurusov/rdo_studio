#include "stdafx.h"
#include "rdostudioframemanager.h"
#include "rdostudiomodel.h"
#include "rdostudioframedoc.h"
#include "rdostudioframeview.h"
#include "rdostudioapp.h"
#include "rdostudiomainfrm.h"
#include "rdostudiochildfrm.h"
#include "rdostudioworkspace.h"
#include "rdostudioframetreectrl.h"
#include "resource.h"

#include <rdokernel.h>
#include <rdorepository.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameManager
// ----------------------------------------------------------------------------
vector< RDOStudioFrameManager::Frame* > RDOStudioFrameManager::frames;

RDOStudioFrameManager::RDOStudioFrameManager():
	frameDocTemplate( NULL ),
	lastShowedFrame( -1 )
{
	frameDocTemplate = new CMultiDocTemplate( IDR_FRAMETYPE, RUNTIME_CLASS(RDOStudioFrameDoc), RUNTIME_CLASS(RDOStudioChildFrame), RUNTIME_CLASS(RDOStudioFrameView) );
	AfxGetApp()->AddDocTemplate( frameDocTemplate );
}

RDOStudioFrameManager::~RDOStudioFrameManager()
{
	bmp_clear();
	vector< Frame* >::iterator it = frames.begin();
	while ( it != frames.end() ) {
		delete *it++;
	};
}

void RDOStudioFrameManager::insertItem( const string& name )
{
	Frame* item = new Frame;
	item->hitem = studioApp.mainFrame->workspace.frames->InsertItem( name.c_str(), 1, 1, studioApp.mainFrame->workspace.frames->GetRootItem() );
	item->name  = name;
	item->doc   = NULL;
	item->view  = NULL;
	frames.push_back( item );
}

int RDOStudioFrameManager::findFrameIndex( const HTREEITEM hitem ) const
{
	vector< Frame* >::iterator it = frames.begin();
	int index = 0;
	while ( it != frames.end() ) {
		if ( (*it)->hitem == hitem ) {
			return index;
		}
		it++;
		index++;
	};
	return -1;
}

int RDOStudioFrameManager::findFrameIndex( const RDOStudioFrameDoc* doc ) const
{
	vector< Frame* >::iterator it = frames.begin();
	int index = 0;
	while ( it != frames.end() ) {
		if ( (*it)->doc == doc ) {
			return index;
		}
		it++;
		index++;
	};
	return -1;
}

int RDOStudioFrameManager::findFrameIndex( const RDOStudioFrameView* view ) const
{
	vector< Frame* >::iterator it = frames.begin();
	int index = 0;
	while ( it != frames.end() ) {
		if ( (*it)->view == view ) {
			return index;
		}
		it++;
		index++;
	};
	return -1;
}

RDOStudioFrameDoc* RDOStudioFrameManager::connectFrameDoc( const int index )
{
	RDOStudioFrameDoc* doc = NULL;
	if ( index != -1 ) {
		CSingleLock lock( getFrameUsed( index ) );
		lock.Lock();

		doc = static_cast<RDOStudioFrameDoc*>(frameDocTemplate->OpenDocumentFile( NULL ));
		frames[index]->doc  = doc;
		frames[index]->view = doc->getView();
		lastShowedFrame     = index;

		lock.Unlock();
	}
	return doc;
}

void RDOStudioFrameManager::disconnectFrameDoc( const RDOStudioFrameDoc* doc ) const
{
	int index = findFrameIndex( doc );
	if ( index != -1 ) {

		CSingleLock lock( getFrameUsed( index ) );
		lock.Lock();

		frames[index]->doc  = NULL;
		frames[index]->view = NULL;

		lock.Unlock();
	}
}

void RDOStudioFrameManager::closeAll()
{
	int backup = lastShowedFrame;
	vector< Frame* >::iterator it = frames.begin();
	while ( it != frames.end() ) {
		if ( isValidFrameDoc( (*it)->doc ) ) {
			(*it)->doc->OnCloseDocument();
		}
		it++;
	};
	lastShowedFrame = backup;
}

void RDOStudioFrameManager::clear()
{
	studioApp.mainFrame->workspace.frames->deleteChildren( studioApp.mainFrame->workspace.frames->GetRootItem() );
	vector< Frame* >::iterator it = frames.begin();
	while ( it != frames.end() ) {
		if ( isValidFrameDoc( (*it)->doc ) ) {
			(*it)->doc->OnCloseDocument();
		}
		delete *it++;
	};
	frames.clear();
	lastShowedFrame = -1;
}

RDOStudioFrameDoc* RDOStudioFrameManager::getFirstExistDoc() const
{
	vector< Frame* >::const_iterator it = frames.begin();
	while ( it != frames.end() ) {
		if ( isValidFrameDoc( (*it)->doc ) ) {
			return (*it)->doc;
		}
		it++;
	};
	return NULL;
}

void RDOStudioFrameManager::expand() const
{
	studioApp.mainFrame->workspace.frames->expand();
}

void RDOStudioFrameManager::setLastShowedFrame( const int value )
{
	if ( lastShowedFrame >= 0 && lastShowedFrame < count() ) {
		lastShowedFrame = value;
	}
}

bool RDOStudioFrameManager::isValidFrameDoc( const RDOStudioFrameDoc* const frame ) const
{
	POSITION pos = frameDocTemplate->GetFirstDocPosition();
	while ( pos ) {
		RDOStudioFrameDoc* doc = static_cast<RDOStudioFrameDoc*>(frameDocTemplate->GetNextDoc( pos ));
		if ( frame == doc ) {
			return true;
		}
	}

	return false;
}

int RDOStudioFrameManager::getNumColors( BITMAPINFOHEADER* pBMIH ) const
{
	int colors = 0;
	switch ( pBMIH->biBitCount ) {
		case  1: colors = 2;   break;
		case  4: colors = 16;  break;
		case  8: colors = 256; break;
	}
	int max = colors;
	if ( pBMIH->biClrUsed ) colors = pBMIH->biClrUsed;
	// Не надо выходить за размеры палитры
	if ( max && colors > max ) colors = max;
	return colors;
}

void RDOStudioFrameManager::bmp_insert( const std::string& name )
{
	BMP* bmp = new BMP;
	bmp->name = name;
	bitmaps.push_back( bmp );

	stringstream stream;
	kernel.getRepository()->loadBMP( name, stream );

	// В потоке, перед битовой картой, идет заголовок файла битовой карты
	BITMAPFILEHEADER bmFileHeader;
	stream.read( reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader) );

	// Проверяем заголовок битовой карты на магическое число "BM"
	if ( bmFileHeader.bfType == 0x4D42 ) {

		// Вот теперь читаем сам заголовок битовой карты
		BITMAPINFOHEADER bmInfoHeader;
		stream.read( reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader) );

		RGBQUAD rgb_q[256];
		memset( &rgb_q, 0, sizeof(rgb_q) );

		if ( bmInfoHeader.biSize == sizeof(BITMAPINFOHEADER) ) {
			// Читаем палитру из потока
			bmInfoHeader.biClrUsed = getNumColors( &bmInfoHeader );
			int size_ColorTable = bmInfoHeader.biClrUsed * sizeof(RGBQUAD);
			stream.read( reinterpret_cast<char*>(&rgb_q), size_ColorTable );
		} else {
			// Если ПОЛЕ 'размер', только что прочитанной структуры, не соврадает
			// с sizeof(BITMAPINFOHEADER), то остаются два варианта:
			// 1. Это была структура BITMAPCOREHEADER, которая меньше по размеру,
			//    но она нас всетаки устраивает, т.к. мы можем сами дополнить необходимые
			//    поля структуры BITMAPINFOHEADER
			// 2. Это не файл битовой карты, что никуда не годится -> throw
			// Ну а если ПОЛЕ 'размер' соврадает, то мы прочитали то, что нам нужно и
			// не выполняем этот блок.
			if ( bmInfoHeader.biSize == sizeof(BITMAPCOREHEADER) ) {
				// Необходима конвертация файла PM в DIB
				// Откатываемся в потоке на начало структуры BITMAPCOREHEADER и читаем ее
				stream.seekg( sizeof(BITMAPFILEHEADER), ios::beg );
				BITMAPCOREHEADER bmCoreHeader;
				stream.read( reinterpret_cast<char*>(&bmCoreHeader), sizeof(bmCoreHeader) );
				bmInfoHeader.biSize     = sizeof(BITMAPINFOHEADER);
				bmInfoHeader.biWidth    = bmCoreHeader.bcWidth;
				bmInfoHeader.biHeight   = bmCoreHeader.bcHeight;
				bmInfoHeader.biPlanes   = bmCoreHeader.bcPlanes;
				bmInfoHeader.biBitCount = bmCoreHeader.bcBitCount;
				bmInfoHeader.biCompression = BI_RGB;
				bmInfoHeader.biSizeImage     = 0;
				bmInfoHeader.biXPelsPerMeter = 0;
				bmInfoHeader.biYPelsPerMeter = 0;
				bmInfoHeader.biClrUsed       = getNumColors(&bmInfoHeader);
				if ( bmInfoHeader.biClrUsed <= 256 ) {
					bmInfoHeader.biClrImportant = 0;
					// Читаем каждый PM цевт и переводим его в DIB
					RGBTRIPLE rgb_t;
					for (int i = 0; i < (int)bmInfoHeader.biClrUsed; i++) {
						stream.read( reinterpret_cast<char*>(&rgb_t), sizeof(RGBTRIPLE) );
						rgb_q[i].rgbRed      = rgb_t.rgbtRed;
						rgb_q[i].rgbGreen    = rgb_t.rgbtGreen;
						rgb_q[i].rgbBlue     = rgb_t.rgbtBlue;
						rgb_q[i].rgbReserved = 0;
					}
				}
			}
		}
		BITMAP hBmp;
		hBmp.bmType       = 0;
		hBmp.bmWidth      = bmInfoHeader.biWidth;
		hBmp.bmHeight     = bmInfoHeader.biHeight;
		hBmp.bmWidthBytes = bmInfoHeader.biWidth * bmInfoHeader.biBitCount / 8;
		hBmp.bmPlanes     = bmInfoHeader.biPlanes;
		hBmp.bmBitsPixel  = bmInfoHeader.biBitCount;
/*
		char* pBits = new char[ hBmp.bmWidthBytes * hBmp.bmHeight ];
		hBmp.bmBits = pBits;

		stream.seekg( bmFileHeader.bfOffBits, ios::beg );
		stream.read( pBits, hBmp.bmWidthBytes * hBmp.bmHeight );

		CBitmap cBmp;
		cBmp.CreateBitmapIndirect( &hBmp );
		DWORD a = GetLastError();
*/
	}
/*
      // Теперь можно заняться инициализацией FImage
      FImage.width   = bmInfoHeader.biWidth;
      FImage.height  = bmInfoHeader.biHeight;
      FImage.aspectx = 1;
      FImage.aspecty = 1;
      FImage.depth   = bmInfoHeader.biBitCount;
      FImage.rgb     = bmInfoHeader.biBitCount > 8 ? true : false;
      FImage.bytes_per_line = (FImage.width * FImage.depth) >> 3;
      // Выделяем память под пикселы
      // Надуюсь, что new и в этом случае не возвращает NULL
      FImage.buffer1 = new byte[FImage.bytes_per_line * FImage.height];
      // Копируем пикселы из потока в конец FImage.buffer1 к его началу
      byte* pBits  = (byte*)FImage.buffer1 + FImage.height * FImage.bytes_per_line;
      int bytes_per_line = FImage.bytes_per_line;
      cs.Stream->Seek(StreamStartPos + bmFileHeader.bfOffBits, soFromBeginning);
      for (int i = 0; i < FImage.height; i++) {
        pBits -= bytes_per_line;
        if (cs.Stream->Read(pBits, bytes_per_line) != bytes_per_line)
          throw BaseObjError(sErrorRead_Bits);
      }
      // Заполняем оставшиеся поля структуры
      switch (FImage.depth) {
        case 16: FImage.red_mask     = 0x7C00;
                 FImage.green_mask   = 0x03E0;
                 FImage.blue_mask    = 0x001F;
                 FImage.alpha_mask   = 0x0000;
                 FImage.palette_size = 0;
                 break;
        case 24: FImage.red_mask     = 0xFF0000;
                 FImage.green_mask   = 0x00FF00;
                 FImage.blue_mask    = 0x0000FF;
                 FImage.alpha_mask   = 0x000000;
                 FImage.palette_size = 0;
                 break;
        case 32: FImage.red_mask     = 0x00FF0000;
                 FImage.green_mask   = 0x0000FF00;
                 FImage.blue_mask    = 0x000000FF;
                 FImage.alpha_mask   = 0x00000000;
                 FImage.palette_size = 0;
                 break;
        default: FImage.red_mask     = 0xFF;
                 FImage.green_mask   = 0xFF;
                 FImage.blue_mask    = 0xFF;
                 FImage.alpha_mask   = 0x00000000;
                 FImage.palette_size = GetNumColors(&bmInfoHeader);
                 if (FImage.palette_size > 256) throw BaseObjError(sErrorPaletteCount);
                 break;
      }
      // Копируем палитру, если она есть
      if (FImage.palette_size > 0) {
        FImage.palette = new D3DRMPALETTEENTRY[FImage.palette_size];
        for (int i = 0; i < FImage.palette_size; i++) {
            FImage.palette[i].red   = rgb_q[i].rgbRed;
            FImage.palette[i].green = rgb_q[i].rgbGreen;
            FImage.palette[i].blue  = rgb_q[i].rgbBlue;
            FImage.palette[i].flags = D3DRMPALETTE_READONLY;
        }
      }
    }
*/
	kernel.debug( "%s\r\n", bmp->name.c_str() );
}

void RDOStudioFrameManager::bmp_clear()
{
	vector< BMP* >::iterator it = bitmaps.begin();
	while ( it != bitmaps.end() ) {
		delete *it++;
	};
	bitmaps.clear();
}
