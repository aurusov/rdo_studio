/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/manager.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_FRAME_MANAGER_H_
#define _RDO_STUDIO_MFC_FRAME_MANAGER_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <map>
#include <memory>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/frame/document.h"
#include "app/rdo_studio_mfc/src/frame/view.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameManager
// --------------------------------------------------------------------------------
namespace rdoAnimation {
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

	class Area {
	friend class RDOStudioFrameManager;
	friend class RDOStudioFrameView;
	private:
		tstring name;
		int x;
		int y;
		int w;
		int h;
	};

	class Frame
	{
	friend class RDOStudioFrameManager;
	friend class RDOStudioFrameView;
	private:
		Frame()
			: hitem( 0 )
			, doc( NULL )
			, view( NULL )
		{}
		~Frame()
		{
			areas_sim_clear();
		}

		typedef  std::vector<PTR(Area)>  AreaList;

		HTREEITEM               hitem;
		tstring                 name;
		PTR(RDOStudioFrameDoc)  doc;
		PTR(RDOStudioFrameView) view;
		AreaList                areas_sim;

		void areas_sim_clear()
		{
			STL_FOR_ALL(areas_sim, it)
			{
				delete *it;
			}
			areas_sim.clear();
		}
	};

	typedef  std::vector<PTR(Frame)>                  FrameList;
	typedef  std::map<tstring, PTR(Gdiplus::Bitmap)>  BitmapList;

	FrameList  m_frameList;
	BitmapList m_bitmapList;

	CDC dcBmp;
	CDC dcMask;

	int lastShowedFrame;
	int currentShowingFrame;
	rbool changed;

public:
	RDOStudioFrameManager();
	virtual ~RDOStudioFrameManager();

	void insertItem( CREF(tstring) name );
	int findFrameIndex( const HTREEITEM hitem ) const
	{
		int index = 0;
		STL_FOR_ALL_CONST(m_frameList, it)
		{
			if ((*it)->hitem == hitem)
			{
				return index;
			}
			index++;
		}
		return -1;
	}
	int findFrameIndex( const RDOStudioFrameDoc* doc ) const
	{
		int index = 0;
		STL_FOR_ALL_CONST(m_frameList, it)
		{
			if ((*it)->doc == doc)
			{
				return index;
			}
			index++;
		}
		return -1;
	}
	int findFrameIndex( const RDOStudioFrameView* view ) const {
		int index = 0;
		STL_FOR_ALL_CONST(m_frameList, it)
		{
			if ((*it)->view == view)
			{
				return index;
			}
			index++;
		}
		return -1;
	}
	RDOStudioFrameDoc* connectFrameDoc( const int index );
	void disconnectFrameDoc( const RDOStudioFrameDoc* doc );
	CREF(tstring)       getFrameName( const int index ) const       { return m_frameList[index]->name;   };
	RDOStudioFrameDoc*  getFrameDoc( const int index ) const        { return m_frameList[index]->doc;    };
	RDOStudioFrameView* getFrameView( const int index ) const       { return m_frameList[index]->view;   };
	int count() const                                               { return m_frameList.size();         };
	rbool isChanged()                                               { rbool res = changed; changed = false; return res; }
	RDOStudioFrameDoc* getFirstExistDoc() const;
	void closeAll();
	void clear();

	void bmp_insert( CREF(tstring) name );
	void bmp_clear();

	void expand() const;

	rbool isValidFrameDoc( const RDOStudioFrameDoc* const frame ) const;

	int   getLastShowedFrame() const              { return lastShowedFrame; };
	void  setLastShowedFrame( const int value );
	void  setCurrentShowingFrame( const int value );
	void  resetCurrentShowingFrame( const int value );
	void  showFrame( const rdoAnimation::RDOFrame* const frame, const int index );
	void  showNextFrame();
	void  showPrevFrame();
	void  showFrame( const int index );
	rbool canShowNextFrame() const;
	rbool canShowPrevFrame() const;

	void updateStyles() const;
};

#endif // _RDO_STUDIO_MFC_FRAME_MANAGER_H_
