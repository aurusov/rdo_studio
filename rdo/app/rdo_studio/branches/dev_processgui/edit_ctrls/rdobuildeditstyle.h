#ifndef RDOBUILDEDITSTYLE_H
#define RDOBUILDEDITSTYLE_H
#pragma once

#include "app/rdo_studio_mfc/edit_ctrls/rdologeditstyle.h"

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOBuildEditTheme
// --------------------------------------------------------------------------------
class RDOBuildEditTheme: public RDOLogEditTheme
{
public:
	RDOBuildEditTheme();
	virtual ~RDOBuildEditTheme();

	RDOBuildEditTheme& operator =( const RDOBuildEditTheme& theme );
	bool operator ==( const RDOBuildEditTheme& theme ) const;
	bool operator !=( const RDOBuildEditTheme& theme ) const;

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	bool warning;

	static RDOBuildEditTheme getDefaultTheme();
	static RDOBuildEditTheme getClassicTheme();
	static RDOBuildEditTheme getTwilightTheme();
	static RDOBuildEditTheme getOceanTheme();
};

// --------------------------------------------------------------------------------
// -------------------- RDOLogEditStyle
// --------------------------------------------------------------------------------
class RDOBuildEditStyle: public RDOLogEditStyle
{
protected:
	virtual void initTheme();

public:
	RDOBuildEditStyle();
	virtual ~RDOBuildEditStyle();

	RDOBuildEditStyle& operator =( const RDOBuildEditStyle& style );
	bool operator ==( const RDOBuildEditStyle& style ) const;
	bool operator !=( const RDOBuildEditStyle& style ) const;
};

}; // namespace rdoEditCtrl

//{{AFX_INSERT_LOCATION}}

#endif // RDOBUILDEDITSTYLE_H
