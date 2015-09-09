#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class CheckableLineEdit: public QWidget
{Q_OBJECT

public:
    CheckableLineEdit(QWidget* parent);
    QCheckBox& getCheckBox() {return *checkBox;};
    QLineEdit& getLineEdit() {return *lineEdit;};

private:
    QCheckBox*    checkBox;
    QLineEdit*    lineEdit;
    const QString defaultText;

private slots:
    void setLineEditState(int state);
};
