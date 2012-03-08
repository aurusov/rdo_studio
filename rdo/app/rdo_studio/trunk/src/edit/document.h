/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/edit/document.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_DOCUMENT_H_
#define _RDO_STUDIO_MFC_EDIT_DOCUMENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/edit/document_base.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioEditDoc
// --------------------------------------------------------------------------------
class RDOStudioEditDoc: public RDOStudioEditBaseDoc
{
protected:
	RDOStudioEditDoc();
	virtual void updateModify();

	DECLARE_DYNCREATE(RDOStudioEditDoc)

public:
	//{{AFX_VIRTUAL(RDOStudioEditDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioEditDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual ~RDOStudioEditDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

//{{AFX_INSERT_LOCATION}}

#endif // _RDO_STUDIO_MFC_EDIT_DOCUMENT_H_
