#ifndef RDOCOLORCOMBOBOX_H
#define RDOCOLORCOMBOBOX_H
#pragma once

#include <afxwin.h>

// ----------------------------------------------------------------------------
// ---------- RDOColorComboBox
// ----------------------------------------------------------------------------
class RDOColorComboBox: public CComboBox
{
protected:
	//{{AFX_MSG(RDOColorComboBox)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_VIRTUAL(RDOColorComboBox)
	public:
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	//}}AFX_VIRTUAL

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

//{{AFX_INSERT_LOCATION}}

#endif // RDOCOLORCOMBOBOX_H
