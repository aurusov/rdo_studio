// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QMessageBox>
#include <QDir>
#include <QCompleter>
#include <QStringListModel>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_dialog.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_model_generator.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_tiles_order_dialog.h"
#include "app/rdo_studio/plugins/game5/src/multi_select_completer.h"
// --------------------------------------------------------------------------------

PluginGame5GenerateSituationDialog::PluginGame5GenerateSituationDialog(QWidget* pParent)
    : QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
{
    setupUi(this);

    MultiSelectCompleter* completer = new MultiSelectCompleter(QStringList(), this);
    lineEditCustom->setCompleter(completer);
    connect(lineEditCustom, &QLineEdit::selectionChanged, completer, &MultiSelectCompleter::onLineEditTextChanged);

    adjustSize();
    hiddenWidget->setFixedWidth(hiddenWidget->width());
    hiddenWidget->hide();

    buttonHide->setCheckable(true);
    buttonHide->setDefault(false);

    adjustSize();
    setFixedSize(width(), height());
    setSizeGripEnabled(false);

    rdo::gui::model::Model* pModel = getCurrentModel();
    clearAllTabs();
    pModel->getTab()->getItemEdit(rdo::FileType::FUN)->clearAll();
    pModel->getTab()->getItemEdit(rdo::FileType::FUN)->appendText(modelFUN());

    connect(buttonHide        , &QPushButton::toggled, this, &PluginGame5GenerateSituationDialog::onClickHide         );
    connect(buttonSetLineup   , &QPushButton::clicked, this, &PluginGame5GenerateSituationDialog::callTilesOrderDialog);
    connect(buttonRandomLineup, &QPushButton::clicked, this, &PluginGame5GenerateSituationDialog::emitSolvabilityCheck);
    connect(buttonOk          , &QPushButton::clicked, this, &PluginGame5GenerateSituationDialog::onClickOk           );

    connect(this     , &PluginGame5GenerateSituationDialog::buttonRandomClicked,
            gameBoard, &Board::buildRandomOrder);
    connect(buttonRightLineup, &QPushButton::clicked,
            gameBoard        , &Board::buildCorrectOrder);

    if (pParent)
    {
        move(pParent->frameGeometry().center() - frameGeometry().center());
    }
}

PluginGame5GenerateSituationDialog::~PluginGame5GenerateSituationDialog()
{}

void PluginGame5GenerateSituationDialog::onClickHide(bool state)
{
    hiddenWidget->setVisible(state);
    setFixedWidth(QWIDGETSIZE_MAX);
    adjustSize();
    setFixedWidth(width());
}

void PluginGame5GenerateSituationDialog::onClickOk()
{
    rdo::gui::model::Model* pModel = getCurrentModel();
    generateModel();
    pModel->runModel();
    done(Accepted);
}

void PluginGame5GenerateSituationDialog::emitSolvabilityCheck()
{
    emit buttonRandomClicked(solvabilityCheck->isChecked());
}

std::string PluginGame5GenerateSituationDialog::evaluateBy() const
{
    if (radioButton0->isChecked())
    {
        return "0";
    }
    if (radioButtonQuantity->isChecked())
    {
        return "Кол_во_фишек_не_на_месте()";
    }
    if (radioButtonDistance->isChecked())
    {
        return "Расстояния_фишек_до_мест()";
    }
    else //if (radioButtonCustom->isChecked())
    {
        return lineEditCustom->text().toStdString();
    }
}

std::string PluginGame5GenerateSituationDialog::activityValue(MoveDirection direction) const
{
    QString costValue;
    QString calcSwitcher;
    switch (direction)
    {
        case MoveDirection::DOWN:
            costValue    = moveDownCost->getLineEdit().text();
            calcSwitcher = moveDownCalcSwitcher->currentText();
            break;
        case MoveDirection::LEFT:
            costValue    = moveLeftCost->getLineEdit().text();
            calcSwitcher = moveLeftCalcSwitcher->currentText();
            break;
        case MoveDirection::RIGHT:
            costValue    = moveRightCost->getLineEdit().text();
            calcSwitcher = moveRightCalcSwitcher->currentText();
            break;
        case MoveDirection::UP:
            costValue    = moveUpCost->getLineEdit().text();
            calcSwitcher = moveUpCalcSwitcher->currentText();
            break;
        default:
            break;
    }
    const QString string = calcSwitcher + " " + costValue;
    return string.toStdString();
}

QString PluginGame5GenerateSituationDialog::modelRTP() const
{
    return PluginGame5ModelGenerator::modelRTP(*gameBoard);
}

QString PluginGame5GenerateSituationDialog::modelRSS() const
{
    return PluginGame5ModelGenerator::modelRSS(*gameBoard);
}

QString PluginGame5GenerateSituationDialog::modelPAT() const
{
    return PluginGame5ModelGenerator::modelPAT();
}

QString PluginGame5GenerateSituationDialog::modelDPT() const
{
    return PluginGame5ModelGenerator::modelDPT(
            *gameBoard,
            evaluateBy(),
            checkBoxCopareTop->isChecked(),
            activityValue(MoveDirection::LEFT),
            activityValue(MoveDirection::RIGHT),
            activityValue(MoveDirection::DOWN),
            activityValue(MoveDirection::UP));
}

QString PluginGame5GenerateSituationDialog::modelFUN() const
{
    return PluginGame5ModelGenerator::modelFUN(*gameBoard);
}

void PluginGame5GenerateSituationDialog::clearAllTabs() const
{
    rdo::gui::model::Model* pModel = getCurrentModel();
    for (int i = 0; i < pModel->getTab()->tabBar()->count(); i++)
    {
        if (i != (int)rdo::FileType::FUN)
        {
            pModel->getTab()->getItemEdit(i)->clearAll();
        }
    }
}

void PluginGame5GenerateSituationDialog::callTilesOrderDialog()
{
    TilesOrderDialog dlg(this, gameBoard->getBoardState());
    connect(&dlg, &TilesOrderDialog::tilesOrderCommited, gameBoard, &Board::setTilesPositon);
    dlg.exec();
}

void PluginGame5GenerateSituationDialog::generateModel() const
{
    rdo::gui::model::Model* pModel = getCurrentModel();
    if (pModel->getTab())
    {
        clearAllTabs();

        pModel->getTab()->getItemEdit(rdo::FileType::RTP)->appendText(modelRTP());
        pModel->getTab()->getItemEdit(rdo::FileType::RSS)->appendText(modelRSS());
        pModel->getTab()->getItemEdit(rdo::FileType::PAT)->appendText(modelPAT());
        pModel->getTab()->getItemEdit(rdo::FileType::DPT)->appendText(modelDPT());
    }
}

void PluginGame5GenerateSituationDialog::onPluginAction()
{
    QStringList funList = parseModelFUN();
    QStringListModel* stringModel = (QStringListModel*)lineEditCustom->completer()->model();
    stringModel->setStringList(funList);
    exec();
}

QStringList PluginGame5GenerateSituationDialog::parseModelFUN() const
{
    rdo::gui::model::Model* pModel = getCurrentModel();
    std::stringstream txtStream;
    pModel->getTab()->getItemEdit(rdo::FileType::FUN)->save(txtStream);
    QString tabStr = QString::fromStdString(txtStream.str());
    QRegExp regExp("(\\$Function)(\\s*)([A-Za-z0-9_А-Яа-я\\$]*)(\\s*):");

    QStringList list;
    int pos = 0;
    while ((pos = regExp.indexIn(tabStr, pos))!= -1)
    {
        list << regExp.cap(3);
        pos += regExp.matchedLength();
    }
    return list;
}

rdo::gui::model::Model* PluginGame5GenerateSituationDialog::getCurrentModel() const
{
    MainWindow* pMainWindow = (MainWindow*)(parent());
    return pMainWindow->getModel();
}

std::vector<unsigned int> PluginGame5GenerateSituationDialog::getBoardState() const
{
    return gameBoard->getBoardState();
}
