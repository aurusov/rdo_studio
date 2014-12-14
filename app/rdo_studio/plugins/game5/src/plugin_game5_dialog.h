#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include "ui_plugin_game5_dialog.h"
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/board.h"
#include "app/rdo_studio/src/model/model.h"
// --------------------------------------------------------------------------------

class PluginGame5GenerateSituationDialog
    : public QDialog
    , public Ui_PluginGame5GenerateSituationDialog
{Q_OBJECT

public:
    PluginGame5GenerateSituationDialog(QWidget* parent);
    ~PluginGame5GenerateSituationDialog();

    std::vector<unsigned int> getBoardState() const;

signals:
    void buttonRandomClicked(bool solvabilityCheck);

public slots:
    void onPluginAction();

private:
    std::string evaluateBy() const;

    enum class MoveDirection
    {
        UP,
        DOWN,
        RIGHT,
        LEFT
    };
    std::string activityValue(MoveDirection direction) const;

    QString modelRTP() const;
    QString modelRSS() const;
    QString modelPAT() const;
    QString modelDPT() const;
    QString modelFUN() const;

    void clearAllTabs() const;
    rdo::gui::model::Model* getCurrentModel() const;
    void generateModel() const;

    QStringList parseModelFUN() const;

private slots:
    void callTilesOrderDialog();
    void onClickOk();
    void onClickHide(bool state);
    void emitSolvabilityCheck();
};
