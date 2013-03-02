/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdologeditstyle.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_EDIT_CTRLS_RDOLOGEDITSTYLE_H_
#define _RDO_STUDIO_EDIT_CTRLS_RDOLOGEDITSTYLE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/edit_ctrls/rdobaseeditstyle.h"
// --------------------------------------------------------------------------------

namespace rdoEditCtrl {

// --------------------------------------------------------------------------------
// -------------------- LogEditTheme
// --------------------------------------------------------------------------------
class LogEditTheme: public EditBaseTheme
{
public:
	LogEditTheme();
	virtual ~LogEditTheme();

	LogEditTheme& operator =( const LogEditTheme& theme );
	rbool operator ==( const LogEditTheme& theme ) const;
	rbool operator !=( const LogEditTheme& theme ) const;

	virtual void load(QSettings& settings);
	virtual void save(QSettings& settings) const;

	QColor selectLineBgColor;

	static LogEditTheme getDefaultTheme();
	static LogEditTheme getClassicTheme();
	static LogEditTheme getTwilightTheme();
	static LogEditTheme getOceanTheme();
};

QSettings& operator<< (QSettings& settings, const LogEditTheme& theme);
QSettings& operator>> (QSettings& settings,       LogEditTheme& theme);

// --------------------------------------------------------------------------------
// -------------------- LogEditStyle
// --------------------------------------------------------------------------------
class LogEditStyle: public EditBaseStyle
{
public:
	LogEditStyle();
	virtual ~LogEditStyle();

	LogEditStyle& operator =( const LogEditStyle& style );
	rbool operator ==( const LogEditStyle& style ) const;
	rbool operator !=( const LogEditStyle& style ) const;
};

} // namespace rdoEditCtrl

#endif // _RDO_STUDIO_EDIT_CTRLS_RDOLOGEDITSTYLE_H_
