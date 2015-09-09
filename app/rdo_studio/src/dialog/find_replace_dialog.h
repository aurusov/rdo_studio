#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <boost/function.hpp>
#include <QDialog>
#include "ui_find_replace_dialog.h"
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/dialog/find_dialog.h"
// --------------------------------------------------------------------------------

class FindReplaceDialog
    : public QDialog
    , public Ui::FindReplaceDialog
{
Q_OBJECT

public:
    struct Settings : public FindDialog::Settings
    {
        QString byWhat;

        Settings();
        Settings(const Settings& settings);
    };

    typedef  boost::function<void (const Settings&)>  OnFindCallback;
    typedef  boost::function<void ()>                 OnCloseCallback;

    FindReplaceDialog(QWidget* pParent, const OnFindCallback& onFindCallback, const OnFindCallback& onReplaceCallback, const OnFindCallback& onReplaceAllCallback, const OnCloseCallback& onCloseCallback);
    virtual ~FindReplaceDialog();

    void setSettings(const Settings& settings);

private:
    Settings         m_settings;
    OnFindCallback   m_onFindCallback;
    OnFindCallback   m_onReplaceCallback;
    OnFindCallback   m_onReplaceAllCallback;
    OnCloseCallback  m_onCloseCallback;

private slots:
    void onFindButton();
    void onReplaceButton();
    void onReplaceAllButton();
    void onWhatEdited           (const QString& text);
    void onByWhatEdited         (const QString& text);
    void onMatchCaseChanged     (int value);
    void onMatchWholeWordChanged(int value);
    void onDirectionDownToggled (bool checked);
};
