/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/chart/chart_preferences.h
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      19.02.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_CHART_PREFERENCES_H_
#define _RDO_STUDIO_CHART_PREFERENCES_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtWidgets/qdialog.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/projects/common/bin/rdo_studio/generated/ui_chart_preferences.h"
#include "app/rdo_studio/src/chart/chart_doc.h"
// --------------------------------------------------------------------------------

class ChartPreferences
	: public QDialog
	, private Ui::ChartPreferencesDialog
{
Q_OBJECT
public:
	explicit ChartPreferences(PTR(ChartView) pView = NULL);

private:
	int       m_ValCountX;
	int       m_ValCountY;
	QString   m_ChartTitle;
	int       m_AxisSerie;

	int       m_sizeMarker;

	PTR(ChartView) m_pView;

	void apply();

};
#endif // _RDO_STUDIO_VIEW_PREFERENCES_H_