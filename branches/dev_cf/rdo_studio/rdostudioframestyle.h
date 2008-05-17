#ifndef RDOSTUDIOFRAMESTYLE_H
#define RDOSTUDIOFRAMESTYLE_H
#pragma once

#include "rdostudiostyle.h"

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameTheme
// ----------------------------------------------------------------------------
class RDOStudioFrameTheme: public rdoStyle::RDOStyleTheme
{
public:
	RDOStudioFrameTheme();
	virtual ~RDOStudioFrameTheme();

	RDOStudioFrameTheme& operator =( const RDOStudioFrameTheme& theme );
	bool operator ==( const RDOStudioFrameTheme& theme ) const;
	bool operator !=( const RDOStudioFrameTheme& theme ) const;

	static RDOStudioFrameTheme getDefaultTheme();
};

// ----------------------------------------------------------------------------
// ---------- RDOStudioFrameStyle
// ----------------------------------------------------------------------------
class RDOStudioFrameStyle: public rdoStyle::RDOStyleWithTheme
{
protected:
	virtual void initTheme();

public:
	RDOStudioFrameStyle();
	virtual ~RDOStudioFrameStyle();

	RDOStudioFrameStyle& operator =( const RDOStudioFrameStyle& style );
	bool operator ==( const RDOStudioFrameStyle& style ) const;
	bool operator !=( const RDOStudioFrameStyle& style ) const;

	virtual void init( const std::string& _regPath = "" );
};

#endif // RDOSTUDIOFRAMESTYLE_H
