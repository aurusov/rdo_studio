#ifndef CHATLISTBOXCTRL_H
#define CHATLISTBOXCTRL_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- CChatListBoxCtrl
// ----------------------------------------------------------------------------
class CChatListBoxCtrl: public CListBox
{
protected:
	CImageList* imageList;

	bool isImage() const;
	CSize getImageSize() const;

public:
	CChatListBoxCtrl();
	virtual ~CChatListBoxCtrl();

	int addString( const std::string& str, const int image = -1 );
	int insertString( const int index, const std::string& str, const int image = -1 );
	void setItemImage( const int index, const int image );
	void setImageList( CImageList* _imageList );

protected:
	//{{AFX_VIRTUAL(CChatListBoxCtrl)
	protected:
	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct );
	virtual void MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatListBoxCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATLISTBOXCTRL_H
