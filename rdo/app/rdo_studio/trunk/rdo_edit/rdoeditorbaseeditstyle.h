/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorbaseeditstyle.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDITSTYLE_H_
#define _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qcolor.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdobaseeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditor {

// --------------------------------------------------------------------------------
// -------------------- RDOEditorBaseEditTheme
// --------------------------------------------------------------------------------
class RDOEditorBaseEditTheme: public rdoEditCtrl::RDOBaseEditTheme
{
public:
	RDOEditorBaseEditTheme();
	virtual ~RDOEditorBaseEditTheme();

	RDOEditorBaseEditTheme& operator =( const RDOEditorBaseEditTheme& theme );
	rbool operator ==( const RDOEditorBaseEditTheme& theme ) const;
	rbool operator !=( const RDOEditorBaseEditTheme& theme ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	QColor identifierColor;
	QColor keywordColor;
	QColor functionsColor;
	QColor traceColor;
	QColor colorColor;
	QColor commentColor;
	QColor numberColor;
	QColor stringColor;
	QColor operatorColor;

	rdoStyle::RDOStyleFont::style identifierStyle;
	rdoStyle::RDOStyleFont::style keywordStyle;
	rdoStyle::RDOStyleFont::style functionsStyle;
	rdoStyle::RDOStyleFont::style traceStyle;
	rdoStyle::RDOStyleFont::style colorStyle;
	rdoStyle::RDOStyleFont::style commentStyle;
	rdoStyle::RDOStyleFont::style numberStyle;
	rdoStyle::RDOStyleFont::style stringStyle;
	rdoStyle::RDOStyleFont::style operatorStyle;

	virtual rbool styleDefault( const int styleType ) const;
	virtual rbool styleUsing( const int styleType ) const;
	virtual rbool styleBold( const int styleType = STYLE_DEFAULT ) const;
	virtual rbool styleItalic( const int styleType = STYLE_DEFAULT ) const;
	virtual tstring styleFGColorToHEX( const int styleType = STYLE_DEFAULT ) const;

	static RDOEditorBaseEditTheme getDefaultTheme();
	static RDOEditorBaseEditTheme getCppTheme();
	static RDOEditorBaseEditTheme getPascalTheme();
	static RDOEditorBaseEditTheme getHtmlTheme();
	static RDOEditorBaseEditTheme getClassicTheme();
	static RDOEditorBaseEditTheme getTwilightTheme();
	static RDOEditorBaseEditTheme getOceanTheme();
};

QSettings& operator<< (QSettings& settings, const RDOEditorBaseEditTheme& theme);
QSettings& operator>> (QSettings& settings,       RDOEditorBaseEditTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- RDOEditorBaseEditStyle
// --------------------------------------------------------------------------------
class RDOEditorBaseEditStyle: public rdoEditCtrl::RDOBaseEditStyle
{
protected:
	virtual void initTheme();

public:
	RDOEditorBaseEditStyle();
	virtual ~RDOEditorBaseEditStyle();

	RDOEditorBaseEditStyle& operator =( const RDOEditorBaseEditStyle& style );
	rbool operator ==( const RDOEditorBaseEditStyle& style ) const;
	rbool operator !=( const RDOEditorBaseEditStyle& style ) const;
};

} // namespace rdoEditor

#endif // _RDO_STUDIO_RDO_EDIT_RDOEDITORBASEEDITSTYLE_H_
