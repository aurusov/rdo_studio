#include "stdafx.h"
#include "chatviewerstyle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatViewerTheme
// ----------------------------------------------------------------------------
CChatViewerTheme::CChatViewerTheme()
{
	messageColor            = RGB( 0x00, 0x00, 0x00 );
	messageBg1Color         = RGB( 0xEB, 0xF5, 0xFE );
	messageBg2Color         = RGB( 0xDF, 0xE9, 0xF0 );
	connectColor            = RGB( 0x00, 0x00, 0x00 );
	connectBgColor          = RGB( 0xBD, 0xEC, 0xB3 );
	disconnectColor         = RGB( 0x00, 0x00, 0x00 );
	disconnectBgColor       = RGB( 0xE3, 0xB3, 0xEC );
	changeNameColor         = RGB( 0x00, 0x00, 0x00 );
	changeNameBgColor       = RGB( 0xEC, 0xE2, 0xB3 );
	changeStatusModeColor   = RGB( 0x00, 0x00, 0x00 );
	changeStatusModeBgColor = RGB( 0xBC, 0xCF, 0xB3 );
	toCryOutColor           = RGB( 0xFF, 0xFF, 0xFF );
	toCryOutBgColor         = RGB( 0xBC, 0x10, 0x20 );
	popupMsgSendColor       = RGB( 0x00, 0x00, 0x00 );
	popupMsgSendBgColor     = RGB( 0x4B, 0xD4, 0x40 );
	popupMsgReceiveColor    = RGB( 0x00, 0x00, 0x00 );
	popupMsgReceiveBgColor  = RGB( 0x36, 0xCC, 0x5A );
	viewerBgColor           = RGB( 0xFA, 0xFD, 0xFE );
	selectedBgColor         = RGB( 0xA0, 0xA0, 0xA0 );

	messageFS          = CFS_NONE;
	connectFS          = CFS_NONE;
	disconnectFS       = CFS_NONE;
	changeNameFS       = CFS_NONE;
	changeStatusModeFS = CFS_NONE;
	toCryOutFS         = CFS_NONE;
	popupMsgSendFS     = CFS_NONE;
	popupMsgReceiveFS  = CFS_NONE;
}

CChatViewerTheme::~CChatViewerTheme()
{
}

CChatViewerTheme& CChatViewerTheme::operator =( const CChatViewerTheme& theme )
{
	messageColor            = theme.messageColor;
	messageBg1Color         = theme.messageBg1Color;
	messageBg2Color         = theme.messageBg2Color;
	connectColor            = theme.connectColor;
	connectBgColor          = theme.connectBgColor;
	disconnectColor         = theme.disconnectColor;
	disconnectBgColor       = theme.disconnectBgColor;
	changeNameColor         = theme.changeNameColor;
	changeNameBgColor       = theme.changeNameBgColor;
	changeStatusModeColor   = theme.changeStatusModeColor;
	changeStatusModeBgColor = theme.changeStatusModeBgColor;
	toCryOutColor           = theme.toCryOutColor;
	toCryOutBgColor         = theme.toCryOutBgColor;
	popupMsgSendColor       = theme.popupMsgSendColor;
	popupMsgSendBgColor     = theme.popupMsgSendBgColor;
	popupMsgReceiveColor    = theme.popupMsgReceiveColor;
	popupMsgReceiveBgColor  = theme.popupMsgReceiveBgColor;
	viewerBgColor           = theme.viewerBgColor;

	messageFS          = theme.messageFS;
	connectFS          = theme.connectFS;
	disconnectFS       = theme.disconnectFS;
	changeNameFS       = theme.changeNameFS;
	changeStatusModeFS = theme.changeStatusModeFS;
	toCryOutFS         = theme.toCryOutFS;
	popupMsgSendFS     = theme.popupMsgSendFS;
	popupMsgReceiveFS  = theme.popupMsgReceiveFS;

	return *this;
}

bool CChatViewerTheme::operator ==( const CChatViewerTheme& theme ) const
{
	return messageColor            == theme.messageColor &&
	       messageBg1Color         == theme.messageBg1Color &&
	       messageBg2Color         == theme.messageBg2Color &&
	       connectColor            == theme.connectColor &&
	       connectBgColor          == theme.connectBgColor &&
	       disconnectColor         == theme.disconnectColor &&
	       disconnectBgColor       == theme.disconnectBgColor &&
	       changeNameColor         == theme.changeNameColor &&
	       changeNameBgColor       == theme.changeNameBgColor &&
	       changeStatusModeColor   == theme.changeStatusModeColor &&
	       changeStatusModeBgColor == theme.changeStatusModeBgColor &&
	       toCryOutColor           == theme.toCryOutColor &&
	       toCryOutBgColor         == theme.toCryOutBgColor &&
	       popupMsgSendColor       == theme.popupMsgSendColor &&
	       popupMsgSendBgColor     == theme.popupMsgSendBgColor &&
	       popupMsgReceiveColor    == theme.popupMsgReceiveColor &&
	       popupMsgReceiveBgColor  == theme.popupMsgReceiveBgColor &&
	       viewerBgColor           == theme.viewerBgColor &&

	       messageFS          == theme.messageFS &&
	       connectFS          == theme.connectFS &&
	       disconnectFS       == theme.disconnectFS &&
	       changeNameFS       == theme.changeNameFS &&
	       changeStatusModeFS == theme.changeStatusModeFS &&
	       toCryOutFS         == theme.toCryOutFS &&
	       popupMsgSendFS     == theme.popupMsgSendFS &&
	       popupMsgReceiveFS  == theme.popupMsgReceiveFS;
}

bool CChatViewerTheme::operator !=( const CChatViewerTheme& theme ) const
{
	return !(*this == theme);
}

CChatViewerTheme CChatViewerTheme::getDefaultTheme()
{
	CChatViewerTheme theme;
	return theme;
}

void CChatViewerTheme::init()
{
	CChatViewerTheme theme = getDefaultTheme();

	CWinApp* app = AfxGetApp();
	messageColor            = app->GetProfileInt( "Style\\Theme", "messageColor", theme.messageColor );
	messageBg1Color         = app->GetProfileInt( "Style\\Theme", "messageBg1Color", theme.messageBg1Color );
	messageBg2Color         = app->GetProfileInt( "Style\\Theme", "messageBg2Color", theme.messageBg2Color );
	connectColor            = app->GetProfileInt( "Style\\Theme", "connectColor", theme.connectColor );
	connectBgColor          = app->GetProfileInt( "Style\\Theme", "connectBgColor", theme.connectBgColor );
	disconnectColor         = app->GetProfileInt( "Style\\Theme", "disconnectColor", theme.disconnectColor );
	disconnectBgColor       = app->GetProfileInt( "Style\\Theme", "disconnectBgColor", theme.disconnectBgColor );
	changeNameColor         = app->GetProfileInt( "Style\\Theme", "changeNameColor", theme.changeNameColor );
	changeNameBgColor       = app->GetProfileInt( "Style\\Theme", "changeNameBgColor", theme.changeNameBgColor );
	changeStatusModeColor   = app->GetProfileInt( "Style\\Theme", "changeStatusModeColor", theme.changeStatusModeColor );
	changeStatusModeBgColor = app->GetProfileInt( "Style\\Theme", "changeStatusModeBgColor", theme.changeStatusModeBgColor );
	toCryOutColor           = app->GetProfileInt( "Style\\Theme", "toCryOutColor", theme.toCryOutColor );
	toCryOutBgColor         = app->GetProfileInt( "Style\\Theme", "toCryOutBgColor", theme.toCryOutBgColor );
	popupMsgSendColor       = app->GetProfileInt( "Style\\Theme", "popupMsgSendColor", theme.popupMsgSendColor );
	popupMsgSendBgColor     = app->GetProfileInt( "Style\\Theme", "popupMsgSendBgColor", theme.popupMsgSendBgColor );
	popupMsgReceiveColor    = app->GetProfileInt( "Style\\Theme", "popupMsgReceiveColor", theme.popupMsgReceiveColor );
	popupMsgReceiveBgColor  = app->GetProfileInt( "Style\\Theme", "popupMsgReceiveBgColor", theme.popupMsgReceiveBgColor );
	viewerBgColor           = app->GetProfileInt( "Style\\Theme", "viewerBgColor", theme.viewerBgColor );

	messageFS          = static_cast<CChatFontStyle>(app->GetProfileInt( "Style\\Theme", "messageFS", theme.messageFS ));
	connectFS          = static_cast<CChatFontStyle>(app->GetProfileInt( "Style\\Theme", "connectFS", theme.connectFS ));
	disconnectFS       = static_cast<CChatFontStyle>(app->GetProfileInt( "Style\\Theme", "disconnectFS", theme.disconnectFS ));
	changeNameFS       = static_cast<CChatFontStyle>(app->GetProfileInt( "Style\\Theme", "changeNameFS", theme.changeNameFS ));
	changeStatusModeFS = static_cast<CChatFontStyle>(app->GetProfileInt( "Style\\Theme", "changeStatusModeFS", theme.changeStatusModeFS ));
	toCryOutFS         = static_cast<CChatFontStyle>(app->GetProfileInt( "Style\\Theme", "toCryOutFS", theme.toCryOutFS ));
	popupMsgSendFS     = static_cast<CChatFontStyle>(app->GetProfileInt( "Style\\Theme", "popupMsgSendFS", theme.popupMsgSendFS ));
	popupMsgReceiveFS  = static_cast<CChatFontStyle>(app->GetProfileInt( "Style\\Theme", "popupMsgReceiveFS", theme.popupMsgReceiveFS ));
}

void CChatViewerTheme::saveSetting() const
{
	CWinApp* app = AfxGetApp();
	app->WriteProfileInt( "Style\\Theme", "messageColor", messageColor );
	app->WriteProfileInt( "Style\\Theme", "messageColor", messageColor );
	app->WriteProfileInt( "Style\\Theme", "messageBg1Color", messageBg1Color );
	app->WriteProfileInt( "Style\\Theme", "messageBg2Color", messageBg2Color );
	app->WriteProfileInt( "Style\\Theme", "connectColor", connectColor );
	app->WriteProfileInt( "Style\\Theme", "connectBgColor", connectBgColor );
	app->WriteProfileInt( "Style\\Theme", "disconnectColor", disconnectColor );
	app->WriteProfileInt( "Style\\Theme", "disconnectBgColor", disconnectBgColor );
	app->WriteProfileInt( "Style\\Theme", "changeNameColor", changeNameColor );
	app->WriteProfileInt( "Style\\Theme", "changeNameBgColor", changeNameBgColor );
	app->WriteProfileInt( "Style\\Theme", "changeStatusModeColor", changeStatusModeColor );
	app->WriteProfileInt( "Style\\Theme", "changeStatusModeBgColor", changeStatusModeBgColor );
	app->WriteProfileInt( "Style\\Theme", "toCryOutColor", toCryOutColor );
	app->WriteProfileInt( "Style\\Theme", "toCryOutBgColor", toCryOutBgColor );
	app->WriteProfileInt( "Style\\Theme", "popupMsgSendColor", popupMsgSendColor );
	app->WriteProfileInt( "Style\\Theme", "popupMsgSendBgColor", popupMsgSendBgColor );
	app->WriteProfileInt( "Style\\Theme", "popupMsgReceiveColor", popupMsgReceiveColor );
	app->WriteProfileInt( "Style\\Theme", "popupMsgReceiveBgColor", popupMsgReceiveBgColor );
	app->WriteProfileInt( "Style\\Theme", "viewerBgColor", viewerBgColor );

	app->WriteProfileInt( "Style\\Theme", "messageFS", messageFS );
	app->WriteProfileInt( "Style\\Theme", "connectFS", connectFS );
	app->WriteProfileInt( "Style\\Theme", "disconnectFS", disconnectFS );
	app->WriteProfileInt( "Style\\Theme", "changeNameFS", changeNameFS );
	app->WriteProfileInt( "Style\\Theme", "changeStatusModeFS", changeStatusModeFS );
	app->WriteProfileInt( "Style\\Theme", "toCryOutFS", toCryOutFS );
	app->WriteProfileInt( "Style\\Theme", "popupMsgSendFS", popupMsgSendFS );
	app->WriteProfileInt( "Style\\Theme", "popupMsgReceiveFS", popupMsgReceiveFS );
}

// ----------------------------------------------------------------------------
// ---------- CChatViewerFont
// ----------------------------------------------------------------------------
CChatViewerFont::CChatViewerFont()
{
	name         = "Courier";
	size         = 10;
	codepage     = 0;
	characterSet = 1;
	if ( PRIMARYLANGID(GetSystemDefaultLangID()) == LANG_RUSSIAN ) {
		characterSet = RUSSIAN_CHARSET;
	}

	HFONT hf = (HFONT)::GetStockObject( SYSTEM_FIXED_FONT );
	if ( hf ) {
		CFont* f = CFont::FromHandle( hf );
		if ( f ) {
			LOGFONT lf;
			f->GetLogFont( &lf );
			name         = lf.lfFaceName;
			characterSet = lf.lfCharSet;
		}
	}
}

CChatViewerFont::~CChatViewerFont()
{
}

CChatViewerFont& CChatViewerFont::operator =( const CChatViewerFont& font )
{
	name         = font.name;
	size         = font.size;
	codepage     = font.codepage;
	characterSet = font.characterSet;

	return *this;
}

bool CChatViewerFont::operator ==( const CChatViewerFont& font ) const
{
	return name         == font.name &&
	       size         == font.size &&
	       codepage     == font.codepage &&
	       characterSet == font.characterSet;
}

bool CChatViewerFont::operator !=( const CChatViewerFont& font ) const
{
	return !(*this == font);
}

void CChatViewerFont::init()
{
	CWinApp* app = AfxGetApp();
	name         = app->GetProfileString( "Style\\Font", "name", name.c_str() );
	size         = app->GetProfileInt( "Style\\Font", "size", size );
	codepage     = app->GetProfileInt( "Style\\Font", "codepage", codepage );
	characterSet = app->GetProfileInt( "Style\\Font", "characterSet", characterSet );
}

void CChatViewerFont::saveSetting() const
{
	CWinApp* app = AfxGetApp();

	app->WriteProfileString( "Style\\Font", "name", name.c_str() );
	app->WriteProfileInt( "Style\\Font", "size", size );
	app->WriteProfileInt( "Style\\Font", "codepage", codepage );
	app->WriteProfileInt( "Style\\Font", "characterSet", characterSet );
}

// ----------------------------------------------------------------------------
// ---------- CChatViewerStyle
// ----------------------------------------------------------------------------
CChatViewerStyle::CChatViewerStyle():
	charWidth( 0 ),
	charHeight( 0 ),
	spaceWidth( 0 ),
	horzBorder( 0 )
{
}

CChatViewerStyle::~CChatViewerStyle()
{
}

CChatViewerStyle& CChatViewerStyle::operator =( const CChatViewerStyle& style )
{
	theme      = style.theme;
	font       = style.font;
	horzBorder = style.horzBorder;

	fontChanged();

	return *this;
}

bool CChatViewerStyle::operator ==( const CChatViewerStyle& style ) const
{
	return theme      == style.theme &&
	       font       == style.font &&
	       horzBorder == style.horzBorder;
}

bool CChatViewerStyle::operator !=( const CChatViewerStyle& style ) const
{
	return !(*this == style);
}

void CChatViewerStyle::init()
{
	theme.init();
	font.init();

	CWinApp* app = AfxGetApp();
	horzBorder = app->GetProfileInt( "Style", "horzBorder", horzBorder );

	fontChanged();
}

void CChatViewerStyle::saveSetting() const
{
	theme.saveSetting();
	font.saveSetting();

	CWinApp* app = AfxGetApp();
	app->WriteProfileInt( "Style", "horzBorder", horzBorder );
}

void CChatViewerStyle::fontChanged()
{
	if ( fontFSN.m_hObject ) fontFSN.DeleteObject();
	if ( fontFSB.m_hObject ) fontFSB.DeleteObject();
	if ( fontFSI.m_hObject ) fontFSI.DeleteObject();
	if ( fontFSU.m_hObject ) fontFSU.DeleteObject();
	if ( fontFSBI.m_hObject ) fontFSBI.DeleteObject();
	if ( fontFSBU.m_hObject ) fontFSBU.DeleteObject();
	if ( fontFSIU.m_hObject ) fontFSIU.DeleteObject();
	if ( fontFSBIU.m_hObject ) fontFSBIU.DeleteObject();

	LOGFONT lf;
	memset( &lf, 0, sizeof(LOGFONT) );
	strcpy( lf.lfFaceName, font.name.c_str() );
	CDC* dc = CWnd::GetDesktopWindow()->GetDC();
	lf.lfHeight    = -MulDiv( font.size, dc->GetDeviceCaps( LOGPIXELSY ), 72 );
	lf.lfCharSet   = font.characterSet;

	lf.lfWeight    = FW_NORMAL;
	lf.lfItalic    = 0;
	lf.lfUnderline = 0;
	fontFSN.CreateFontIndirect( &lf );

	lf.lfWeight    = FW_BOLD;
	lf.lfItalic    = 0;
	lf.lfUnderline = 0;
	fontFSB.CreateFontIndirect( &lf );

	lf.lfWeight    = FW_NORMAL;
	lf.lfItalic    = CFS_ITALIC;
	lf.lfUnderline = 0;
	fontFSI.CreateFontIndirect( &lf );

	lf.lfWeight    = FW_NORMAL;
	lf.lfItalic    = 0;
	lf.lfUnderline = CFS_UNDERLINE;
	fontFSU.CreateFontIndirect( &lf );

	lf.lfWeight    = FW_BOLD;
	lf.lfItalic    = CFS_ITALIC;
	lf.lfUnderline = 0;
	fontFSBI.CreateFontIndirect( &lf );

	lf.lfWeight    = FW_BOLD;
	lf.lfItalic    = 0;
	lf.lfUnderline = CFS_UNDERLINE;
	fontFSBU.CreateFontIndirect( &lf );

	lf.lfWeight    = FW_NORMAL;
	lf.lfItalic    = CFS_ITALIC;
	lf.lfUnderline = CFS_UNDERLINE;
	fontFSIU.CreateFontIndirect( &lf );

	lf.lfWeight    = FW_BOLD;
	lf.lfItalic    = CFS_ITALIC;
	lf.lfUnderline = CFS_UNDERLINE;
	fontFSBIU.CreateFontIndirect( &lf );

	CFont* prev_font = dc->SelectObject( &fontFSN );

	TEXTMETRIC tm;
	dc->GetTextMetrics( &tm );
	charWidth  = tm.tmAveCharWidth;
	charHeight = tm.tmHeight;

	spaceWidth = dc->GetTextExtent( " " ).cx;

	dc->SelectObject( prev_font );
	CWnd::GetDesktopWindow()->ReleaseDC( dc );
}
