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
	userName   = "";
	toUserName = "";
	str        = "";
	type       = CSTRT_Message;
	time( &global_time );
	height      = 0;
	width       = 0;
	prev_height = 0;
	prev_width  = 0;
	maxWidth    = 0;
	tmps        = "";
}

CChatString::CChatString( const std::string& _userName, const std::string& _str, CChatStringType _type, const std::string& _toUserName ):
	userName( _userName ),
	toUserName( _toUserName ),
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

std::string& CChatString::getString()
{
	if ( tmps.empty() ) {
		struct tm* local_time = localtime( &global_time );
		std::string str_time = format( "[%02d:%02d]", local_time->tm_hour, local_time->tm_min );
		if ( type == CSTRT_Message ) {
			tmps = str_time + " (" + userName + "): " + str;
		} else if ( type == CSTRT_ToCryOut ) {
			tmps = str_time + " (" + userName + " cry out): " + str;
		} else if ( type == CSTRT_PopupMsgSend ) {
			tmps = str_time + " (for " + toUserName + "): " + str;
		} else if ( type == CSTRT_PopupMsgReceive ) {
			tmps = str_time + " (from " + userName + "): " + str;
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
		case CSTRT_PopupMsgSend: {
			fontStyle = style.theme.popupMsgSendFS;
			break;
		}
		case CSTRT_PopupMsgReceive: {
			fontStyle = style.theme.popupMsgReceiveFS;
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
	std::string s = getString();
	int pos_from = 0;
	int pos = s.find( ' ', pos_from );
	int x_pos = r.left;
	int y_pos = r.top;
	bool flag = true;
	while ( flag ) {
		if ( pos == std::string::npos ) {
			pos = s.length();
			flag = false;
		}
		std::string s2 = s.substr( pos_from, pos - pos_from );
		trimLeft( s2 );
		if ( !s2.empty() ) {
			rect = CRect( 0, 0, 1, 1 );
			dc->DrawText( s2.c_str(), &rect, DT_LEFT | DT_SINGLELINE | DT_CALCRECT );
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
			dc->DrawText( s2.c_str(), &rect, DT_LEFT | DT_SINGLELINE );
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
		pos = s.find( ' ', pos_from );
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
			case CSTRT_PopupMsgSend: {
				fontStyle = style.theme.popupMsgSendFS;
				break;
			}
			case CSTRT_PopupMsgReceive: {
				fontStyle = style.theme.popupMsgReceiveFS;
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
		std::string s = getString();
		int pos_from = 0;
		int pos = s.find( ' ', pos_from );
		int x_pos = 0;
		int y_pos = 0;
		bool flag = true;
		while ( flag ) {
			if ( pos == std::string::npos ) {
				pos = s.length();
				flag = false;
			}
			std::string s2 = s.substr( pos_from, pos - pos_from );
			trimLeft( s2 );
			if ( !s2.empty() ) {
				rect = CRect( 0, 0, 1, 1 );
				dc->DrawText( s2.c_str(), &rect, DT_LEFT | DT_SINGLELINE | DT_CALCRECT );
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
			pos = s.find( ' ', pos_from );
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
		case CSTRT_PopupMsgSend: {
			textColor = style.theme.popupMsgSendColor;
			bgColor   = style.theme.popupMsgSendBgColor;
			break;
		}
		case CSTRT_PopupMsgReceive: {
			textColor = style.theme.popupMsgReceiveColor;
			bgColor   = style.theme.popupMsgReceiveBgColor;
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
