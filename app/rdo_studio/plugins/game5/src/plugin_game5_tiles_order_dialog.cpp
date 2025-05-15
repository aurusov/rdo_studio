// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <vector>
#include "utils/src/common/warning_enable.h"
#include <QRegExpValidator>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/plugin_game5_tiles_order_dialog.h"
// --------------------------------------------------------------------------------

TilesOrderDialog::TilesOrderDialog(QWidget* pParent, const std::vector<unsigned int>& state)
    : QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
    setupUi(this);

    QString boardStateStr;
    for (auto index : state)
    {
        boardStateStr += QString::number(index) + " ";
    }

    QRegExpValidator * validator = new QRegExpValidator();
    validator->setRegExp(QRegExp(validatorRegExpPattern(state.size())));

    lineEditPosition->setValidator(validator);
    lineEditPosition->setText(boardStateStr);

    connect(buttonCancel, &QPushButton::clicked, this, &TilesOrderDialog::reject);
    connect(buttonOk    , &QPushButton::clicked, this, &TilesOrderDialog::onOkClick);
    
    if (pParent)
    {
        move(pParent->frameGeometry().center() - frameGeometry().center());
    }
}

TilesOrderDialog::~TilesOrderDialog()
{
}

void TilesOrderDialog::onOkClick()
{
    QStringList positionList = lineEditPosition->text().split(' ', QString::SkipEmptyParts);
    std::vector<unsigned int> newState;
    for (const auto& position: positionList)
    {
        newState.push_back(position.toInt());
    }
    emit tilesOrderCommited(newState);
    accept();
}

QString TilesOrderDialog::validatorRegExpPattern(int value) const
{
    QString singleRegExp;
    int leastBit = value % 10 - 1;
    int highBit  = value / 10;
    if (highBit == 0)
    {
        singleRegExp += "[0-" + QString::number(leastBit) + "]";
    }
    else
    {
        if (highBit == 1)
        {
            singleRegExp += "[0-9]|[0-1][0-" + QString::number(leastBit) + "]";
        }
        else
        {
            singleRegExp += "[0-9]|[0-" + QString::number(highBit - 1) + "][0-9]|[0-" + QString::number(highBit) + "][0-" + QString::number(leastBit) + "]";
        }
    }

    QString regExpString = "(((" + singleRegExp + ")(\\s)+)|((\\s)+(" + singleRegExp + ")(\\s)+)){" + QString::number(value) + "}($|((" + singleRegExp + ")$))";
    return regExpString;
}
