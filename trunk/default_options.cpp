#include "default_options.h"
#include "int_validator.h"

#include "pixmap/open.xpm"
#include "pixmap/open_d.xpm"

#include <qpushbutton.h>
#include <qtoolbutton.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qfiledialog.h>
#include <qiconset.h>
#include <qvalidator.h>
#include <limits.h>

// -------------------------------------------------------
// -------- WGDefaultOptionsDlg
// -------------------------------------------------------
WGDefaultOptionsDlg::WGDefaultOptionsDlg( QWidget* parent, const char* name, bool modal, WFlags fl ): WGDefaultOptionsDlgBase( parent, name, modal, fl )
{
	QIconSet image_open( QPixmap((const char**)image_open_data) );
	image_open.setPixmap( QPixmap((const char**)image_open_d_data), QIconSet::Automatic, QIconSet::Disabled );

	DirectoryB->setIconSet( image_open );
	LogDirectoryB->setIconSet( image_open );

	RetriesNumberLE->setValidator( new WGIntValidator( RetriesNumberLE ) );
	RetriesWaitLE->setValidator( new WGIntValidator( RetriesWaitLE ) );

	connect( OkB             , SIGNAL(clicked())      , this, SLOT(slot_ok())              );
	connect( CancelB         , SIGNAL(clicked())      , this, SLOT(slot_cancel())          );
	connect( DirectoryB      , SIGNAL(clicked())      , this, SLOT(slot_directory())       );
	connect( RestartCB       , SIGNAL(clicked())      , this, SLOT(slot_restart())         );
	connect( PasswordCB      , SIGNAL(clicked())      , this, SLOT(slot_password())        );
	connect( LogCB           , SIGNAL(clicked())      , this, SLOT(slot_log())             );
	connect( LogDirectoryB   , SIGNAL(clicked())      , this, SLOT(slot_log_directory())   );
	connect( ClipboardCheckCB, SIGNAL(clicked())      , this, SLOT(slot_clipboard_check()) );
	connect( PasswordUsrLE   , SIGNAL(returnPressed()), this, SLOT(slot_ok())              );
	connect( PasswordPassLE  , SIGNAL(returnPressed()), this, SLOT(slot_ok())              );
	connect( RetriesNumberLE , SIGNAL(returnPressed()), this, SLOT(slot_ok())              );
	connect( RetriesWaitLE   , SIGNAL(returnPressed()), this, SLOT(slot_ok())              );

	updateControls();
}

WGDefaultOptionsDlg::~WGDefaultOptionsDlg()
{
}

void WGDefaultOptionsDlg::slot_ok()
{
	accept();
}

void WGDefaultOptionsDlg::slot_cancel()
{
	reject();
}

void WGDefaultOptionsDlg::slot_directory()
{
	DirectoryLE->setText( QFileDialog::getExistingDirectory( DirectoryLE->text(), this ) );
}

void WGDefaultOptionsDlg::slot_restart()
{
	bool flag = !RestartCB->isChecked();
	DontUseSuffixCB->setEnabled( flag );
	OlderCB->setEnabled( flag );
}

void WGDefaultOptionsDlg::slot_password()
{
	bool flag = PasswordCB->isChecked();
	bool f = PasswordUsrLE->hasFocus() || PasswordPassLE->hasFocus();
	PasswordUsrL->setEnabled( flag );
	PasswordUsrLE->setEnabled( flag );
	PasswordPassL->setEnabled( flag );
	PasswordPassLE->setEnabled( flag );
	if ( flag ) {
		PasswordUsrLE->setFocus();
	} else {
		if ( f ) PasswordCB->setFocus();
	}
}

void WGDefaultOptionsDlg::slot_log()
{
	bool flag = LogCB->isChecked();
	LogDirectoryL->setEnabled( flag );
	LogDirectoryLE->setEnabled( flag );
	LogDirectoryB->setEnabled( flag );
}

void WGDefaultOptionsDlg::slot_log_directory()
{
	LogDirectoryLE->setText( QFileDialog::getExistingDirectory( LogDirectoryLE->text(), this ) );
}

void WGDefaultOptionsDlg::slot_clipboard_check()
{
	ClipboardAutoStartCB->setEnabled( ClipboardCheckCB->isChecked() );
}

void WGDefaultOptionsDlg::updateControls()
{
	slot_restart();
	slot_password();
	slot_log();
	slot_clipboard_check();
}
