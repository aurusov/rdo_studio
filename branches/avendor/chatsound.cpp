#include "stdafx.h"
#include "chatsound.h"
#include "resource.h"

#include <mmsystem.h>

// ----------------------------------------------------------------------------
// ---------- CChatSound
// ----------------------------------------------------------------------------
CChatSound::CChatSound()
{
}

CChatSound::CChatSound( const CChatSound& snd )
{
	*this = snd;
}

CChatSound::~CChatSound()
{
}

CChatSound& CChatSound::operator= ( const CChatSound& snd )
{
	type       = snd.type;
	useSound   = snd.useSound;
	useDefault = snd.useDefault;
	res        = snd.res;
	file       = snd.file;
	return *this;
}

bool CChatSound::operator== ( const CChatSound& snd )
{
	return type       == snd.type &&
	       useSound   == snd.useSound &&
	       useDefault == snd.useDefault &&
	       res        == snd.res &&
	       file       == snd.file;
}

bool CChatSound::operator!= ( const CChatSound& snd )
{
	return !( *this == snd );
}

// ----------------------------------------------------------------------------
// ---------- CChatSoundList
// ----------------------------------------------------------------------------
CChatSoundList::CChatSoundList(): CList< CChatSound*, CChatSound* >()
{
}

CChatSoundList::~CChatSoundList()
{
	while ( POSITION pos = GetTailPosition() ) {
		delete GetAt( pos );
		RemoveTail();
	}
}

void CChatSoundList::init()
{
	useSound = AfxGetApp()->GetProfileInt( "Sound", "useSound", true ) ? true : false;

	CWinApp* app = AfxGetApp();
	CChatSound* snd;
	snd = new CChatSound();
	snd->type       = CST_Connect;
	snd->useSound   = app->GetProfileInt( "Sound\\Connect", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\Connect", "useDefault", true ) ? true : false;
	snd->res        = IDR_CONNECT_WAVE;
	snd->file       = app->GetProfileString( "Sound\\Connect", "file", "" );
	AddTail( snd );
	snd = new CChatSound();
	snd->type       = CST_Disconnect;
	snd->useSound   = app->GetProfileInt( "Sound\\Disconnect", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\Disconnect", "useDefault", true ) ? true : false;
	snd->res        = IDR_DISCONNECT_WAVE;
	snd->file       = app->GetProfileString( "Sound\\Disconnect", "file", "" );
	AddTail( snd );
	snd = new CChatSound();
	snd->type       = CST_ChangeStatusMode;
	snd->useSound   = app->GetProfileInt( "Sound\\ChangeStatusMode", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\ChangeStatusMode", "useDefault", true ) ? true : false;
	snd->res        = IDR_CHANGESTATUSMODE_WAVE;
	snd->file       = app->GetProfileString( "Sound\\ChangeStatusMode", "file", "" );
	AddTail( snd );
	snd = new CChatSound();
	snd->type       = CST_IncomingMessage;
	snd->useSound   = app->GetProfileInt( "Sound\\IncomingMessage", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\IncomingMessage", "useDefault", true ) ? true : false;
	snd->res        = IDR_INCOMINGMESSAGE_WAVE;
	snd->file       = app->GetProfileString( "Sound\\IncomingMessage", "file", "" );
	AddTail( snd );
	snd = new CChatSound();
	snd->type       = CST_ChatType;
	snd->useSound   = app->GetProfileInt( "Sound\\ChatType", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\ChatType", "useDefault", true ) ? true : false;
	snd->res        = IDR_CHATTYPE_WAVE;
	snd->file       = app->GetProfileString( "Sound\\ChatType", "file", "" );
	AddTail( snd );
	snd = new CChatSound();
	snd->type       = CST_ChatBack;
	snd->useSound   = app->GetProfileInt( "Sound\\ChatBack", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\ChatBack", "useDefault", true ) ? true : false;
	snd->res        = IDR_CHATBACK_WAVE;
	snd->file       = app->GetProfileString( "Sound\\ChatBack", "file", "" );
	AddTail( snd );
	snd = new CChatSound();
	snd->type       = CST_ChatRet;
	snd->useSound   = app->GetProfileInt( "Sound\\ChatRet", "useSound", true ) ? true : false;
	snd->useDefault = app->GetProfileInt( "Sound\\ChatRet", "useDefault", true ) ? true : false;
	snd->res        = IDR_CHATRET_WAVE;
	snd->file       = app->GetProfileString( "Sound\\ChatRet", "file", "" );
	AddTail( snd );
}

void CChatSoundList::saveSetting() const
{
	CWinApp* app = AfxGetApp();
	CChatSound* snd;
	snd = getSound( CST_Connect );
	app->WriteProfileInt( "Sound\\Connect", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\Connect", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\Connect", "file", snd->file );
	snd = getSound( CST_Disconnect );
	app->WriteProfileInt( "Sound\\Disconnect", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\Disconnect", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\Disconnect", "file", snd->file );
	snd = getSound( CST_ChangeStatusMode );
	app->WriteProfileInt( "Sound\\ChangeStatusMode", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\ChangeStatusMode", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\ChangeStatusMode", "file", snd->file );
	snd = getSound( CST_IncomingMessage );
	app->WriteProfileInt( "Sound\\IncomingMessage", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\IncomingMessage", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\IncomingMessage", "file", snd->file );
	snd = getSound( CST_ChatType );
	app->WriteProfileInt( "Sound\\ChatType", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\ChatType", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\ChatType", "file", snd->file );
	snd = getSound( CST_ChatBack );
	app->WriteProfileInt( "Sound\\ChatBack", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\ChatBack", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\ChatBack", "file", snd->file );
	snd = getSound( CST_ChatRet );
	app->WriteProfileInt( "Sound\\ChatRet", "useSound", snd->useSound );
	app->WriteProfileInt( "Sound\\ChatRet", "useDefault", snd->useDefault );
	app->WriteProfileString( "Sound\\ChatRet", "file", snd->file );
}

void CChatSoundList::play( const int resID )
{
	::sndPlaySound( NULL, 0 );
	HINSTANCE hinst = AfxGetInstanceHandle();
	HRSRC hResInfo= ::FindResource( hinst, MAKEINTRESOURCE( resID ), "WAVE" );
	if( hResInfo != NULL ) {

		HANDLE hRes = ::LoadResource( hinst, hResInfo );
		if ( hRes != NULL ) {

			LPSTR lpRes = (LPSTR)::LockResource( hRes );
			if( lpRes != NULL ) {

				::sndPlaySound( lpRes, SND_MEMORY | SND_ASYNC );
				::FreeResource( hRes );
			}
		}
	}
}

void CChatSoundList::play( const CString& file )
{
	bool fileExist = true;
	HANDLE hFile;
	hFile = ::CreateFile( file, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hFile == INVALID_HANDLE_VALUE ) fileExist = false;
	::CloseHandle(hFile);
	if ( fileExist ) {
		::sndPlaySound( NULL, 0 );
		::sndPlaySound( file, SND_ASYNC );
	}
}

CChatSound* CChatSoundList::getSound( const CChatSoundType soundType ) const
{
	POSITION pos = GetHeadPosition();
	for ( int i = 0; i < GetCount(); i++ ) {
		if ( GetAt( pos )->type == soundType ) {
			return GetAt( pos );
		}
		GetNext( pos );
	}
	return NULL;
}

void CChatSoundList::play( const CChatSoundType soundType )
{
	if ( useSound ) {
		CChatSound* snd = getSound( soundType );
		if ( snd && snd->useSound ) {
			if ( snd->useDefault ) {
				play( snd->res );
			} else {
				if ( !snd->file.IsEmpty() ) {
					play( snd->file );
				}
			}
		}
	}
}

bool CChatSoundList::getUseSound() const
{
	return useSound;
}

void CChatSoundList::setUseSound( const bool value )
{
	if ( useSound != value ) {
		useSound = value;
		if ( !useSound ) {
			::sndPlaySound( NULL, 0 );
		}
		AfxGetApp()->WriteProfileInt( "Sound", "useSound", useSound );
	}
}

CString CChatSoundList::getName( const CChatSoundType soundType ) const
{
	CString s;
	int res;
	switch ( soundType ) {
		case CST_Connect         : res = ID_CONNECT_WAVE; break;
		case CST_Disconnect      : res = ID_DISCONNECT_WAVE; break;
		case CST_ChangeStatusMode: res = ID_CHANGESTATUSMODE_WAVE; break;
		case CST_IncomingMessage : res = ID_INCOMINGMESSAGE_WAVE; break;
		case CST_ChatType        : res = ID_CHATTYPE_WAVE; break;
		case CST_ChatBack        : res = ID_CHATBACK_WAVE; break;
		case CST_ChatRet         : res = ID_CHATRET_WAVE; break;
		default                  : res = -1;
	}
	if ( res != -1 ) {
		s.LoadString( res );
	} else {
		s = "";
	}
	return s;
}
