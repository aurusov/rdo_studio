/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_preferences.h
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      19.02.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_TRACER_CHART_PREFERENCES_H_
#define _RDO_STUDIO_TRACER_CHART_PREFERENCES_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qcolordialog.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/projects/common/bin/rdo_studio/generated/ui_chart_preferences.h"
#include "app/rdo_studio/src/tracer/chart/chart_doc.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

class ChartPreferences
	: public QDialog
	, private Ui::ChartPreferencesDialog
{
Q_OBJECT
public:
	explicit ChartPreferences(PTR(ChartView) pView = NULL);

private:

	class ChartIntValidator : public QIntValidator
	{
		public:
			ChartIntValidator(int bottom, int top, PTR(QObject) pParent)
				: QIntValidator(bottom, top, pParent)
			{}

			QValidator::State validate(QString& input, int& pos) const
			{
				QIntValidator::State result(QIntValidator::validate(input, pos));
				if (result == QValidator::Intermediate) result = QValidator::Invalid;
				return result;
			}
	};

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

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_CHART_PREFERENCES_H_
