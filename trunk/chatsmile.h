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
	enum Type { eyes };

private:
	Type type;

public:
	CChatSmile();
	virtual ~CChatSmile();
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

	CChatSmile* getSmile( const CChatSmile::Type type );
	CChatSmile* operator[] ( const int index ) { return list[index]; }
	int count() const                          { return list.size(); }
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATSMILES_H
