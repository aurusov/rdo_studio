/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/view.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      21.04.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_VIEW_H_
#define _RDO_STUDIO_MFC_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioView
// --------------------------------------------------------------------------------
class RDOStudioView: public CView
{
protected:
	RDOStudioView();
	DECLARE_DYNAMIC(RDOStudioView)

public:
	virtual ~RDOStudioView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	//{{AFX_VIRTUAL(RDOStudioView)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RDOStudioView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _RDO_STUDIO_MFC_VIEW_H_
