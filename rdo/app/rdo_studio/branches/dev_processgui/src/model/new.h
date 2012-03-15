/*!
  \copyright (c) RDO-Team, 2007-2012
  \file      app/rdo_studio_mfc/src/model/new.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.01.2007
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_MODEL_NEW_H_
#define _RDO_STUDIO_MFC_MODEL_NEW_H_

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
	enum { IDD = IDD_MODEL_NEW };
	CButton  m_ok;
	CString  m_modelName;
	CString  m_modelPath;
	CString  m_info;
	BOOL     m_comment;
	int      m_modelTemplate;

private:
	virtual void DoDataExchange(PTR(CDataExchange) pDX);
	virtual BOOL OnNotify      (WPARAM wParam, LPARAM lParam, PTR(LRESULT) pResult);

	afx_msg void   OnModelPathButton();
	afx_msg void   OnChangeModelName();
	virtual BOOL   OnInitDialog     ();
	afx_msg void   OnModelEmpty     ();
	afx_msg HBRUSH OnCtlColor       (PTR(CDC) pDC, PTR(CWnd) pWnd, UINT nCtlColor);

	CString getMyDocFolder() const;
	void    updateInfo    ();

	CFont     m_fontNormal;
	CFont     m_fontRed;
	COLORREF  m_colorNormal;
	COLORREF  m_colorRed;
	rbool     m_needRed;

	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_MFC_MODEL_NEW_H_
