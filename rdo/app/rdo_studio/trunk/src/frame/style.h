#ifndef RDOSTUDIOFRAMESTYLE_H
#define RDOSTUDIOFRAMESTYLE_H

#include "app/rdo_studio_mfc/src/style.h"

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameTheme
// --------------------------------------------------------------------------------
class RDOStudioFrameTheme: public rdoStyle::RDOStyleTheme
{
public:
	RDOStudioFrameTheme();
	virtual ~RDOStudioFrameTheme();

	RDOStudioFrameTheme& operator =( const RDOStudioFrameTheme& theme );
	rbool operator ==( const RDOStudioFrameTheme& theme ) const;
	rbool operator !=( const RDOStudioFrameTheme& theme ) const;

	static RDOStudioFrameTheme getDefaultTheme();
};

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameStyle
// --------------------------------------------------------------------------------
class RDOStudioFrameStyle: public rdoStyle::RDOStyleWithTheme
{
protected:
	virtual void initTheme();

public:
	RDOStudioFrameStyle();
	virtual ~RDOStudioFrameStyle();

	RDOStudioFrameStyle& operator =( const RDOStudioFrameStyle& style );
	rbool operator ==( const RDOStudioFrameStyle& style ) const;
	rbool operator !=( const RDOStudioFrameStyle& style ) const;

	virtual void init( CREF(tstring) _regPath = "" );
};

#endif // RDOSTUDIOFRAMESTYLE_H
