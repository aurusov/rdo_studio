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
#include "utils/warning_disable.h"
#include <QLabel>
#include <QBoxLayout>
#include "utils/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/frame/frame_options_view.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/style.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::frame;
using namespace rdo::gui::style;

OptionsView::OptionsView(PTR(QWidget) pParent)
	: QWidget(pParent)
	, m_style(NULL)
{
	setupUi(this);
	englishSampleLabel->setText("Sample of drawing text");
	russianSampleLabel->setText("Пример текста");
}

void OptionsView::setStyle(PTR(FrameStyle) style)
{
	m_style = style;

	QFont font(QString::fromStdString(m_style->font->name), m_style->font->size);

	font.setWeight((m_style->theme->defaultStyle & StyleFont::BOLD) != 0 ? QFont::Bold : QFont::Normal);
	font.setItalic((m_style->theme->defaultStyle & StyleFont::ITALIC) != 0 ? true : false);
	font.setUnderline((m_style->theme->defaultStyle & StyleFont::UNDERLINE) != 0 ? true : false);

	PTR(QPalette) fontPalette = new QPalette();
	fontPalette->setColor(QPalette::WindowText, Qt::yellow);
	englishSampleLabel->setPalette(*fontPalette);
	russianSampleLabel->setPalette(*fontPalette);
	englishSampleLabel->setFont(font);
	russianSampleLabel->setFont(font);

	pictureLabel->setStyleSheet("border: 1px solid "+m_style->theme->defaultColor.name()+";");

	PTR(QPalette) palette = new QPalette();
	palette->setColor(QPalette::Background, m_style->theme->backgroundColor);
	setAutoFillBackground(true);
	setPalette(*palette);
}