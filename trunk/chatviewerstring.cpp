#include "stdafx.h"
#include "chatviewerstring.h"
#include "chatapp.h"
#include "chatmainfrm.h"

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
	message    = "";
	type       = CSTRT_Message;
	time( &global_time );
	height      = 0;
	width       = 0;
	prev_height = 0;
	prev_width  = 0;
	maxWidth    = 0;
	tmps        = "";
	textColor   = RGB( 0x00, 0x00, 0x00 );
	bgColor     = RGB( 0xFF, 0xFF, 0xFF );
}

CChatString::CChatString( const int lineIndex, const std::string& _userName, const std::string& _message, CChatStringType _type, const std::string& _toUserName ):
	userName( _userName ),
	toUserName( _toUserName ),
	message( _message ),
	type( _type )
{
	time( &global_time );
	height      = 0;
	width       = 0;
	prev_height = 0;
	prev_width  = 0;
	maxWidth    = 0;
	tmps        = "";

	CChatViewerTheme* theme = &chatApp.style.theme;
	switch ( type ) {
		case CSTRT_Message: {
			textColor = theme->messageColor;
			bgColor   = lineIndex % 2 ? theme->messageBg1Color : theme->messageBg2Color;
			break;
		}
		case CSTRT_Connect: {
			textColor = theme->connectColor;
			bgColor   = theme->connectBgColor;
			break;
		}
		case CSTRT_Disconnect: {
			textColor = theme->disconnectColor;
			bgColor   = theme->disconnectBgColor;
			break;
		}
		case CSTRT_ChangeName: {
			textColor = theme->changeNameColor;
			bgColor   = theme->changeNameBgColor;
			break;
		}
		case CSTRT_ChangeStatusMode: {
			textColor = theme->changeStatusModeColor;
			bgColor   = theme->changeStatusModeBgColor;
			break;
		}
		case CSTRT_ToCryOut: {
			textColor = theme->toCryOutColor;
			bgColor   = theme->toCryOutBgColor;
			break;
		}
		case CSTRT_PopupMsgSend: {
			textColor = theme->popupMsgSendColor;
			bgColor   = theme->popupMsgSendBgColor;
			break;
		}
		case CSTRT_PopupMsgReceive: {
			textColor = theme->popupMsgReceiveColor;
			bgColor   = theme->popupMsgReceiveBgColor;
			break;
		}
		default: {
			textColor = theme->messageColor;
			bgColor   = lineIndex % 2 ? theme->messageBg1Color : theme->messageBg2Color;
			break;
		}
	}

	int pos_from = 0;
	int pos = message.find( ' ', pos_from );
	bool flag = true;
	while ( flag ) {
		if ( pos == std::string::npos ) {
			pos = message.length();
			flag = false;
		}
		std::string str = message.substr( pos_from, pos - pos_from );
		trimLeft( str );
		CChatSmile* smile = NULL;
		if ( str == ":)" || str == ":-)" ) {
			smile = smiles.addSmile( CChatSmile::smile );
		} else if ( str == ";)" || str == ";-)" ) {
			smile = smiles.addSmile( CChatSmile::wink );
		} else if ( str == ":eyes:" ) {
			smile = smiles.addSmile( CChatSmile::eyes );
		} else if ( str == ":beer:" ) {
			smile = smiles.addSmile( CChatSmile::beer );
		} else if ( str == ":friday:" ) {
			smile = smiles.addSmile( CChatSmile::friday );
		} else if ( str == ":tomato:" ) {
			smile = smiles.addSmile( CChatSmile::tomato );
		}
		if ( smile ) {
			if ( !smile->IsAnimatedGIF() ) smile->Stop();
			smile->SetBkColor( getBgColor() );
			if ( !smile->IsAnimatedGIF() ) smile->Draw();
		}
		pos_from = pos + 1;
		pos = message.find( ' ', pos_from );
	}
}

CChatString::~CChatString()
{
}

CChatSmile* CChatString::getSmile( const std::string& str, int& smile_index ) const
{
	if ( str == ":)" || str == ":-)" ||
	     str == ";)" || str == ";-)" ||
	     str == ":eyes:" ||
	     str == ":beer:" ||
	     str == ":friday:" ||
	     str == ":tomato:"
	   ) {
		return smiles[smile_index++];
	}
	return NULL;
}

std::string& CChatString::getString()
{
	if ( tmps.empty() ) {
		struct tm* local_time = localtime( &global_time );
		std::string str_time = format( "[%02d:%02d]", local_time->tm_hour, local_time->tm_min );
		if ( type == CSTRT_Message ) {
			tmps = str_time + " (" + userName + "): " + message;
		} else if ( type == CSTRT_ToCryOut ) {
			tmps = str_time + " (" + userName + " cry out): " + message;
		} else if ( type == CSTRT_PopupMsgSend ) {
			tmps = str_time + " (for " + toUserName + "): " + message;
		} else if ( type == CSTRT_PopupMsgReceive ) {
			tmps = str_time + " (from " + userName + "): " + message;
		} else {
			tmps = str_time + " " + message;
		}
	}
	return tmps;
}

CFont* CChatString::setFont( CDC* dc ) const
{
	CChatViewerTheme* theme = &chatApp.style.theme;
	CChatFontStyle fontStyle;
	switch ( type ) {
		case CSTRT_Message         : fontStyle = theme->messageFS; break;
		case CSTRT_Connect         : fontStyle = theme->connectFS; break;
		case CSTRT_Disconnect      : fontStyle = theme->disconnectFS; break;
		case CSTRT_ChangeName      : fontStyle = theme->changeNameFS; break;
		case CSTRT_ChangeStatusMode: fontStyle = theme->changeStatusModeFS; break;
		case CSTRT_ToCryOut        : fontStyle = theme->toCryOutFS; break;
		case CSTRT_PopupMsgSend    : fontStyle = theme->popupMsgSendFS; break;
		case CSTRT_PopupMsgReceive : fontStyle = theme->popupMsgReceiveFS; break;
		default                    : fontStyle = CFS_NONE; break;
	}
	CFont* prev_font = NULL;
	if ( fontStyle != CFS_NONE ) {

		if ( fontStyle == CFS_BOLD ) {

			prev_font = dc->SelectObject( &chatApp.style.fontFSB );

		} else if ( fontStyle == CFS_ITALIC ) {

			prev_font = dc->SelectObject( &chatApp.style.fontFSI );

		} else if ( fontStyle == CFS_UNDERLINE ) {

			prev_font = dc->SelectObject( &chatApp.style.fontFSU );

		} else if ( fontStyle & CFS_BOLD && fontStyle & CFS_ITALIC && fontStyle & CFS_UNDERLINE ) {

			prev_font = dc->SelectObject( &chatApp.style.fontFSBIU );

		} else if ( fontStyle & CFS_BOLD && fontStyle & CFS_ITALIC  ) {

			prev_font = dc->SelectObject( &chatApp.style.fontFSBI );

		} else if ( fontStyle & CFS_BOLD && fontStyle & CFS_UNDERLINE ) {

			prev_font = dc->SelectObject( &chatApp.style.fontFSBU );

		} else if ( fontStyle & CFS_ITALIC && fontStyle & CFS_UNDERLINE ) {

			prev_font = dc->SelectObject( &chatApp.style.fontFSIU );

		}
	}
	return prev_font;
}

void CChatString::drawText( CDC* dc, CRect& r )
{
	CFont* prev_font = setFont( dc );
	CRect rect;
	std::string s = getString();
	int pos_from = 0;
	int pos = s.find( ' ', pos_from );
	int x_pos = r.left;
	int y_pos = r.top;
	int smile_index = 0;
	int height_of_line = getOneLineHeight( dc, r.Width(), pos_from, smile_index );
	bool flag = true;
	while ( flag ) {
		if ( pos == std::string::npos ) {
			pos = s.length();
			flag = false;
		}
		std::string s2 = s.substr( pos_from, pos - pos_from );
		trimLeft( s2 );
		if ( !s2.empty() ) {
			CChatSmile* smile = getSmile( s2, smile_index );
			if ( smile ) {
				CSize size = smile->GetSize();
				if ( x_pos + size.cx > r.right ) {
					x_pos = r.left;
					if ( pos_from != 0 ) {
						y_pos += height_of_line;
						smile_index--;
						height_of_line = getOneLineHeight( dc, r.Width(), pos_from, smile_index );
					}
				}
				smile->MoveWindow( x_pos, y_pos + height_of_line - size.cy, x_pos + size.cx, y_pos + height_of_line );
				x_pos += size.cx;
				if ( flag ) {
//					rect.left    = x_pos;
//					rect.top     = y_pos;
//					rect.right  += x_pos;
//					rect.bottom += y_pos;
//					dc->DrawText( " ", &rect, DT_LEFT | DT_SINGLELINE );
					x_pos += chatApp.style.spaceWidth;
				}
			} else {
				rect = CRect( 0, 0, 1, 1 );
				dc->DrawText( s2.c_str(), &rect, DT_LEFT | DT_SINGLELINE | DT_CALCRECT );
				if ( x_pos + rect.right > r.right ) {
					x_pos = r.left;
					if ( pos_from != 0 ) {
						y_pos += height_of_line;
					}
				}
				rect.left    = x_pos;
				rect.top     = y_pos;
				rect.right  += x_pos;
				rect.bottom  = y_pos + height_of_line;
				dc->DrawText( s2.c_str(), &rect, DT_LEFT | DT_SINGLELINE | DT_BOTTOM );
				x_pos += rect.Width();
				if ( flag ) {
					rect.left    = x_pos;
					rect.top     = y_pos;
					rect.right  += x_pos;
					rect.bottom += y_pos;
					dc->DrawText( " ", &rect, DT_LEFT | DT_SINGLELINE );
					x_pos += chatApp.style.spaceWidth;
				}
			}
		}
		pos_from = pos + 1;
		pos = s.find( ' ', pos_from );
	}

	dc->SelectObject( prev_font );
}

int CChatString::getOneLineHeight( CDC* dc, const int _width, int pos_from, int smile_index )
{
	CRect rect;
	std::string s = getString();
	int pos = s.find( ' ', pos_from );
	int x_pos = 0;
	int y_pos = 0;
	int height_of_line = 0;
	bool flag = true;
	while ( flag ) {
		if ( pos == std::string::npos ) {
			pos = s.length();
			flag = false;
		}
		std::string s2 = s.substr( pos_from, pos - pos_from );
		if ( !s2.empty() ) {
			CChatSmile* smile = getSmile( s2, smile_index );
			if ( smile ) {
				CSize size = smile->GetSize();
				if ( y_pos < size.cy ) {
					y_pos = size.cy;
				}
				if ( x_pos + size.cy > _width ) {
					return y_pos;
				}
				x_pos += size.cy;
			} else {
				rect = CRect( 0, 0, 1, 1 );
				dc->DrawText( s2.c_str(), &rect, DT_LEFT | DT_SINGLELINE | DT_CALCRECT );
				if ( y_pos < rect.bottom ) {
					y_pos = rect.bottom;
				}
				if ( x_pos + rect.right > _width ) {
					return y_pos;
				}
				x_pos += rect.right;
			}
		}

		pos_from = pos + 1;
		pos = s.find( ' ', pos_from );
	}

	return y_pos;
}

int CChatString::getHeight( CDC* dc, const int _width )
{
	if ( !height ) {
		CFont* prev_font = setFont( dc );
		CRect rect;
		std::string s = getString();
		int pos_from = 0;
		int pos = s.find( ' ', pos_from );
		int x_pos = 0;
		int y_pos = 0;
		int smile_index = 0;
		bool flag = true;
		while ( flag ) {
			if ( pos == std::string::npos ) {
				pos = s.length();
				flag = false;
			}
			std::string s2 = s.substr( pos_from, pos - pos_from );
			if ( !s2.empty() ) {
				CChatSmile* smile = getSmile( s2, smile_index );
				if ( smile ) {
					CSize size = smile->GetSize();
					if ( y_pos < size.cy ) {
						y_pos = size.cy;
					}
					if ( x_pos + size.cy > _width ) {
						x_pos = 0;
						if ( pos_from != 0 ) {
							y_pos += size.cy;
						}
					}
					x_pos += size.cy;
					if ( maxWidth < size.cy ) {
						maxWidth = size.cy;
					}
					if ( x_pos > width ) {
						width = x_pos;
					}
					if ( flag ) {
						x_pos += chatApp.style.spaceWidth;
					}
				} else {
					rect = CRect( 0, 0, 1, 1 );
					dc->DrawText( s2.c_str(), &rect, DT_LEFT | DT_SINGLELINE | DT_CALCRECT );
					if ( y_pos < rect.bottom ) {
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
						x_pos += chatApp.style.spaceWidth;
					}
				}
			}

			pos_from = pos + 1;
			pos = s.find( ' ', pos_from );
		}
		height = y_pos;

		dc->SelectObject( prev_font );

	}
	return height;
}

bool CChatString::sizeChanged( CDC* dc, const int _width )
{
	return prev_height != height || prev_width != width;
}

int CChatString::getMaxWidth( CDC* dc, const int _width )
{
	if ( !maxWidth ) {
		height = 0;
		height = getHeight( dc, _width );
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

void CChatString::selectedON()
{
	smiles.setBgColor( chatApp.style.theme.selectedBgColor );
}

void CChatString::selectedOFF()
{
	smiles.setBgColor( getBgColor() );
}

// ----------------------------------------------------------------------------
// ---------- CChatStringList
// ----------------------------------------------------------------------------
CChatStringList::CChatStringList(): selected( -1 )
{
}

CChatStringList::~CChatStringList()
{
	std::vector< CChatString* >::iterator it = list.begin();
	while ( it != list.end() ) {
		delete *it++;
	}
	list.clear();
}

void CChatStringList::recalculateSize()
{
	std::vector< CChatString* >::iterator it = list.begin();
	while ( it != list.end() ) {
		(*it++)->recalculateSize();
	}
}

void CChatStringList::setSelected( const int _selected )
{
	if ( selected != _selected ) {
		selected = _selected;
		if ( selected != -1 ) list[ selected ]->selectedON();
	}
}

void CChatStringList::clearSelected( const int _selected )
{
	selected = -1;
	if ( _selected != -1 ) list[ _selected ]->selectedOFF();
}
