#ifndef RDOLOGEDITSTYLE_H
#define RDOLOGEDITSTYLE_H
#pragma once

#include "rdobaseeditstyle.h"

// ----------------------------------------------------------------------------
// ---------- RDOLogEditTheme
// ----------------------------------------------------------------------------
class RDOLogEditTheme: public rdoStyle::RDOBaseEditTheme
{
public:
	RDOLogEditTheme();
	virtual ~RDOLogEditTheme();

	RDOLogEditTheme& operator =( const RDOLogEditTheme& theme );
	bool operator ==( const RDOLogEditTheme& theme ) const;
	bool operator !=( const RDOLogEditTheme& theme ) const;

	virtual void load( string regPath );
	virtual void save( string regPath ) const;

	COLORREF selectLineBgColor;
};

// ----------------------------------------------------------------------------
// ---------- RDOLogEditStyle
// ----------------------------------------------------------------------------
class RDOLogEditStyle: public rdoStyle::RDOBaseEditStyle
{
protected:
	virtual void initTheme();

public:
	RDOLogEditStyle();
	virtual ~RDOLogEditStyle();

	RDOLogEditStyle& operator =( const RDOLogEditStyle& style );
	bool operator ==( const RDOLogEditStyle& style ) const;
	bool operator !=( const RDOLogEditStyle& style ) const;
};

#endif // RDOLOGEDITSTYLE_H
