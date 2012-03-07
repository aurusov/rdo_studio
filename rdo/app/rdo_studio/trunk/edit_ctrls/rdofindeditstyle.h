/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdofindeditstyle.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_EDIT_CTRLS_RDOFINDEDITSTYLE_H_
#define _RDO_STUDIO_MFC_EDIT_CTRLS_RDOFINDEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/edit_ctrls/rdologeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- RDOFindEditTheme
// --------------------------------------------------------------------------------
class RDOFindEditTheme: public RDOLogEditTheme
{
public:
	RDOFindEditTheme();
	virtual ~RDOFindEditTheme();

	RDOFindEditTheme& operator =( const RDOFindEditTheme& theme );
	bool operator ==( const RDOFindEditTheme& theme ) const;
	bool operator !=( const RDOFindEditTheme& theme ) const;

	virtual void load( std::string regPath );
	virtual void save( std::string regPath ) const;

	COLORREF keywordColor;

	rdoStyle::RDOStyleFont::style keywordStyle;

	virtual bool styleDefault( const int styleType ) const;
	virtual bool styleUsing( const int styleType ) const;
	virtual bool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual bool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual std::string styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static RDOFindEditTheme getDefaultTheme();
	static RDOFindEditTheme getClassicTheme();
	static RDOFindEditTheme getTwilightTheme();
	static RDOFindEditTheme getOceanTheme();
};

// --------------------------------------------------------------------------------
// -------------------- RDOFindEditStyle
// --------------------------------------------------------------------------------
class RDOFindEditStyle: public RDOLogEditStyle
{
protected:
	virtual void initTheme();

public:
	RDOFindEditStyle();
	virtual ~RDOFindEditStyle();

	RDOFindEditStyle& operator =( const RDOFindEditStyle& style );
	bool operator ==( const RDOFindEditStyle& style ) const;
	bool operator !=( const RDOFindEditStyle& style ) const;
};

}; // namespace rdoEditCtrl

#endif // _RDO_STUDIO_MFC_EDIT_CTRLS_RDOFINDEDITSTYLE_H_
