#ifndef RDOCOLORCOMBOBOX_H
#define RDOCOLORCOMBOBOX_H

#include <afxwin.h>

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

#endif // RDOCOLORCOMBOBOX_H
