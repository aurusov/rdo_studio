#ifndef RDOSTUDIOFRAMEDOC_H
#define RDOSTUDIOFRAMEDOC_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioframeview.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameDoc
// ----------------------------------------------------------------------------
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

public:
	//{{AFX_VIRTUAL(RDOStudioFrameDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioFrameDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOFRAMEDOC_H
