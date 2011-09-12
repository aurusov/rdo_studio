/*!
  \copyright (c) RDO-Team, 2011
  \file      rdostudiomodelnew.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOSTUDIOMODEL_NEW_H_
#define _RDOSTUDIOMODEL_NEW_H_

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/resource.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioOptionsGeneral
// --------------------------------------------------------------------------------
class RDOStudioModelNew: public CDialog
{
public:
	RDOStudioModelNew();
	virtual ~RDOStudioModelNew();

	tstring getModelName    () const;
	tstring getModelPath    () const;
	ruint   getModelTemplate() const;

	virtual void OnOK();

protected:
	//{{AFX_DATA(RDOStudioModelNew)
	enum { IDD = IDD_MODEL_NEW };
	CButton  m_ok;
	CString  m_modelName;
	CString  m_modelPath;
	CString  m_info;
	BOOL     m_comment;
	int      m_modelTemplate;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RDOStudioModelNew)

private:
	virtual void DoDataExchange(PTR(CDataExchange) pDX);
	virtual BOOL OnNotify      (WPARAM wParam, LPARAM lParam, PTR(LRESULT) pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioModelNew)
	afx_msg void   OnModelPathButton();
	afx_msg void   OnChangeModelName();
	virtual BOOL   OnInitDialog     ();
	afx_msg void   OnModelEmpty     ();
	afx_msg HBRUSH OnCtlColor       (PTR(CDC) pDC, PTR(CWnd) pWnd, UINT nCtlColor);
	//}}AFX_MSG

	CString getMyDocFolder() const;
	void    updateInfo    ();

	CFont     m_fontNormal;
	CFont     m_fontRed;
	COLORREF  m_colorNormal;
	COLORREF  m_colorRed;
	rbool     m_needRed;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _RDOSTUDIOMODEL_NEW_H_
