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
	std::string tmps;

protected:
	std::string     userName;
	std::string     str;
	CChatStringType type;
	long            global_time;

	int height;
	int width;
	int prev_height;
	int prev_width;
	int maxWidth;

public:
	CChatString();
	CChatString( const std::string& _userName, const std::string& _str, CChatStringType _type = CSTRT_Message );
	virtual ~CChatString();

	CChatStringType getType() const;
	std::string& getString();

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
public:
	CChatStringList( int blockSize = 10 );
	virtual ~CChatStringList();
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATVIEWERSTRING_H
