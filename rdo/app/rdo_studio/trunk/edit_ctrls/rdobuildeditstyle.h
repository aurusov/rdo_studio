/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdobuildeditstyle.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      25.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBUILDEDITSTYLE_H_
#define _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBUILDEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdologeditstyle.h"
// --------------------------------------------------------------------------------

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

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDOBUILDEDITSTYLE_H_
