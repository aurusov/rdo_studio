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
	enum Type { none, smile, frown, wink, smirk, tongue, biggrin, gigi, beer, lamer, friday, eyes, tomato };

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
	CChatSmile::Type getType( const std::string& str );
	CChatSmile* operator[] ( const int index ) const { return list[index]; }
	int count() const                                { return list.size(); }

	void setBgColor( COLORREF color );
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATSMILES_H
