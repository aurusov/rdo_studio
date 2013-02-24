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
#include <QtWidgets/qcolordialog.h>
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
	PTR(QColorDialog) colorDlg;

	int       m_valueCountX;
	int       m_valueCountY;
	QString   m_chartTitle;

	int       m_sizeMarker;

	PTR(ChartView)  m_pView;
	PTR(ChartSerie) m_pSerie;

	void ChartPreferences::insertColors(QComboBox* colorBox);
	void ChartPreferences::insertColor(const QColor& color, const QString& colorName, QComboBox* colorBox);
	void apply();

private slots:
	void onOkButton();
	void onCancelButton();
	void onApplyButton();
	void onXValue(const QString& text);
	void onYValue(const QString& text);
	void onTitle(const QString& text);
	void onMarkerSize(const QString& text);
	void onColorDialog();
	void onColorSelected(const QColor& color);
	void onValueComboBox(int index);
	void onCheckAllData();

};
#endif // _RDO_STUDIO_VIEW_PREFERENCES_H_