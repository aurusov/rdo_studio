#ifndef ABOUT_WGET_H
#define ABOUT_WGET_H

#include "about_wget_base.h"
#include "unicode_log.h"

// -------------------------------------------------------
// -------- WGAboutWgetDialog
// -------------------------------------------------------
class WGAboutWgetDialog: public WGAboutWgetDialogBase
{ Q_OBJECT

public:
	WGAboutWgetDialog( QWidget* parent = NULL, const char* name = NULL, bool modal = FALSE, WFlags fl = 0 );
	~WGAboutWgetDialog();

public slots:
	void insertOutStrList( WGUnicodeLog& strList, const bool new_line = true );

};

#endif // ABOUT_WGET_H


