#ifndef CHATSOUND_H
#define CHATSOUND_H
#pragma once

#include <afxtempl.h>

// ----------------------------------------------------------------------------
// ---------- CChatSound
// ----------------------------------------------------------------------------
enum CChatSoundType {
	CST_Connect = 0,
	CST_Disconnect,
	CST_ChangeStatusMode,
	CST_IncomingMessage,
	CST_ChatType,
	CST_ChatBack,
	CST_ChatRet
};

class CChatSound
{
public:
	CChatSoundType type;
	bool           useSound;
	bool           useDefault;
	int            res;
	std::string    file;

	CChatSound();
	CChatSound( const CChatSound& snd );
	virtual ~CChatSound();

	CChatSound& operator= ( const CChatSound& snd );
	bool operator== ( const CChatSound& snd );
	bool operator!= ( const CChatSound& snd );
};

// ----------------------------------------------------------------------------
// ---------- CChatSoundList
// ----------------------------------------------------------------------------
class CChatSoundList: public CList< CChatSound*, CChatSound* >
{
protected:
	bool useSound;

	void play( const int resID );
	void play( const std::string& file );
	CChatSound* getSound( const CChatSoundType soundType ) const;

public:
	CChatSoundList();
	virtual ~CChatSoundList();

	bool getUseSound() const;
	void setUseSound( const bool value );

	void play( const CChatSoundType soundType );

	std::string getName( const CChatSoundType soundType ) const;

	void init();
	void saveSetting() const;
};

#endif // CHATSOUND_H
