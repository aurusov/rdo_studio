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
	enum Type { smile, frown, wink, smirk, tongue, biggrin, gigi, eyes, beer1, beer2, lamer, tomato };

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

	CChatSmile* addSmile( const CChatSmile::Type type );
	CChatSmile* operator[] ( const int index ) const { return list[index]; }
	int count() const                                { return list.size(); }

	void setBgColor( COLORREF color );
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATSMILES_H
