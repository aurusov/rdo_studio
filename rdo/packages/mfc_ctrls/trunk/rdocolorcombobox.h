/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdocolorcombobox.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _UI_MFC_CTRLS_RDOCOLORCOMBOBOX_H_
#define _UI_MFC_CTRLS_RDOCOLORCOMBOBOX_H_

// ----------------------------------------------------------------------- INCLUDES
#include <afxwin.h>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOColorComboBox
// --------------------------------------------------------------------------------
class RDOColorComboBox: public CComboBox
{
private:
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );

	DECLARE_MESSAGE_MAP()

public:
	RDOColorComboBox();
	virtual ~RDOColorComboBox();

	void setItemHeight( const int height );

	void insertBaseColors();

	int insertColor( const COLORREF color, CString colorName = "" );
	int findItem( const CString colorName );
	int findItem( const COLORREF color );

	CString getCurrentColorName() const;
	COLORREF getCurrentColor() const;
	void setCurrentColor( const CString& colorName );
	void setCurrentColor( const COLORREF color );
};

#endif // _UI_MFC_CTRLS_RDOCOLORCOMBOBOX_H_
