#ifndef RDOSTUDIOFRAMEMANAGER_H
#define RDOSTUDIOFRAMEMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioframedoc.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameManager
// ----------------------------------------------------------------------------
class RDOStudioFrameDoc;
class RDOStudioFrameView;
namespace RDOSimulatorNS {
	struct RDOFrame;
}

class RDOStudioFrameManager
{
private:

	class FrameDocTemplate: public CMultiDocTemplate {
	friend class RDOStudioFrameManager;
	private:
		FrameDocTemplate( UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass ): CMultiDocTemplate( nIDResource, pDocClass, pFrameClass, pViewClass ) {
		};
		virtual CFrameWnd* CreateNewFrame( CDocument* pDoc, CFrameWnd* pOther ) {
			CFrameWnd* frame = CMultiDocTemplate::CreateNewFrame( pDoc, pOther );
			static_cast<RDOStudioFrameDoc*>(pDoc)->frame = frame;
			return frame;
		}
	};
	FrameDocTemplate* frameDocTemplate;

	class BMPReadError {
	};

	class Frame {
	friend class RDOStudioFrameManager;
	private:
		Frame(): hitem( 0 ), doc( NULL ), view( NULL ), timer( false, true ) {};
		HTREEITEM           hitem;
		std::string         name;
		RDOStudioFrameDoc*  doc;
		RDOStudioFrameView* view;
		CMutex              used;
		CMutex              draw;
		CEvent              timer;
		CEvent              close;
	};
	static std::vector< Frame* > frames;

	class BMP {
	friend class RDOStudioFrameManager;
	private:
		BMP(): w( 0 ), h( 0 ) {};
		CBitmap bmp;
		int w;
		int h;
	};
	std::map< std::string, BMP* > bitmaps;
	CDC dcBmp;
	CDC dcMask;

	int lastShowedFrame;

public:
	RDOStudioFrameManager();
	virtual ~RDOStudioFrameManager();

	void insertItem( const std::string& name );
	int findFrameIndex( const HTREEITEM hitem ) const;
	int findFrameIndex( const RDOStudioFrameDoc* doc ) const;
	int findFrameIndex( const RDOStudioFrameView* view ) const;
	RDOStudioFrameDoc* connectFrameDoc( const int index );
	void disconnectFrameDoc( const RDOStudioFrameDoc* doc ) const;
	std::string         getFrameName( const int index ) const    { return frames[index]->name;   };
	RDOStudioFrameDoc*  getFrameDoc( const int index ) const     { return frames[index]->doc;    };
	RDOStudioFrameView* getFrameView( const int index ) const    { return frames[index]->view;   };
	CMutex*             getFrameUsed( const int index ) const    { return &frames[index]->used;  };
	CMutex*             getFrameDraw( const int index ) const    { return &frames[index]->draw;  };
	CEvent*             getFrameTimer( const int index ) const   { return &frames[index]->timer; };
	CEvent*             getFrameClose( const int index ) const   { return &frames[index]->close; };
	int count() const                                            { return frames.size();         };
	RDOStudioFrameDoc* getFirstExistDoc() const;
	void closeAll();
	void clear();

	void bmp_insert( const std::string& name );
	void bmp_clear();

	void expand() const;

	int getLastShowedFrame() const              { return lastShowedFrame; };
	void setLastShowedFrame( const int value );

	bool isValidFrameDoc( const RDOStudioFrameDoc* const frame ) const;

	void showFrame( std::vector<RDOSimulatorNS::RDOFrame *>::const_iterator& frame, const int index );
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOFRAMEMANAGER_H
