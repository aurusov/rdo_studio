#ifndef CHATSMILES_H
#define CHATSMILES_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "PictureEx/PictureEx.h"

// ----------------------------------------------------------------------------
// ---------- CChatSmile
// ----------------------------------------------------------------------------
class CChatSmile: public CPictureEx
{
friend class CChatSmileList;
friend class CChatSmileListCtrl;

public:
	enum Type { none = 0, smile, frown, wink, smirk, tongue, biggrin, gigi, laugh, lol, cool, confused, eek, rolleyes, insane, redface, mad, weep, up, down, rotate, super, upset, beer, amazed, shuffle, lamer, spy, idea, puke, wink2, smirk2, think, fear, yes, no, crazy, joke, hah, ballet, friday, drink, fart, box, invalid, duel, kos, gossip, bis, hit, ivan, venera, mobile, kult, figa, eyes, tomato };

	CChatSmile( const Type _type, CWnd* parent );
	virtual ~CChatSmile();

	Type getType() const { return type; }

	void setBgColor( COLORREF color );
	void showAnimation( const bool value );

private:
	Type type;
};

// ----------------------------------------------------------------------------
// ---------- CChatSmileList
// ----------------------------------------------------------------------------
class CChatSmileList
{
private:
	std::vector< CChatSmile* > list;

public:
	CChatSmileList();
	virtual ~CChatSmileList();

	CChatSmile* addSmile( const std::string& str, CWnd* parent );
	CChatSmile* addSmile( const CChatSmile::Type type, CWnd* parent );
	static CChatSmile::Type getType( std::string str );
	static std::string getStr( const CChatSmile::Type type );
	static std::string getInfo( const CChatSmile::Type type );
	CChatSmile* operator[] ( const int index ) const { return list[index]; }
	int count() const                                { return list.size(); }

	void setBgColor( COLORREF color );
	void showAnimation( const bool value );
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATSMILES_H
