/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/edit/document_base.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_DOCUMENT_BASE_H_
#define _RDO_STUDIO_MFC_EDIT_DOCUMENT_BASE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioEditBaseDoc
// --------------------------------------------------------------------------------
class RDOStudioEditBaseDoc: public CDocument
{
protected:
	RDOStudioEditBaseDoc();
	DECLARE_DYNCREATE(RDOStudioEditBaseDoc)

public:
	virtual void updateModify() {};

	//{{AFX_VIRTUAL(RDOStudioEditBaseDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioEditBaseDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual ~RDOStudioEditBaseDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}

#endif // _RDO_STUDIO_MFC_EDIT_DOCUMENT_BASE_H_
