#ifndef CHATLISTBOXCTRL_H
#define CHATLISTBOXCTRL_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- CChatListBoxCtrl
// ----------------------------------------------------------------------------
class CChatListBoxCtrl: public CListBox
{
protected:
	CImageList* imageList;

	bool isImage() const;
	CSize getImageSize() const;

	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );

public:
	CChatListBoxCtrl();
	virtual ~CChatListBoxCtrl();

	int addString( const CString& str, const int image = -1 );
	int insertString( const int index, const CString& str, const int image = -1 );
	void setItemImage( const int index, const int image );
	void setImageList( CImageList* _imageList );
};

#endif // CHATLISTBOXCTRL_H
