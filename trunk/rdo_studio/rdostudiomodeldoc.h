#ifndef RDOSTUDIOMODELDOC_H
#define RDOSTUDIOMODELDOC_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioeditbasedoc.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioModelDoc
// ----------------------------------------------------------------------------
class RDOStudioModelView;

class RDOStudioModelDoc: public RDOStudioEditBaseDoc
{
friend class RDOStudioModel;

protected:
	RDOStudioModelDoc();
	DECLARE_DYNCREATE(RDOStudioModelDoc)

public:
	//{{AFX_VIRTUAL(RDOStudioModelDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioModelDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual ~RDOStudioModelDoc();

	virtual void updateModify();

	RDOStudioModelView* getView() const;

	bool isRunning() const;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOMODELDOC_H
