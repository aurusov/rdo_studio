#include "stdafx.h"
#include "chatviewerstring.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatString
// ----------------------------------------------------------------------------
CChatString::CChatString()
{
	userName = "";
	str      = "";
	type     = CSTRT_Message;
	time( &global_time );
	height      = 0;
	width       = 0;
	prev_height = 0;
	prev_width  = 0;
	maxWidth    = 0;
	tmps        = "";
}

CChatString::CChatString( const CString& _userName, const CString& _str, CChatStringType _type ):
	userName( _userName ),
	str( _str ),
	type( _type )
{
	time( &global_time );
	height      = 0;
	width       = 0;
	prev_height = 0;
	prev_width  = 0;
	maxWidth    = 0;
	tmps        = "";
}

CChatString::~CChatString()
{
}

CChatStringType CChatString::getType() const
{
	return type;
}

CString& CChatString::getString()
{
	if ( tmps.IsEmpty() ) {
		struct tm* local_time = localtime( &global_time );
		CString str_time;
		str_time.Format( "[%02d:%02d]", local_time->tm_hour, local_time->tm_min );
		if ( type == CSTRT_Message ) {
			tmps = str_time + " (" + userName + "): " + str;
		} else if ( type == CSTRT_ToCryOut ) {
			tmps = str_time + " (" + userName + " cry out): " + str;
		} else {
			tmps = str_time + " " + str;
		}
	}
	return tmps;
}

void CChatString::drawText( CDC* dc, CRect& r, CChatViewerStyle& style )
{
	CChatFontStyle fontStyle;
	switch ( type ) {
		case CSTRT_Message: {
			fontStyle = style.theme.messageFS;
			break;
		}
		case CSTRT_Connect: {
			fontStyle = style.theme.connectFS;
			break;
		}
		case CSTRT_Disconnect: {
			fontStyle = style.theme.disconnectFS;
			break;
		}
		case CSTRT_ChangeName: {
			fontStyle = style.theme.changeNameFS;
			break;
		}
		case CSTRT_ChangeStatusMode: {
			fontStyle = style.theme.changeStatusModeFS;
			break;
		}
		case CSTRT_ToCryOut: {
			fontStyle = style.theme.toCryOutFS;
			break;
		}
		default: {
			fontStyle = CFS_NONE;
			break;
		}
	}

	CFont* prev_font = NULL;
	if ( fontStyle != CFS_NONE ) {

		if ( fontStyle == CFS_BOLD ) {

			prev_font = dc->SelectObject( &style.fontFSB );

		} else if ( fontStyle == CFS_ITALIC ) {

			prev_font = dc->SelectObject( &style.fontFSI );

		} else if ( fontStyle == CFS_UNDERLINE ) {

			prev_font = dc->SelectObject( &style.fontFSU );

		} else if ( fontStyle & CFS_BOLD && fontStyle & CFS_ITALIC && fontStyle & CFS_UNDERLINE ) {

			prev_font = dc->SelectObject( &style.fontFSBIU );

		} else if ( fontStyle & CFS_BOLD && fontStyle & CFS_ITALIC  ) {

			prev_font = dc->SelectObject( &style.fontFSBI );

		} else if ( fontStyle & CFS_BOLD && fontStyle & CFS_UNDERLINE ) {

			prev_font = dc->SelectObject( &style.fontFSBU );

		} else if ( fontStyle & CFS_ITALIC && fontStyle & CFS_UNDERLINE ) {

			prev_font = dc->SelectObject( &style.fontFSIU );

		}
	}

	CRect rect;
	CString s = getString();
	int pos_from = 0;
	int pos = s.Find( ' ', pos_from );
	int x_pos = r.left;
	int y_pos = r.top;
	bool flag = true;
	while ( flag ) {
		if ( pos == -1 ) {
			pos = s.GetLength();
			flag = false;
		}
		CString s2 = s.Mid( pos_from, pos - pos_from );
		s2.TrimLeft();
		if ( !s2.IsEmpty() ) {
			rect = CRect( 0, 0, 1, 1 );
			dc->DrawText( s2, &rect, DT_LEFT | DT_SINGLELINE | DT_CALCRECT );
			if ( x_pos + rect.right > r.right ) {
				x_pos = r.left;
				if ( pos_from != 0 ) {
					y_pos += rect.bottom;
				}
			}
			rect.left    = x_pos;
			rect.top     = y_pos;
			rect.right  += x_pos;
			rect.bottom += y_pos;
			dc->DrawText( s2, &rect, DT_LEFT | DT_SINGLELINE );
			x_pos += rect.Width();
			if ( flag ) {
				rect.left    = x_pos;
				rect.top     = y_pos;
				rect.right  += x_pos;
				rect.bottom += y_pos;
				dc->DrawText( " ", &rect, DT_LEFT | DT_SINGLELINE );
				x_pos += style.spaceWidth;
			}
		}
		pos_from = pos + 1;
		pos = s.Find( ' ', pos_from );
	}

	if ( prev_font ) {
		dc->SelectObject( prev_font );
	}
}

int CChatString::getHeight( CDC* dc, const int _width, CChatViewerStyle& style )
{
	if ( !height ) {
		CChatFontStyle fontStyle;
		switch ( type ) {
			case CSTRT_Message: {
				fontStyle = style.theme.messageFS;
				break;
			}
			case CSTRT_Connect: {
				fontStyle = style.theme.connectFS;
				break;
			}
			case CSTRT_Disconnect: {
				fontStyle = style.theme.disconnectFS;
				break;
			}
			case CSTRT_ChangeName: {
				fontStyle = style.theme.changeNameFS;
				break;
			}
			case CSTRT_ChangeStatusMode: {
				fontStyle = style.theme.changeStatusModeFS;
				break;
			}
			case CSTRT_ToCryOut: {
				fontStyle = style.theme.toCryOutFS;
				break;
			}
			default: {
				fontStyle = CFS_NONE;
				break;
			}
		}

		CFont* prev_font = NULL;
		if ( fontStyle != CFS_NONE ) {

			if ( fontStyle == CFS_BOLD ) {

				prev_font = dc->SelectObject( &style.fontFSB );

			} else if ( fontStyle == CFS_ITALIC ) {

				prev_font = dc->SelectObject( &style.fontFSI );

			} else if ( fontStyle == CFS_UNDERLINE ) {

				prev_font = dc->SelectObject( &style.fontFSU );

			} else if ( fontStyle & CFS_BOLD && fontStyle & CFS_ITALIC && fontStyle & CFS_UNDERLINE ) {

				prev_font = dc->SelectObject( &style.fontFSBIU );

			} else if ( fontStyle & CFS_BOLD && fontStyle & CFS_ITALIC  ) {

				prev_font = dc->SelectObject( &style.fontFSBI );

			} else if ( fontStyle & CFS_BOLD && fontStyle & CFS_UNDERLINE ) {

				prev_font = dc->SelectObject( &style.fontFSBU );

			} else if ( fontStyle & CFS_ITALIC && fontStyle & CFS_UNDERLINE ) {

				prev_font = dc->SelectObject( &style.fontFSIU );

			}
		}

		CRect rect;
		CString s = getString();
		int pos_from = 0;
		int pos = s.Find( ' ', pos_from );
		int x_pos = 0;
		int y_pos = 0;
		bool flag = true;
		while ( flag ) {
			if ( pos == -1 ) {
				pos = s.GetLength();
				flag = false;
			}
			CString s2 = s.Mid( pos_from, pos - pos_from );
			s2.TrimLeft();
			if ( !s2.IsEmpty() ) {
				rect = CRect( 0, 0, 1, 1 );
				dc->DrawText( s2, &rect, DT_LEFT | DT_SINGLELINE | DT_CALCRECT );
				if ( !y_pos ) {
					y_pos = rect.bottom;
				}
				if ( x_pos + rect.right > _width ) {
					x_pos = 0;
					if ( pos_from != 0 ) {
						y_pos += rect.bottom;
					}
				}
				x_pos += rect.right;
				if ( maxWidth < rect.right ) {
					maxWidth = rect.right;
				}
				if ( x_pos > width ) {
					width = x_pos;
				}
				if ( flag ) {
					x_pos += style.spaceWidth;
				}
			}

			pos_from = pos + 1;
			pos = s.Find( ' ', pos_from );
		}
		height = y_pos;

		if ( prev_font ) {
			dc->SelectObject( prev_font );
		}

	}
	return height;
}

bool CChatString::sizeChanged( CDC* dc, const int _width, CChatViewerStyle& style )
{
	return prev_height != height || prev_width != width;
}

int CChatString::getMaxWidth( CDC* dc, const int _width, CChatViewerStyle& style )
{
	if ( !maxWidth ) {
		height = 0;
		height = getHeight( dc, _width, style );
	}
	return maxWidth;
}

void CChatString::recalculateSize()
{
	prev_height = height;
	prev_width  = width;
	height      = 0;
	width       = 0;
	maxWidth    = 0;
}

void CChatString::getColors( const CChatViewerStyle& style, const int lineIndex, COLORREF& textColor, COLORREF& bgColor ) const
{
	switch ( type ) {
		case CSTRT_Message: {
			textColor = style.theme.messageColor;
			bgColor   = lineIndex % 2 ? style.theme.messageBg1Color : style.theme.messageBg2Color;
			break;
		}
		case CSTRT_Connect: {
			textColor = style.theme.connectColor;
			bgColor   = style.theme.connectBgColor;
			break;
		}
		case CSTRT_Disconnect: {
			textColor = style.theme.disconnectColor;
			bgColor   = style.theme.disconnectBgColor;
			break;
		}
		case CSTRT_ChangeName: {
			textColor = style.theme.changeNameColor;
			bgColor   = style.theme.changeNameBgColor;
			break;
		}
		case CSTRT_ChangeStatusMode: {
			textColor = style.theme.changeStatusModeColor;
			bgColor   = style.theme.changeStatusModeBgColor;
			break;
		}
		case CSTRT_ToCryOut: {
			textColor = style.theme.toCryOutColor;
			bgColor   = style.theme.toCryOutBgColor;
			break;
		}
		default: {
			textColor = style.theme.messageColor;
			bgColor   = lineIndex % 2 ? style.theme.messageBg1Color : style.theme.messageBg2Color;
			break;
		}
	}
}

// ----------------------------------------------------------------------------
// ---------- CChatStringList
// ----------------------------------------------------------------------------
CChatStringList::CChatStringList( int blockSize ): CList< CChatString*, CChatString* >( blockSize )
{
}

CChatStringList::~CChatStringList()
{
	POSITION pos = GetHeadPosition();
	while ( pos ) {
		delete GetAt( pos );
		GetNext( pos );
	}
}

bool CChatStringList::scan( char*& wildCards, char*&str ) const
{
	// remove the '?' and '*'
	for( wildCards ++; *str != '\0' && ( *wildCards == '?' || *wildCards == '*' ); wildCards ++ )
		if ( *wildCards == '?') str ++;
	while ( *wildCards == '*') wildCards ++;
	
	// if str is empty and Wildcards has more characters or,
	// Wildcards is empty, return 
	if ( *str == '\0' && *wildCards != '\0' ) return false;
	if ( *str == '\0' && *wildCards == '\0' ) return true; 
	// else search substring
	else
	{
		char* wdsCopy = wildCards;
		char* strCopy = str;
		bool  res     = 1;
		do 
		{
			if ( !match( wildCards, str, true, false ) )	strCopy ++;
			wildCards = wdsCopy;
			str		  = strCopy;
			while ( ( *wildCards != *str ) && ( *str != '\0' ) ) str ++;
			wdsCopy = wildCards;
			strCopy = str;
		} while ( ( *str != '\0' ) ? !match( wildCards, str, true, false ) : ( res = false ) != false );

		if ( *str == '\0' && *wildCards == '\0' ) return true;

		return res;
	}
}

bool CChatStringList::match( const CString& wildCards, const CString& str, const bool matchCase, const bool matchWholeWord ) const
{
	bool res = true;
	
	CString strWild = wildCards;
	CString strComp = str;
	
	if ( !matchCase ) {
		strWild.MakeLower();
		strComp.MakeLower();
	}
	
	if ( matchWholeWord )
		return strWild == strComp;
	
	char* wildcards = strWild.GetBuffer( strWild.GetLength() + 1 );
	char* strcomp   = strComp.GetBuffer( strComp.GetLength() + 1 );

	//iterate and delete '?' and '*' one by one
	while( *wildcards != '\0' && res && *strcomp != '\0' )
	{
		if ( *wildcards == '?' )
			strcomp ++;
		else if ( *wildcards == '*' )
		{
			res = scan( wildcards, strcomp );
			wildcards --;
		}
		else
		{
			res = ( *wildcards == *strcomp );
			strcomp ++;
		}
		wildcards ++;
	}
	while ( *wildcards == '*' && res )  wildcards ++;

	return res && *strcomp == '\0' && *wildcards == '\0';
}

int CChatStringList::findNext( const CString& findWhat, const int findFrom, const int findTo, const bool searchDown, const bool matchCase, const bool matchWholeWord ) const
{
	ASSERT_VALID( this );

	if ( findFrom < 0 || findFrom >= m_nCount || findTo < 0 || findTo >= m_nCount )
		return -1;

	CString findStr = findWhat;
	
	if ( !matchWholeWord && findStr.FindOneOf( "*?") == -1 ) {
		findStr.Insert( 0, "*");
		findStr += "*";
	}

	CNode* nodeFrom = (CNode*)FindIndex( findFrom );
	CNode* nodeTo   = (CNode*)FindIndex( findTo );
	int found = -1;

	if ( !searchDown ) {
		for ( ; nodeFrom != nodeTo->pPrev; nodeFrom = nodeFrom->pPrev ) {
			found++;
			if ( match( findStr, (*nodeFrom->data).getString(), matchCase, matchWholeWord ) )
				return findFrom - found;
		}
	} else {
		for ( ; nodeFrom != nodeTo->pNext; nodeFrom = nodeFrom->pNext ) {
			found++;
			if ( match( findStr, (*nodeFrom->data).getString(), matchCase, matchWholeWord ) )
				return findFrom + found;
		}
	}

	return -1;
}
