#ifndef CHATVIEWERSTRING_H
#define CHATVIEWERSTRING_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatviewerstyle.h"

// ----------------------------------------------------------------------------
// ---------- CChatString
// ----------------------------------------------------------------------------
enum CChatStringType {
	CSTRT_Message = 0,
	CSTRT_Connect,
	CSTRT_Disconnect,
	CSTRT_ChangeName,
	CSTRT_ChangeStatusMode,
	CSTRT_ToCryOut,
	CSTRT_PopupMsgSend,
	CSTRT_PopupMsgReceive
};

class CChatString
{
private:
	std::string tmps;

protected:
	std::string     userName;
	std::string     toUserName;
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
	CChatString( const std::string& _userName, const std::string& _str, CChatStringType _type = CSTRT_Message, const std::string& _toUserName = "" );
	virtual ~CChatString();

	CChatStringType getType() const { return type; }
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
class CChatStringList
{
private:
	std::vector< CChatString* > list;

public:
	CChatStringList();
	virtual ~CChatStringList();

	void addString( CChatString* str )          { list.push_back( str ); }
	int count() const                           { return list.size();    }
	CChatString* operator[] ( int index ) const { return list[index];    }

	void recalculateSize();
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATVIEWERSTRING_H
