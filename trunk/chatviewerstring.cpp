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
		case CSTRT_PrivateMsgSend: {
			textColor = theme->privateMsgSendColor;
			bgColor   = theme->privateMsgSendBgColor;
			break;
		}
		case CSTRT_PrivateMsgReceive: {
			textColor = theme->privateMsgReceiveColor;
			bgColor   = theme->privateMsgReceiveBgColor;
			break;
		}
		default: {
			textColor = theme->messageColor;
			bgColor   = lineIndex % 2 ? theme->messageBg1Color : theme->messageBg2Color;
			break;
		}
	}

	if ( chatApp.isWinNT() ) {
		int pos_from = 0;
		int pos = message.find( ' ', pos_from );
		bool flag = true;
		while ( flag ) {
			if ( pos == std::string::npos ) {
				pos = message.length();
				flag = false;
			}
			std::string str = message.substr( pos_from, pos - pos_from );
			CChatSmile* smile = smiles.addSmile( str, &chatApp.mainFrame->childView.viewer );
			if ( smile ) {
				if ( !smile->IsAnimatedGIF() ) smile->Stop();
				smile->SetBkColor( getBgColor() );
				if ( !smile->IsAnimatedGIF() ) smile->Draw();
			}
			pos_from = pos + 1;
			pos = message.find( ' ', pos_from );
		}
	}
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
			tmps = str_time + " (" + userName + "): " + message;
		} else if ( type == CSTRT_ToCryOut ) {
			tmps = str_time + " (" + userName + " cry out): " + message;
		} else if ( type == CSTRT_PrivateMsgSend ) {
			tmps = str_time + " (for " + toUserName + "): " + message;
		} else if ( type == CSTRT_PrivateMsgReceive ) {
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
		case CSTRT_Message          : fontStyle = theme->messageFS; break;
		case CSTRT_Connect          : fontStyle = theme->connectFS; break;
		case CSTRT_Disconnect       : fontStyle = theme->disconnectFS; break;
		case CSTRT_ChangeName       : fontStyle = theme->changeNameFS; break;
		case CSTRT_ChangeStatusMode : fontStyle = theme->changeStatusModeFS; break;
		case CSTRT_ToCryOut         : fontStyle = theme->toCryOutFS; break;
		case CSTRT_PrivateMsgSend   : fontStyle = theme->privateMsgSendFS; break;
		case CSTRT_PrivateMsgReceive: fontStyle = theme->privateMsgReceiveFS; break;
		default                     : fontStyle = CFS_NONE; break;
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
	int pos  = s.find( ' ', pos_from );
	int left = r.left;
	int top  = r.top;
	int smile_index = 0;
	int height_of_line = getOneLineHeight( dc, r.Width(), pos_from, smile_index );
	bool flag = true;
	while ( flag ) {
		if ( pos == std::string::npos ) {
			pos  = s.length();
			flag = false;
		}
		std::string s2 = s.substr( pos_from, pos - pos_from );
		if ( !s2.empty() ) {
			if ( chatApp.isWinNT() && smiles.getType( s2 ) != CChatSmile::none ) {
				CChatSmile* smile = smiles[smile_index];
				CSize size = smile->GetSize();
				if ( left + size.cx > r.right ) {
					left = r.left;
					if ( pos_from != 0 ) {
						top += height_of_line;
						height_of_line = getOneLineHeight( dc, r.Width(), pos_from, smile_index );
					}
				}
				smile->MoveWindow( left, top + height_of_line - size.cy, left + size.cx, top + height_of_line );
				left += size.cx;
				if ( flag ) {
					left += chatApp.style.spaceWidth;
				}
				smile_index++;
			} else {
				rect = CRect( 0, 0, 1, 1 );
				dc->DrawText( s2.c_str(), &rect, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_CALCRECT );
				if ( left + rect.right > r.right ) {
					left = r.left;
					if ( pos_from != 0 ) {
						top += height_of_line;
						height_of_line = getOneLineHeight( dc, r.Width(), pos_from, smile_index );
					}
				}
				rect.left    = left;
				rect.top     = top;
				rect.right  += left;
				rect.bottom  = top + height_of_line;
				dc->DrawText( s2.c_str(), &rect, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_BOTTOM );
				left += rect.Width();
				if ( flag ) {
					rect.left    = left;
					rect.top     = top;
					rect.right  += left;
					rect.bottom  = top + height_of_line;
					dc->DrawText( " ", &rect, DT_LEFT | DT_NOPREFIX | DT_SINGLELINE );
					left += chatApp.style.spaceWidth;
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
	int pos  = s.find( ' ', pos_from );
	int left = 0;
	int height_of_line = 0;
	bool flag = true;
	while ( flag ) {
		if ( pos == std::string::npos ) {
			pos  = s.length();
			flag = false;
		}
		std::string s2 = s.substr( pos_from, pos - pos_from );
		if ( !s2.empty() ) {
			if ( chatApp.isWinNT() && smiles.getType( s2 ) != CChatSmile::none ) {
				CChatSmile* smile = smiles[smile_index];
				CSize size = smile->GetSize();
				if ( !height_of_line ) {
					height_of_line = size.cy;
				}
				if ( left + size.cx > _width ) {
					return height_of_line;
				}
				if ( height_of_line < size.cy ) {
					height_of_line = size.cy;
				}
				left += size.cx;
				if ( flag ) {
					left += chatApp.style.spaceWidth;
				}
				smile_index++;
			} else {
				rect = CRect( 0, 0, 1, 1 );
				dc->DrawText( s2.c_str(), &rect, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_CALCRECT );
				if ( !height_of_line ) {
					height_of_line = rect.bottom;
				}
				if ( left + rect.right > _width ) {
					return height_of_line;
				}
				if ( height_of_line < rect.bottom ) {
					height_of_line = rect.bottom;
				}
				left += rect.right;
				if ( flag ) {
					left += chatApp.style.spaceWidth;
				}
			}
		}

		pos_from = pos + 1;
		pos = s.find( ' ', pos_from );
	}

	return height_of_line;
}

int CChatString::getHeight( CDC* dc, const int _width )
{
	if ( !height ) {
		CFont* prev_font = setFont( dc );
		CRect rect;
		std::string s = getString();
		int pos_from = 0;
		int pos  = s.find( ' ', pos_from );
		int left = 0;
		int top  = 0;
		int smile_index = 0;
		int height_of_line = getOneLineHeight( dc, _width, pos_from, smile_index );
		height = height_of_line;
		bool flag = true;
		while ( flag ) {
			if ( pos == std::string::npos ) {
				pos  = s.length();
				flag = false;
			}
			std::string s2 = s.substr( pos_from, pos - pos_from );
			if ( !s2.empty() ) {
				if ( chatApp.isWinNT() && smiles.getType( s2 ) != CChatSmile::none ) {
					CChatSmile* smile = smiles[smile_index];
					CSize size = smile->GetSize();
					if ( left + size.cx > _width ) {
						left = 0;
						if ( pos_from != 0 ) {
							top += height_of_line;
							height_of_line = getOneLineHeight( dc, _width, pos_from, smile_index );
							height += height_of_line;
						}
					}
					left += size.cx;
					if ( maxWidth < size.cx ) {
						maxWidth = size.cx;
					}
					if ( left > width ) {
						width = left;
					}
					if ( flag ) {
						left += chatApp.style.spaceWidth;
					}
					smile_index++;
				} else {
					rect = CRect( 0, 0, 1, 1 );
					dc->DrawText( s2.c_str(), &rect, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_CALCRECT );
					if ( left + rect.right > _width ) {
						left = 0;
						if ( pos_from != 0 ) {
							top += height_of_line;
							height_of_line = getOneLineHeight( dc, _width, pos_from, smile_index );
							height += height_of_line;
						}
					}
					left += rect.right;
					if ( maxWidth < rect.right ) {
						maxWidth = rect.right;
					}
					if ( left > width ) {
						width = left;
					}
					if ( flag ) {
						left += chatApp.style.spaceWidth;
					}
				}
			}

			pos_from = pos + 1;
			pos = s.find( ' ', pos_from );
		}

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
	if ( chatApp.isWinNT() ) {
		smiles.setBgColor( chatApp.style.theme.selectedBgColor );
	}
}

void CChatString::selectedOFF()
{
	if ( chatApp.isWinNT() ) {
		smiles.setBgColor( getBgColor() );
	}
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
	if ( selected != _selected && _selected >=0 && _selected < count() ) {
		selected = _selected;
		if ( selected != -1 ) list[ selected ]->selectedON();
	}
}

void CChatStringList::clearSelected( const int _selected )
{
	selected = -1;
	if ( _selected != -1 && _selected >=0 && _selected < count() ) list[ _selected ]->selectedOFF();
}
