#ifndef DOWNLOAD_OPTIONS_H
#define DOWNLOAD_OPTIONS_H

#include "download_options_base.h"

// -------------------------------------------------------
// -------- WGDownloadOptionsDlg
// -------------------------------------------------------
class WGDownloadOptionsDlg: public WGDownloadOptionsDlgBase
{ Q_OBJECT

public:
	WGDownloadOptionsDlg( QWidget* parent = NULL, const char* name = NULL, bool modal = FALSE, WFlags fl = 0 );
	~WGDownloadOptionsDlg();

	void updateControls();

protected slots:
	void slot_ok();
	void slot_cancel();
	void slot_directory();
	void slot_restart();
	void slot_password();
	void slot_log();
	void slot_log_directory();

};

#endif // DOWNLOAD_OPTIONS_H
