/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      chart_preferences.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      19.02.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/chart/chart_preferences.h"
#include "app/rdo_studio/src/tracer/chart/chart_view.h"
#include "app/rdo_studio/src/tracer/chart/chart_doc.h"
#include "app/rdo_studio/src/application.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::tracer;

ChartPreferences::ChartPreferences(PTR(ChartView) pView)
	: QDialog(pView, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
	, m_pView(pView)
	, m_pSerie(NULL)
{
	setupUi(this);

	m_valueCountX = m_pView->getValueCountX();
	m_valueCountY = m_pView->getValueCountY();
	m_chartTitle  = m_pView->getDocument()->getTitle();

	titleLineEdit->setText(m_chartTitle);

	yValueLineEdit->setValidator(new QIntValidator(2, 100, this));
	yValueLineEdit->setText(QString::number(m_valueCountY));
	xValueLineEdit->setValidator(new QIntValidator(2, 100, this));
	xValueLineEdit->setText(QString::number(m_valueCountX));
	
	showLegendCheckBox->setChecked(m_pView->isDrawLegend());

	ChartDoc* doc = m_pView->getDocument();

	BOOST_FOREACH(const ChartSerie* const pSerie, doc->getSerieList())
	{
		yTraceComboBox->addItem(pSerie->getSerie()->getTitle());
		valueComboBox->addItem(pSerie->getSerie()->getTitle());
	}
	
	markerSizeLineEdit->setValidator(new QIntValidator(2, 6, markerSizeLineEdit));

	insertColors(colorComboBox);

	markerComboBox->addItem("Нет",         Serie::M_NONE);
	markerComboBox->addItem("Круг",        Serie::M_CIRCLE);
	markerComboBox->addItem("Квадрат",     Serie::M_SQUARE);
	markerComboBox->addItem("Треугольник", Serie::M_TRIANG);
	markerComboBox->addItem("Крестик",     Serie::M_CROSS);

	connect(xValueLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onXValue(const QString&)));
	connect(yValueLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onYValue(const QString&)));
	connect(titleLineEdit,  SIGNAL(textEdited(const QString&)), this, SLOT(onTitle (const QString&)));

	connect(markerSizeLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onMarkerSize(const QString&)));	

	connect(colorToolButton, SIGNAL(clicked()), this, SLOT(onColorDialog()));
	connect(valueComboBox,   SIGNAL(activated(int)), this, SLOT(onValueComboBox(int)));

	connect(okButton,     SIGNAL(clicked()), this, SLOT(onOkButton()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(onCancelButton()));
	connect(applyButton,  SIGNAL(clicked()), this, SLOT(onApplyButton()));

	onValueComboBox(valueComboBox->currentIndex());

	connect(xValueLineEdit,            SIGNAL(textEdited(const QString&)), this, SLOT(onCheckAllData()));
	connect(yValueLineEdit,            SIGNAL(textEdited(const QString&)), this, SLOT(onCheckAllData()));
	connect(titleLineEdit,             SIGNAL(textEdited(const QString&)), this, SLOT(onCheckAllData()));
	connect(markerSizeLineEdit,        SIGNAL(textEdited(const QString&)), this, SLOT(onCheckAllData()));
	connect(showLegendCheckBox,        SIGNAL(stateChanged(int)), this, SLOT(onCheckAllData()));
	connect(showInLegendCheckBox,      SIGNAL(stateChanged(int)), this, SLOT(onCheckAllData()));
	connect(showMarkerCheckBox,        SIGNAL(stateChanged(int)), this, SLOT(onCheckAllData()));
	connect(transparentMarkerCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckAllData()));
	connect(markerComboBox,            SIGNAL(activated(int)), this, SLOT(onCheckAllData()));
	connect(titleValueLineEdit,        SIGNAL(textEdited(const QString&)), this, SLOT(onCheckAllData()));
	connect(colorComboBox,             SIGNAL(activated(int)), this, SLOT(onCheckAllData()));

	connect(xValueLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onCheckInput(const QString&)));
	connect(yValueLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onCheckInput(const QString&)));
	connect(markerSizeLineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onCheckInput(const QString&)));

	applyButton->setEnabled(false);
}

void ChartPreferences::insertColors(QComboBox* colorBox)
{
	insertColor(QColor(0x00, 0x80, 0x00), "Green",   colorBox);
	insertColor(QColor(0x00, 0x00, 0x80), "Navy",    colorBox);
	insertColor(QColor(0x80, 0x80, 0x80), "Grey",    colorBox);
	insertColor(QColor(0x80, 0x00, 0x80), "Purple",  colorBox);
	insertColor(QColor(0xFF, 0x00, 0x00), "Red",     colorBox);
	insertColor(QColor(0x00, 0xFF, 0x00), "Lime",    colorBox);
	insertColor(QColor(0x00, 0x00, 0x00), "Black",   colorBox);
	insertColor(QColor(0xFF, 0xFF, 0xFF), "White",   colorBox);
	insertColor(QColor(0x80, 0x80, 0x00), "Olive",   colorBox);
	insertColor(QColor(0xC0, 0xC0, 0xC0), "Silver",  colorBox);
	insertColor(QColor(0x80, 0x00, 0x00), "Maroon",  colorBox);
	insertColor(QColor(0x00, 0x80, 0x80), "Teal",    colorBox);
	insertColor(QColor(0xFF, 0xFF, 0x00), "Yellow",  colorBox);
	insertColor(QColor(0x00, 0x00, 0xFF), "Blue",    colorBox);
	insertColor(QColor(0xFF, 0x00, 0xFF), "Fushcia", colorBox);
	insertColor(QColor(0x00, 0xFF, 0xFF), "Aqua",    colorBox);
}

void ChartPreferences::insertColor(const QColor& color, const QString& colorName, QComboBox* colorBox)
{
	colorBox->addItem(colorName);
	int size = colorBox ->style()->pixelMetric(QStyle::PM_SmallIconSize);
	QPixmap pixmap(size,size-5);
	pixmap.fill(color);
	QRect rBorder(0,0,size-1,size-6);
	QPainter p(&pixmap);
	QPen pen(Qt::black, 1, Qt::SolidLine);
	p.setPen(pen);
	p.drawRect(rBorder);
	colorBox->setItemData(colorBox->findText(colorName), color, Qt::UserRole);
	colorBox->setItemIcon(colorBox->findText(colorName), QIcon(pixmap));
}

void ChartPreferences::apply()
{
	m_pView->setYAxis(valueComboBox->currentIndex() != -1
		? m_pView->getDocument()->getSerieList().at(valueComboBox->currentIndex())
		: NULL
	);
	m_pView->setDrawLegend(showLegendCheckBox->checkState() == Qt::Checked ? true : false);
	m_pView->setValueCountX(m_valueCountX);
	m_pView->setValueCountY(m_valueCountY);
	m_pView->getDocument()->setTitle(m_chartTitle);

	if (m_pSerie)
	{
		QString title = titleValueLineEdit->text();

		ChartSerie::Options options;
		options.title = title;
		options.color = colorComboBox->itemData(colorComboBox->currentIndex(), Qt::UserRole).value<QColor>();
		options.markerType = static_cast<Serie::Marker>(markerComboBox->itemData(markerComboBox->currentIndex(), Qt::UserRole).toInt());
		options.markerSize = m_sizeMarker;
		options.markerNeedDraw = showMarkerCheckBox->checkState() == Qt::Checked ? true : false;
		options.markerTransparent = transparentMarkerCheckBox->checkState() == Qt::Checked ? true : false;
		options.showInLegend = showInLegendCheckBox->checkState() == Qt::Checked ? true : false;

		m_pSerie->setOptions(options);
	}

	m_pView->repaint();
}

void ChartPreferences::onHelpContext()
{
	QByteArray ba;
	switch(tabWidget->currentIndex())
	{
	case 0:
		ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_model/work_model_chart.htm#chart\n");
		break;
	case 1:
		ba.append("setSource qthelp://studio/doc/rdo_studio_rus/html/work_model/work_model_chart.htm#series\n");
		break;
	}
	g_pApp->callQtAssistant(ba);

}

void ChartPreferences::onCheckAllData()
{
	rbool legend = showLegendCheckBox->checkState() == Qt::Checked ? true : false;
	rbool showInLegend = showInLegendCheckBox->checkState() == Qt::Checked ? true : false;
	rbool showMarker = showMarkerCheckBox->checkState() == Qt::Checked ? true : false;
	rbool transparentMarker = transparentMarkerCheckBox->checkState() == Qt::Checked ? true : false;

	rbool titleFlag  = titleLineEdit->text() == m_pView->getDocument()->getTitle() ? true : false;
	rbool yValueFlag = yValueLineEdit->text().toInt() == m_pView->getValueCountX() ? true : false;
	rbool xValueFlag = xValueLineEdit->text().toInt() == m_pView->getValueCountY() ? true : false;
	rbool showLegendFlag = legend == m_pView->isDrawLegend();
	rbool showInLegendFlag = showInLegend == m_pSerie->options().showInLegend ? true : false;
	rbool markerSizeFlag = markerSizeLineEdit->text().toInt() == m_pSerie->options().markerSize ? true : false;
	rbool markerShowFlag = showMarker == m_pSerie->options().markerNeedDraw ? true : false;
	rbool markerTransparentFlag = transparentMarker == m_pSerie->options().markerTransparent ? true : false;
	rbool titleValueFlag = titleValueLineEdit->text() == m_pSerie->options().title;
	rbool colorFlag = colorComboBox->itemData(colorComboBox->currentIndex(), Qt::UserRole).value<QColor>() == m_pSerie->options().color ? true : false;
	rbool markerTypeFlag = markerComboBox->itemData(markerComboBox->currentIndex(), Qt::UserRole).toInt() == m_pSerie->options().markerType ? true : false;

	if (titleFlag
		&& yValueFlag
		&& xValueFlag
		&& showLegendFlag
		&& showInLegendFlag
		&& markerSizeFlag
		&& markerShowFlag
		&& markerTransparentFlag
		&& titleValueFlag
		&& colorFlag
		&& markerTypeFlag)
		applyButton->setEnabled(false);
	else
		applyButton->setEnabled(true);
}

void ChartPreferences::onOkButton()
{
	apply();
	done(Accepted);
}

void ChartPreferences::onCancelButton()
{
	done(Rejected);
}

void ChartPreferences::onApplyButton()
{
	apply();
	applyButton->setEnabled(false);
}

void ChartPreferences::onCheckInput(const QString& text)
{
	UNUSED(text);

	rbool check = yValueLineEdit->text().toInt() >= 2 &&
		xValueLineEdit->text().toInt() >= 2 &&
		markerSizeLineEdit->text().toInt() >= 2;

	okButton->setEnabled(check);
	applyButton->setEnabled(check);
}

void ChartPreferences::onXValue(const QString& text)
{
	m_valueCountX = text.toInt();
}

void ChartPreferences::onYValue(const QString& text)
{
	m_valueCountY = text.toInt();
}

void ChartPreferences::onTitle(const QString& text)
{
	m_chartTitle = text;
}

void ChartPreferences::onMarkerSize(const QString& text)
{
	m_sizeMarker = text.toInt();
}

void ChartPreferences::onColorDialog()
{
	QColor color = colorComboBox->itemData(colorComboBox->currentIndex(), Qt::UserRole).value<QColor>();
	colorDlg = new QColorDialog(color, this);
	connect(colorDlg, SIGNAL(colorSelected(const QColor&)), this, SLOT(onColorSelected(const QColor&)));
	colorDlg->exec();
}

void ChartPreferences::onColorSelected(const QColor& color)
{
	if (colorComboBox->findData(color, Qt::UserRole) == -1)
	{
		insertColor(color, QString("[%1, %2, %3]").arg(color.red()).arg(color.green()).arg(color.blue()), colorComboBox);
	}
	colorComboBox->setCurrentIndex(colorComboBox->findData(color, Qt::UserRole));
	onCheckAllData();
}

void ChartPreferences::onValueComboBox(int index)
{
	m_pSerie = m_pView->getDocument()->getSerieList().at(index);

	m_sizeMarker = m_pSerie->options().markerSize;
	titleValueLineEdit->setText(m_pSerie->options().title);
	showMarkerCheckBox->setChecked(m_pSerie->options().markerNeedDraw);
	transparentMarkerCheckBox->setChecked(m_pSerie->options().markerTransparent);
	showInLegendCheckBox->setChecked(m_pSerie->options().showInLegend);
	markerSizeLineEdit->setText(QString::number(m_pSerie->options().markerSize));
	markerComboBox->setCurrentIndex(markerComboBox->findData(m_pSerie->options().markerType));
	if (colorComboBox->findData(m_pSerie->options().color, Qt::UserRole) == -1)
	{
		insertColor(m_pSerie->options().color, QString("[%1, %2, %3]").arg(m_pSerie->options().color.red()).arg(m_pSerie->options().color.green()).arg(m_pSerie->options().color.blue()), colorComboBox);
	}
	colorComboBox->setCurrentIndex(colorComboBox->findData(m_pSerie->options().color, Qt::UserRole));
}

void ChartPreferences::keyPressEvent(QKeyEvent* pEvent)
{
	if(QKeySequence(pEvent->key()) == QKeySequence::HelpContents)
		onHelpContext();
}