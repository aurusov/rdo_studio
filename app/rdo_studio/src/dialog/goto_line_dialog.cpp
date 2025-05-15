// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/application_pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <QIntValidator>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dialog/goto_line_dialog.h"
// --------------------------------------------------------------------------------

GoToLineDialog::GoToLineDialog(QWidget* pParent, int line, int lineCount)
    : QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
    , m_line (line   )
{
    setupUi(this);

    layout()->setSizeConstraint(QLayout::SetFixedSize);

    label->setText(QString("Номер строки (1-%1):").arg(lineCount));

    lineEdit->setValidator(new QIntValidator(1, lineCount, this));
    lineEdit->setText(QString::number(m_line));
    lineEdit->setFocus();
    lineEdit->selectAll();

    connect(buttonOk, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
    connect(lineEdit, SIGNAL(textEdited(const QString&)), this, SLOT(onCheckInput(const QString&)));
}

void GoToLineDialog::onOkButtonClicked()
{
    m_line = lineEdit->text().toInt();
    done(Accepted);
}

void GoToLineDialog::onCheckInput(const QString& /*text*/)
{
    buttonOk->setEnabled(lineEdit->text().toInt() >= 1 ? true : false);
}

int GoToLineDialog::getLine() const
{
    return m_line;
}
