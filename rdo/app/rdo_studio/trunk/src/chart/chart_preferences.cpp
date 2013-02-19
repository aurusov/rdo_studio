/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/chart/chart_preferences.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      19.02.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/chart/chart_preferences.h"
#include "app/rdo_studio/src/chart/chart_view.h"
#include "app/rdo_studio/src/chart/chart_doc.h"
// --------------------------------------------------------------------------------

ChartPreferences::ChartPreferences(PTR(ChartView) m_pView)
	: QDialog(m_pView)
{
	setupUi(this);

	m_ValCountX  = m_pView->m_valueCountX;
	m_ValCountY  = m_pView->m_valueCountY;
	m_ChartTitle = QString::fromLocal8Bit(m_pView->getDocument()->getTitle().c_str());
	m_AxisSerie  = -1;

	m_sizeMarker = 2;

	titleLineEdit->setText(m_ChartTitle);
	yValueLineEdit->setValidator(new QIntValidator(1, 100, this));
	yValueLineEdit->setText(QString::number(m_ValCountY));
}