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
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/warning_disable.h"
#include <QStatusBar>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/status_bar.h"
// --------------------------------------------------------------------------------

StatusBar::StatusBar(QMainWindow* pParent)
	: m_pParent(pParent)
{
	m_pSBCoord         = new QLabel(m_pParent);
	m_pSBModify        = new QLabel(m_pParent);
	m_pSBOverwrite     = new QLabel(m_pParent);
	m_pSBModelTime     = new QLabel(m_pParent);
	m_pSBModelRuntype  = new QLabel(m_pParent);
	m_pSBModelSpeed    = new QLabel(m_pParent);
	m_pSBModelShowRate = new QLabel(m_pParent);

	m_pParent->statusBar()->addWidget(m_pSBCoord,         5);
	m_pParent->statusBar()->addWidget(m_pSBModify,        5);
	m_pParent->statusBar()->addWidget(m_pSBOverwrite,     3);
	m_pParent->statusBar()->addWidget(m_pSBModelTime,     5);
	m_pParent->statusBar()->addWidget(m_pSBModelRuntype,  7);
	m_pParent->statusBar()->addWidget(m_pSBModelSpeed,    5);
	m_pParent->statusBar()->addWidget(m_pSBModelShowRate, 7);

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

template <>
PTR(QLabel) StatusBar::getLabel<StatusBar::SB_COORD>(StatusBarType<SB_COORD>)
{
	return m_pSBCoord;
}

template <>
PTR(QLabel) StatusBar::getLabel<StatusBar::SB_MODIFY>(StatusBarType<SB_MODIFY>)
{
	return m_pSBModify;
}

template <>
PTR(QLabel) StatusBar::getLabel<StatusBar::SB_OVERWRITE>(StatusBarType<SB_OVERWRITE>)
{
	return m_pSBOverwrite;
}

template <>
PTR(QLabel) StatusBar::getLabel<StatusBar::SB_MODEL_TIME>(StatusBarType<SB_MODEL_TIME>)
{
	return m_pSBModelTime;
}

template <>
PTR(QLabel) StatusBar::getLabel<StatusBar::SB_MODEL_RUNTYPE>(StatusBarType<SB_MODEL_RUNTYPE>)
{
	return m_pSBModelRuntype;
}

template <>
PTR(QLabel) StatusBar::getLabel<StatusBar::SB_MODEL_SPEED>(StatusBarType<SB_MODEL_SPEED>)
{
	return m_pSBModelSpeed;
}

template <>
PTR(QLabel) StatusBar::getLabel<StatusBar::SB_MODEL_SHOWRATE>(StatusBarType<SB_MODEL_SHOWRATE>)
{
	return m_pSBModelShowRate;
}
