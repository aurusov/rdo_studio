#ifndef RDOSTUDIOMODELDOC_H
#define RDOSTUDIOMODELDOC_H

#include "app/rdo_studio_mfc/src/edit/document_base.h"

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModelDoc
// --------------------------------------------------------------------------------
class RDOStudioModelView;

class RDOStudioModelDoc: public RDOStudioEditBaseDoc
{
friend class RDOStudioModel;

DECLARE_DYNCREATE(RDOStudioModelDoc)

private:
	tstring name;

	RDOStudioModelDoc();

private:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL SaveModified();
	DECLARE_MESSAGE_MAP()

public:
	virtual ~RDOStudioModelDoc();

	virtual void updateModify();

	RDOStudioModelView* getView() const;

	tstring getName() const { return name; }
	void setName( CREF(tstring) str );

	bool isModify();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

#endif // RDOSTUDIOMODELDOC_H
