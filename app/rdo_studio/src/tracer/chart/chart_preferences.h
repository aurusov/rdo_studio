#ifndef _RDO_STUDIO_TRACER_CHART_PREFERENCES_H_
#define _RDO_STUDIO_TRACER_CHART_PREFERENCES_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QDialog>
#include <QColorDialog>
#include "ui_chart_preferences.h"
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_doc.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

class ChartPreferences
	: public QDialog
	, private Ui::ChartPreferencesDialog
{
Q_OBJECT
public:
	explicit ChartPreferences(ChartView* pView = NULL);

private:

	QColorDialog* colorDlg;

	int      m_valueCountX;
	int      m_valueCountY;
	QString  m_chartTitle;

	int      m_sizeMarker;

	int      traceIndex;

	ChartView*  m_pView;
	ChartSerie* m_pSerie;

	void insertColors(QComboBox* colorBox);
	void insertColor(const QColor& color, const QString& colorName, QComboBox* colorBox);
	void apply();

	void onHelpContext();
	void keyPressEvent(QKeyEvent* pEvent);

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
	void onTraceValue(int index);
	void onValueComboBox(int index);
	void onCheckAllData();
	void onCheckInput(const QString& text);
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_CHART_PREFERENCES_H_
