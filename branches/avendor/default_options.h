#ifndef DEFAULT_OPTIONS_H
#define DEFAULT_OPTIONS_H

#include "default_options_base.h"

#include <qstring.h>

// -------------------------------------------------------
// -------- WGDefaultOptionsDlg
// -------------------------------------------------------
class WGDefaultOptionsDlg: public WGDefaultOptionsDlgBase
{ Q_OBJECT

public:
	WGDefaultOptionsDlg( QWidget* parent = NULL, const char* name = NULL, bool modal = FALSE, WFlags fl = 0 );
	~WGDefaultOptionsDlg();

	void updateControls();

	QString saveDir;

protected slots:
	void slot_ok();
	void slot_cancel();
	void slot_directory();
	void slot_restart();
	void slot_password();
	void slot_log();
	void slot_log_directory();
	void slot_clipboard_check();

};

#endif // DEFAULT_OPTIONS_H
