#ifndef RDOSTUDIOFRAMEMANAGER_H
#define RDOSTUDIOFRAMEMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioframedoc.h"
#include <vector>
#include <map>

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameManager
// ----------------------------------------------------------------------------
class RDOStudioFrameDoc;
class RDOStudioFrameView;
namespace rdosim {
	struct RDOFrame;
}

class RDOStudioFrameManager
{
friend class RDOStudioFrameView;
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

	class Area {
	friend class RDOStudioFrameManager;
	friend class RDOStudioFrameView;
	private:
		std::string name;
		int x;
		int y;
		int w;
		int h;
	};

	class Frame {
	friend class RDOStudioFrameManager;
	friend class RDOStudioFrameView;
	private:
		Frame(): hitem( 0 ), doc( NULL ), view( NULL ), timer( false, true ) {};
		~Frame() { areas_sim_clear(); };
		HTREEITEM           hitem;
		std::string         name;
		RDOStudioFrameDoc*  doc;
		RDOStudioFrameView* view;
		CMutex              used;
		CMutex              draw;
		CEvent              timer;
		CEvent              close;
		std::vector< Area* >       areas_sim;
		std::vector< std::string > areas_clicked;
//		std::vector< int >         keys_pressed;
		void areas_sim_clear() {
			std::vector< Area* >::iterator it = areas_sim.begin();
			while ( it != areas_sim.end() ) {
				delete *it++;
			};
			areas_sim.clear();
		}
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
	int currentShowingFrame;

public:
	RDOStudioFrameManager();
	virtual ~RDOStudioFrameManager();

	void insertItem( const std::string& name );
	int findFrameIndex( const HTREEITEM hitem ) const;
	int findFrameIndex( const RDOStudioFrameDoc* doc ) const;
	int findFrameIndex( const RDOStudioFrameView* view ) const;
	RDOStudioFrameDoc* connectFrameDoc( const int index );
	void disconnectFrameDoc( const RDOStudioFrameDoc* doc ) const;
	const std::string&  getFrameName( const int index ) const       { return frames[index]->name;   };
	RDOStudioFrameDoc*  getFrameDoc( const int index ) const        { return frames[index]->doc;    };
	RDOStudioFrameView* getFrameView( const int index ) const       { return frames[index]->view;   };
	CMutex*             getFrameMutexUsed( const int index ) const  { return &frames[index]->used;  };
	CMutex*             getFrameMutexDraw( const int index ) const  { return &frames[index]->draw;  };
	CEvent*             getFrameEventTimer( const int index ) const { return &frames[index]->timer; };
	CEvent*             getFrameEventClose( const int index ) const { return &frames[index]->close; };
	int count() const                                               { return frames.size();         };
	RDOStudioFrameDoc* getFirstExistDoc() const;
	void closeAll();
	void clear();

	void bmp_insert( const std::string& name );
	void bmp_clear();

	void expand() const;

	bool isValidFrameDoc( const RDOStudioFrameDoc* const frame ) const;

	int getLastShowedFrame() const              { return lastShowedFrame; };
	void setLastShowedFrame( const int value );
	void setCurrentShowingFrame( const int value );
	void resetCurrentShowingFrame( const int value );
	void showFrame( const rdosim::RDOFrame* const frame, const int index );
	void showNextFrame();
	void showPrevFrame();
	void showFrame( const int index );
	bool canShowNextFrame() const;
	bool canShowPrevFrame() const;

	void updateStyles() const;
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOFRAMEMANAGER_H
