#ifndef CHATVIEWERSTRING_H
#define CHATVIEWERSTRING_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatviewerstyle.h"
#include "chatsmile.h"

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
	CSTRT_PrivateMsgSend,
	CSTRT_PrivateMsgReceive
};

class CChatString
{
friend class CChatStringList;

private:
	std::string tmps;

protected:
	std::string     userName;
	std::string     toUserName;
	std::string     message;
	CChatStringType type;
	COLORREF        textColor;
	COLORREF        bgColor;

	long            global_time;

	int height;
	int width;
	int prev_height;
	int prev_width;
	int maxWidth;

	CChatSmileList smiles;

	CFont* setFont( CDC* dc ) const;

	void selectedON();
	void selectedOFF();

	int getOneLineHeight( CDC* dc, const int _width, int pos_from, int smile_index );

public:
	CChatString();
	CChatString( const int lineIndex, const std::string& _userName, const std::string& _message, CChatStringType _type = CSTRT_Message, const std::string& _toUserName = "" );
	virtual ~CChatString();

	CChatStringType getType() const { return type; }
	std::string& getString();

	void drawText( CDC* dc, CRect& rect );
	int getHeight( CDC* dc, const int _width );
	bool sizeChanged( CDC* dc, const int _width );
	int getMaxWidth( CDC* dc, const int _width );
	void recalculateSize();

	COLORREF getTextColor() const { return textColor; }
	COLORREF getBgColor() const   { return bgColor;   }

	void showAnimation( const bool value ) { smiles.showAnimation( value ); }
};

// ----------------------------------------------------------------------------
// ---------- CChatStringList
// ----------------------------------------------------------------------------
class CChatStringList
{
private:
	std::vector< CChatString* > list;

	int selected;

public:
	CChatStringList();
	virtual ~CChatStringList();

	void addString( CChatString* str )          { list.push_back( str ); }
	int count() const                           { return list.size();    }
	CChatString* operator[] ( int index ) const { return list[index];    }

	void setSelected( const int _selected );
	void clearSelected( const int _selected );
	void recalculateSize();
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATVIEWERSTRING_H
