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

public:
	enum Type { none, smile, frown, wink, smirk, tongue, biggrin, gigi, laugh, lol, cool, confused, eek, rolleyes, insane, redface, mad, weep, up, down, rotate, super, upset, beer, amazed, shuffle, lamer, spy, idea, puke, wink2, smirk2, think, fear, yes, no, crazy, joke, hah, ballet, friday, drink, fart, box, invalid, duel, kos, gossip, bis, hit, ivan, venera, mobile, kult, figa, eyes, tomato };

private:
	CChatSmile();
	virtual ~CChatSmile();

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

	CChatSmile* addSmile( const std::string& str );
	CChatSmile* addSmile( const CChatSmile::Type type );
	CChatSmile::Type getType( std::string str );
	CChatSmile* operator[] ( const int index ) const { return list[index]; }
	int count() const                                { return list.size(); }

	void setBgColor( COLORREF color );
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATSMILES_H
