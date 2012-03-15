/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/model/document.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_MODEL_DOCUMENT_H_
#define _RDO_STUDIO_MFC_MODEL_DOCUMENT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/edit/document_base.h"
// --------------------------------------------------------------------------------

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

	rbool isModify();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

#endif // _RDO_STUDIO_MFC_MODEL_DOCUMENT_H_
