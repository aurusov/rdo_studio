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
	std::string name;

	RDOStudioModelDoc();

public:
	//{{AFX_VIRTUAL(RDOStudioModelDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioModelDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual ~RDOStudioModelDoc();

	virtual void updateModify();

	RDOStudioModelView* getView() const;

	std::string getName() const { return name; }
	void setName( const std::string& str );

	bool isModify();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOMODELDOC_H
