#ifndef CHATSOUND_H
#define CHATSOUND_H

#if _MSC_VER > 1000
#pragma once
#endif

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
	CST_ChatRet,
	CST_PrivateMsg,
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
class CChatSoundList
{
private:
	std::vector< CChatSound* > list;
	void clear();

	bool useSound;

	void play( const int resID );
	void play( const std::string& file );
	CChatSound* getSound( const CChatSoundType soundType ) const;

public:
	CChatSoundList();
	virtual ~CChatSoundList();

	CChatSoundList& operator= ( const CChatSoundList& soundList );
	bool operator== ( const CChatSoundList& soundList );
	bool operator!= ( const CChatSoundList& soundList );
	CChatSound* operator[] ( const int index ) { return list[index]; }
	int count() const                          { return list.size(); }

	bool getUseSound() const { return useSound; }
	void setUseSound( const bool value );

	void play( const CChatSoundType soundType );

	std::string getName( const CChatSoundType soundType ) const;

	void init();
	void saveSetting() const;
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATSOUND_H
