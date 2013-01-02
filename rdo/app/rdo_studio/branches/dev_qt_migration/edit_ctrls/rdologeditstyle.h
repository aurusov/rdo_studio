/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologeditstyle.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_CTRLS_RDOLOGEDITSTYLE_H_
#define _RDO_STUDIO_MFC_EDIT_CTRLS_RDOLOGEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdobaseeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOLogEditTheme
// --------------------------------------------------------------------------------
class RDOLogEditTheme: public RDOBaseEditTheme
{
public:
	RDOLogEditTheme();
	virtual ~RDOLogEditTheme();

	RDOLogEditTheme& operator =( const RDOLogEditTheme& theme );
	rbool operator ==( const RDOLogEditTheme& theme ) const;
	rbool operator !=( const RDOLogEditTheme& theme ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	COLORREF selectLineBgColor;

	static RDOLogEditTheme getDefaultTheme();
	static RDOLogEditTheme getClassicTheme();
	static RDOLogEditTheme getTwilightTheme();
	static RDOLogEditTheme getOceanTheme();
};

// --------------------------------------------------------------------------------
// -------------------- RDOLogEditStyle
// --------------------------------------------------------------------------------
class RDOLogEditStyle: public RDOBaseEditStyle
{
protected:
	virtual void initTheme();

public:
	RDOLogEditStyle();
	virtual ~RDOLogEditStyle();

	RDOLogEditStyle& operator =( const RDOLogEditStyle& style );
	rbool operator ==( const RDOLogEditStyle& style ) const;
	rbool operator !=( const RDOLogEditStyle& style ) const;
};

} // namespace rdoEditCtrl

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDOLOGEDITSTYLE_H_
