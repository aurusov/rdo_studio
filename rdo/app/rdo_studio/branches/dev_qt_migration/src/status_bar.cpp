/*!
  \copyright (c) RDO-Team, 2003-2013
  \file      status_bar.cpp
  \authors   Захаров Павел
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.04.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qstatusbar.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/status_bar.h"
// --------------------------------------------------------------------------------

StatusBar::StatusBar(QMainWindow* pParent)
	: m_pParent(pParent)
{
	m_pProgressBar = new QProgressBar(m_pParent);
	m_pProgressBar->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	m_pProgressBar->setMaximumSize(QWIDGETSIZE_MAX, 15);
	m_pProgressBar->setVisible(false);

	m_pProgressBarFakeWidget = new QWidget(m_pParent);
	m_pProgressBarFakeWidget->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	m_pProgressBarFakeWidget->setMaximumSize(QWIDGETSIZE_MAX, 15);
	m_pProgressBarFakeWidget->setVisible(true);

	m_pParent->statusBar()->addWidget(m_pProgressBarFakeWidget, 7);
}

StatusBar::~StatusBar()
{}

void StatusBar::beginProgress(rsint lower, rsint upper)
{
	m_pProgressBar->setRange(lower, upper);
	m_pProgressBar->setValue(lower);
	m_pProgressBar->setVisible(true);

	m_pParent->statusBar()->removeWidget(m_pProgressBarFakeWidget);
	m_pParent->statusBar()->addWidget(m_pProgressBar, 7);

	m_pProgressBarFakeWidget->setVisible(false);
}

void StatusBar::stepProgress()
{
	m_pProgressBar->setValue(m_pProgressBar->value() + 1);
}

void StatusBar::endProgress()
{
	m_pProgressBarFakeWidget->setVisible(true);

	m_pParent->statusBar()->removeWidget(m_pProgressBar);
	m_pParent->statusBar()->addWidget(m_pProgressBarFakeWidget, 7);

	m_pProgressBar->setVisible(false);
}
