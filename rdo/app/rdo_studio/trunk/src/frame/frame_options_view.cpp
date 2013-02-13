/*!
  \copyright (c) RDO-Team, 2003-2013
  \file      frame_options_view.cpp
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      07.02.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qboxlayout.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/frame/frame_options_view.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/style.h"
// --------------------------------------------------------------------------------
using namespace rdoStyle;

FrameOptionsView::FrameOptionsView(PTR(QWidget) pParent)
	: QWidget(pParent)
	, m_style(NULL)
{
	setupUi(this);
	englishSampleLabel->setText(QString::fromLocal8Bit("Sample of drawing text"));
	russianSampleLabel->setText(QString::fromLocal8Bit("Пример текста"));
}

void FrameOptionsView::setStyle(PTR(RDOStudioFrameStyle) style)
{
	m_style = style;

	QFont font(QString::fromStdString(m_style->font->name), m_style->font->size);

	font.setWeight((m_style->theme->defaultStyle & RDOStyleFont::BOLD) != 0 ? QFont::Bold : QFont::Normal);
	font.setItalic((m_style->theme->defaultStyle & RDOStyleFont::ITALIC) != 0 ? true : false);
	font.setUnderline((m_style->theme->defaultStyle & RDOStyleFont::UNDERLINE) != 0 ? true : false);

	PTR(QPalette) fontPalette = new QPalette();
	fontPalette->setColor(QPalette::WindowText, Qt::yellow);
	englishSampleLabel->setPalette(*fontPalette);
	russianSampleLabel->setPalette(*fontPalette);
	englishSampleLabel->setFont(font);
	russianSampleLabel->setFont(font);

	QColor fgColor(m_style->theme->defaultColor.blue(), m_style->theme->defaultColor.green(), m_style->theme->defaultColor.red());
	QColor bgColor(m_style->theme->backgroundColor.blue(), m_style->theme->backgroundColor.green(), m_style->theme->backgroundColor.red());
	
	pictureLabel->setStyleSheet("border: 1px solid "+fgColor.name()+";");

	PTR(QPalette) palette = new QPalette();
	palette->setColor(QPalette::Background, bgColor);
	setAutoFillBackground(true);
	setPalette(*palette);
}