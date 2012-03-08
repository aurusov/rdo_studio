#ifndef RDOSTUDIOFRAMEDOC_H
#define RDOSTUDIOFRAMEDOC_H

#include "app/rdo_studio_mfc/src/frame/view.h"

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

#endif // RDOSTUDIOFRAMEDOC_H
