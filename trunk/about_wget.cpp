#include "about_wget.h"

#include <qlayout.h>
#include <qtextedit.h>

// -------------------------------------------------------
// -------- WGAboutWgetDialog
// -------------------------------------------------------
WGAboutWgetDialog::WGAboutWgetDialog( QWidget* parent, const char* name, bool modal, WFlags fl ): WGAboutWgetDialogBase( parent, name, modal, fl )
{
}

WGAboutWgetDialog::~WGAboutWgetDialog()
{
}

void WGAboutWgetDialog::insertOutStrList( WGUnicodeLog& strList, const bool new_line )
{
	for ( unsigned int i = 0; i < strList.unicode_count(); i++ ) {
		if ( i == 0 && !new_line ) {
			LogTV->setText( LogTV->text() + strList.unicode_at(i) );
		} else {
			LogTV->append( strList.unicode_at(i) );
		}
	}
}

