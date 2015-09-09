// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/frame_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QLabel>
#include <QBoxLayout>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/frame/frame_options_view.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/style.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::frame;
using namespace rdo::gui::style;

OptionsView::OptionsView(QWidget* pParent)
    : QWidget(pParent)
    , m_style(NULL)
{
    setupUi(this);
    englishSampleLabel->setText("Sample of drawing text");
    russianSampleLabel->setText("Пример текста");
}

void OptionsView::setStyle(FrameStyle* style)
{
    m_style = style;

    QFont font(QString::fromStdString(m_style->font.name), m_style->font.size);

    font.setWeight((static_cast<int>(m_style->defaultStyle) & static_cast<int>(StyleFont::Style::BOLD)) != 0 ? QFont::Bold : QFont::Normal);
    font.setItalic((static_cast<int>(m_style->defaultStyle) & static_cast<int>(StyleFont::Style::ITALIC)) != 0 ? true : false);
    font.setUnderline((static_cast<int>(m_style->defaultStyle) & static_cast<int>(StyleFont::Style::UNDERLINE)) != 0 ? true : false);

    QPalette* fontPalette = new QPalette();
    fontPalette->setColor(QPalette::WindowText, Qt::yellow);
    englishSampleLabel->setPalette(*fontPalette);
    russianSampleLabel->setPalette(*fontPalette);
    englishSampleLabel->setFont(font);
    russianSampleLabel->setFont(font);

    pictureLabel->setStyleSheet("border: 1px solid "+m_style->defaultColor.name()+";");

    QPalette* palette = new QPalette();
    palette->setColor(QPalette::Background, m_style->backgroundColor);
    setAutoFillBackground(true);
    setPalette(*palette);
}