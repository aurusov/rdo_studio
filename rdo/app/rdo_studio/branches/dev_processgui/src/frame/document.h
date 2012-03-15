/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/frame/document.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      06.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_FRAME_DOCUMENT_H_
#define _RDO_STUDIO_MFC_FRAME_DOCUMENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/frame/view.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameDoc
// --------------------------------------------------------------------------------
class RDOStudioFrameDoc: public CDocument
{
friend class RDOStudioModel;
friend class RDOStudioFrameView;

DECLARE_DYNCREATE(RDOStudioFrameDoc)

protected:
	RDOStudioFrameDoc();

public:
	virtual ~RDOStudioFrameDoc();

	CFrameWnd* frame;

	RDOStudioFrameView* getView() const {
		POSITION pos = GetFirstViewPosition();
		if ( pos ) {
			return static_cast<RDOStudioFrameView*>(GetNextView( pos ));
		}
		return NULL;
	}

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_MFC_FRAME_DOCUMENT_H_
