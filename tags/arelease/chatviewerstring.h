#ifndef CHATVIEWERSTRING_H
#define CHATVIEWERSTRING_H
#pragma once

#include "chatviewerstyle.h"

#include <afxtempl.h>

// ----------------------------------------------------------------------------
// ---------- CChatString
// ----------------------------------------------------------------------------
enum CChatStringType {
	CSTRT_Message = 0,
	CSTRT_Connect,
	CSTRT_Disconnect,
	CSTRT_ChangeName,
	CSTRT_ChangeStatusMode,
	CSTRT_ToCryOut
};

class CChatString
{
private:
	CString tmps;

protected:
	CString         userName;
	CString         str;
	CChatStringType type;
	long            global_time;

	int height;
	int width;
	int prev_height;
	int prev_width;
	int maxWidth;

public:
	CChatString();
	CChatString( const CString& _userName, const CString& _str, CChatStringType _type = CSTRT_Message );
	virtual ~CChatString();

	CChatStringType getType() const;
	CString& getString();

	void drawText( CDC* dc, CRect& rect, CChatViewerStyle& style );
	int getHeight( CDC* dc, const int _width, CChatViewerStyle& style );
	bool sizeChanged( CDC* dc, const int _width, CChatViewerStyle& style );
	int getMaxWidth( CDC* dc, const int _width, CChatViewerStyle& style );
	void recalculateSize();

	void getColors( const CChatViewerStyle& style, const int lineIndex, COLORREF& textColor, COLORREF& bgColor ) const;
};

// ----------------------------------------------------------------------------
// ---------- CChatStringList
// ----------------------------------------------------------------------------
class CChatStringList: public CList< CChatString*, CChatString* >
{
protected:
	bool scan( char*& wildCards, char*&str ) const;

public:
	CChatStringList( int blockSize = 10 );
	virtual ~CChatStringList();

	bool match( const CString& wildCards, const CString& str, const bool matchCase, const bool matchWholeWord ) const;
	int findNext( const CString& findWhat, const int findFrom, const int findTo, const bool searchDown, const bool matchCase, const bool matchWholeWord ) const;
};

#endif // CHATVIEWERSTRING_H
