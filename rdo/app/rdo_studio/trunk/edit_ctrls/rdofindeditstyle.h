/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdofindeditstyle.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDIT_CTRLS_RDOFINDEDITSTYLE_H_
#define _RDO_STUDIO_EDIT_CTRLS_RDOFINDEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdologeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOFindEditTheme
// --------------------------------------------------------------------------------
class RDOFindEditTheme: public LogEditTheme
{
public:
	RDOFindEditTheme();
	virtual ~RDOFindEditTheme();

	RDOFindEditTheme& operator =( const RDOFindEditTheme& theme );
	rbool operator ==( const RDOFindEditTheme& theme ) const;
	rbool operator !=( const RDOFindEditTheme& theme ) const;

	virtual void load( tstring regPath );
	virtual void save( tstring regPath ) const;

	COLORREF keywordColor;

	rdoStyle::RDOStyleFont::style keywordStyle;

	virtual rbool styleDefault( const int styleType ) const;
	virtual rbool styleUsing( const int styleType ) const;
	virtual rbool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual rbool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static RDOFindEditTheme getDefaultTheme();
	static RDOFindEditTheme getClassicTheme();
	static RDOFindEditTheme getTwilightTheme();
	static RDOFindEditTheme getOceanTheme();
};

// --------------------------------------------------------------------------------
// -------------------- RDOFindEditStyle
// --------------------------------------------------------------------------------
class RDOFindEditStyle: public LogEditStyle
{
protected:
	virtual void initTheme();

public:
	RDOFindEditStyle();
	virtual ~RDOFindEditStyle();

	RDOFindEditStyle& operator =( const RDOFindEditStyle& style );
	rbool operator ==( const RDOFindEditStyle& style ) const;
	rbool operator !=( const RDOFindEditStyle& style ) const;
};

} // namespace rdoEditCtrl

#endif // _RDO_STUDIO_EDIT_CTRLS_RDOFINDEDITSTYLE_H_
