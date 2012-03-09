/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotreectrl.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _UI_MFC_CTRLS_RDOTREECTRL_H_
#define _UI_MFC_CTRLS_RDOTREECTRL_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOTreeCtrl
// --------------------------------------------------------------------------------
class RDOTreeCtrl: public CTreeCtrl
{
DECLARE_DYNAMIC( RDOTreeCtrl )

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

private:
	DECLARE_MESSAGE_MAP()

public:
	RDOTreeCtrl();
	virtual ~RDOTreeCtrl();

	void deleteChildren( const HTREEITEM parent );
};

#endif // _UI_MFC_CTRLS_RDOTREECTRL_H_
