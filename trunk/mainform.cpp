#include "mainform.h"
#include "wget_process.h"
#include "log_view.h"
#include "default_options.h"
#include "download_options.h"
#include "about_winwget_base.h"
#include "about_wget.h"
#include "unicode_log.h"

#include "pixmap/about.xpm"
#include "pixmap/delete.xpm"
#include "pixmap/delete_d.xpm"
#include "pixmap/exit.xpm"
#include "pixmap/new.xpm"
#include "pixmap/open.xpm"
#include "pixmap/options.xpm"
#include "pixmap/pause.xpm"
#include "pixmap/pause_d.xpm"
#include "pixmap/start.xpm"

#include <qvariant.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qaction.h>
#include <qsplitter.h>
#include <qlistview.h>
#include <qtimer.h>
#include <qsize.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qfiledialog.h>
#include <qfile.h>
#include <qtextstream.h>

// -------------------------------------------------------
// -------- WGMainForm
// -------------------------------------------------------
WGMainForm::WGMainForm( QWidget* parent, const char* name, WFlags fl ):
	QMainWindow( parent, name, fl ),
	image_start( (const char**)image_start_data ),
	image_pause( QPixmap((const char**)image_pause_data) ),
	optionsDownloadDlgShowing( false )
{
	setCaption( "winWGet" );
	setIcon( QPixmap((const char**)image_about_data) );

	timer = new QTimer( this );
	timer->start( 5000 );

	image_pause.setPixmap( QPixmap((const char**)image_pause_d_data), QIconSet::Automatic, QIconSet::Disabled );

	QIconSet image_delete( QPixmap((const char**)image_delete_data) );
	image_delete.setPixmap( QPixmap((const char**)image_delete_d_data), QIconSet::Automatic, QIconSet::Disabled );

	QSplitter* splitter = new QSplitter( this );
	splitter->setOrientation( QSplitter::Vertical );
	setCentralWidget( splitter );
	toolBar = new QToolBar( "", this, Top );
	menubar = new QMenuBar( this );

	fileOpenURLListAction = new QAction( "Open URL List", QIconSet( QPixmap((const char**)image_open_data) ), "&Open URL List", 4194383, this );
	connect( fileOpenURLListAction, SIGNAL(activated()), this, SLOT(slot_open_url_list()) );

	fileExitAction = new QAction( "Exit", QIconSet( QPixmap((const char**)image_exit_data) ), "E&xit", 0, this );
	connect( fileExitAction, SIGNAL(activated()), this, SLOT(slot_exit()) );

	downloadNewAction = new QAction( "New Download", QIconSet( QPixmap((const char**)image_new_data) ), "&New Download", 4194382, this );
	connect( downloadNewAction, SIGNAL(activated()), this, SLOT(slot_new()) );

	downloadOptionsAction = new QAction( "Default Options", QIconSet( QPixmap((const char**)image_options_data) ), "Default O&ptions", 4194384, this );
	connect( downloadOptionsAction, SIGNAL(activated()), this, SLOT(slot_options_default()) );

	downloadStartPauseAction = new QAction( "Start/Pause Download", image_pause, "P&ause Download", 4194369, this );
	connect( downloadStartPauseAction, SIGNAL(activated()), this, SLOT(slot_start_pause()) );

	downloadDeleteAction = new QAction( "Delete Process", image_delete, "&Delete Process", 4194372, this );
	connect( downloadDeleteAction, SIGNAL(activated()), this, SLOT(slot_delete()) );

	viewLogAction = new QAction( "Show Log", "Show &Log", 0, this );
	viewLogAction->setToggleAction( true );
	viewLogAction->setOn( true );
	connect( viewLogAction, SIGNAL(activated()), this, SLOT(slot_show_log()) );

	viewToolBarAction = new QAction( "Show ToolBar", "Show &ToolBar", 0, this );
	viewToolBarAction->setToggleAction( true );
	viewToolBarAction->setOn( true );
	connect( viewToolBarAction, SIGNAL(activated()), this, SLOT(slot_show_toolbar()) );

	viewGridColStatusAction = new QAction( "Show Status Column", "Show &Status", 0, this );
	viewGridColStatusAction->setToggleAction( true );
	viewGridColStatusAction->setOn( true );

	viewGridColTotalAction = new QAction( "Show Total Column", "Show &Total", 0, this );
	viewGridColTotalAction->setToggleAction( true );
	viewGridColTotalAction->setOn( true );

	viewGridColCurrentAction = new QAction( "Show Current Column", "Show &Current", 0, this );
	viewGridColCurrentAction->setToggleAction( true );
	viewGridColCurrentAction->setOn( true );

	viewGridColProgressAction = new QAction( "Show Progress Column", "Show &Progress", 0, this );
	viewGridColProgressAction->setToggleAction( true );
	viewGridColProgressAction->setOn( true );

	viewGridColSpeedAction = new QAction( "Show Speed Column", "Show Sp&eed", 0, this );
	viewGridColSpeedAction->setToggleAction( true );
	viewGridColSpeedAction->setOn( true );

	viewGridColURLAction = new QAction( "Show URL Column", "Show &URL", 0, this );
	viewGridColURLAction->setToggleAction( true );
	viewGridColURLAction->setOn( true );

	viewGridColLocalFileAction = new QAction( "Show Local File Column", "Show &Local File", 0, this );
	viewGridColLocalFileAction->setToggleAction( true );
	viewGridColLocalFileAction->setOn( true );

	connect( viewGridColStatusAction   , SIGNAL(activated()), this, SLOT(slot_show_grid_columns()) );
	connect( viewGridColTotalAction    , SIGNAL(activated()), this, SLOT(slot_show_grid_columns()) );
	connect( viewGridColCurrentAction  , SIGNAL(activated()), this, SLOT(slot_show_grid_columns()) );
	connect( viewGridColProgressAction , SIGNAL(activated()), this, SLOT(slot_show_grid_columns()) );
	connect( viewGridColSpeedAction    , SIGNAL(activated()), this, SLOT(slot_show_grid_columns()) );
	connect( viewGridColURLAction      , SIGNAL(activated()), this, SLOT(slot_show_grid_columns()) );
	connect( viewGridColLocalFileAction, SIGNAL(activated()), this, SLOT(slot_show_grid_columns()) );

	helpAboutAction = new QAction( "About", QIconSet( QPixmap((const char**)image_about_data) ), "&About...", 0, this );
	connect( helpAboutAction, SIGNAL(activated()), this, SLOT(slot_about()) );

	helpAboutWGetAction = new QAction( "About wget", "About &wget", 0, this );
	connect( helpAboutWGetAction, SIGNAL(activated()), this, SLOT(slot_about_wget()) );

	fileMenu = new QPopupMenu( this );
	fileOpenURLListAction->addTo( fileMenu );
	fileMenu->insertSeparator();
	fileExitAction->addTo( fileMenu );
	menubar->insertItem( "&File", fileMenu );

	downloadMenu = new QPopupMenu( this );
	downloadNewAction->addTo( downloadMenu );
	downloadMenu->insertSeparator();
	downloadOptionsAction->addTo( downloadMenu );
	downloadMenu->insertSeparator();
	downloadStartPauseAction->addTo( downloadMenu );
	downloadDeleteAction->addTo( downloadMenu );
	menubar->insertItem( "&Download", downloadMenu );

	viewMenu = new QPopupMenu( this );
	viewLogAction->addTo( viewMenu );
	viewToolBarAction->addTo( viewMenu );
	menubar->insertItem( "&View", viewMenu );

	viewGridMenu = new QPopupMenu( this );
	viewGridColStatusAction->addTo( viewGridMenu );
	viewGridColTotalAction->addTo( viewGridMenu );
	viewGridColCurrentAction->addTo( viewGridMenu );
	viewGridColProgressAction->addTo( viewGridMenu );
	viewGridColSpeedAction->addTo( viewGridMenu );
	viewGridColURLAction->addTo( viewGridMenu );
	viewGridColLocalFileAction->addTo( viewGridMenu );
	viewMenu->insertItem( "Show &Grid Columns", viewGridMenu );

	helpMenu = new QPopupMenu( this );
	helpAboutAction->addTo( helpMenu );
	helpAboutWGetAction->addTo( helpMenu );
	menubar->insertItem( "&Help", helpMenu );

	toolBar->setLabel( "Tools" );
	downloadNewAction->addTo( toolBar );
	fileOpenURLListAction->addTo( toolBar );
	downloadStartPauseAction->addTo( toolBar );
	downloadDeleteAction->addTo( toolBar );

	procList = new WGProcList( centralWidget() );
	connect( procList, SIGNAL(selectionChanged(QListViewItem*)), this, SLOT(slot_proc_select(QListViewItem*)) );
	connect( procList, SIGNAL(currentChanged(QListViewItem*)), this, SLOT(slot_proc_changed(QListViewItem*)) );
	connect( timer, SIGNAL(timeout()), procList, SLOT(slot_update_file_speed()) );

	log = new WGLogView( centralWidget() );

	updateControls();

	connect( qApp->clipboard(), SIGNAL(dataChanged()), this, SLOT(slot_clipboard_changed()) );

}

WGMainForm::~WGMainForm()
{
}

void WGMainForm::slot_open_url_list()
{
	QString fileName = QFileDialog::getOpenFileName( QString::null, "All files (*.*)", this );
	if ( !fileName.isEmpty() ) {
		QFile file( fileName );
		if ( file.open( IO_ReadOnly ) ) {
			QTextStream stream( &file );
			while ( !stream.atEnd() ) {
				QString url = stream.readLine().stripWhiteSpace();
				if ( !url.isEmpty() && ( url.find("ftp://") == 0 || url.find("http://") == 0 ) ) newDownload( url, true );
			}
		}
	}
}

void WGMainForm::slot_exit()
{
	close();
}

void WGMainForm::slot_new()
{
	newDownload();
}

void WGMainForm::slot_delete()
{
	WGProcListItem* item_proc = (WGProcListItem*)procList->selectedItem();
	if ( item_proc && item_proc->getProc() ) {
		WGProcess_s->deleteProcess( item_proc->getProc() );
		updateControls();
	}
}

void WGMainForm::slot_start_pause()
{
	WGProcListItem* item_proc = (WGProcListItem*)procList->selectedItem();
	if ( item_proc ) {
		WGProcess* proc = item_proc->getProc();
		if ( proc ) proc->setPause( !proc->getPause() );
	}
	updateControls();
}

void WGMainForm::slot_options_default()
{
	WGDefaultOptionsDlg* dialog = new WGDefaultOptionsDlg( this, NULL, true );
	dialog->DirectoryLE->setText( WGProcess_s->saveToDir );
	dialog->RestartCB->setChecked( WGProcess_s->doRestart );
	dialog->DontUseSuffixCB->setChecked( WGProcess_s->dontUseSuffix );
	dialog->OlderCB->setChecked( WGProcess_s->dontRetrieveOldFile );
	dialog->PasswordCB->setChecked( WGProcess_s->useUsernameAndPassword );
	dialog->PasswordUsrLE->setText( WGProcess_s->username );
	dialog->PasswordPassLE->setText( WGProcess_s->password );
	dialog->LogCB->setChecked( WGProcess_s->writeLog );
	dialog->LogDirectoryLE->setText( WGProcess_s->logDirectory );
	if ( WGProcess_s->retriesNumber != -1 ) {
		dialog->RetriesNumberLE->setText( QString::number( WGProcess_s->retriesNumber ));
	} else {
		dialog->RetriesNumberLE->setText( "" );
	}
	if ( WGProcess_s->waitBetweenRetrievals != -1 ) {
		dialog->RetriesWaitLE->setText( QString::number( WGProcess_s->waitBetweenRetrievals ));
	} else {
		dialog->RetriesWaitLE->setText( "" );
	}
	dialog->ClipboardCheckCB->setChecked( WGProcess_s->checkClipboard );
	dialog->ClipboardAutoStartCB->setChecked( WGProcess_s->autostartClipboard );
	dialog->updateControls();
	if ( dialog->exec() == QDialog::Accepted ) {
		WGProcess_s->saveToDir = dialog->DirectoryLE->text();
		WGProcess_s->doRestart = dialog->RestartCB->isChecked();
		WGProcess_s->dontUseSuffix = dialog->DontUseSuffixCB->isChecked();
		WGProcess_s->dontRetrieveOldFile = dialog->OlderCB->isChecked();
		WGProcess_s->useUsernameAndPassword = dialog->PasswordCB->isChecked();
		WGProcess_s->username = dialog->PasswordUsrLE->text();
		WGProcess_s->password = dialog->PasswordPassLE->text();
		WGProcess_s->writeLog = dialog->LogCB->isChecked();
		WGProcess_s->logDirectory = dialog->LogDirectoryLE->text();

		bool ok = true;
		int v = dialog->RetriesNumberLE->text().toInt( &ok );
		WGProcess_s->retriesNumber = ok ? v : -1;

		ok = true;
		v = dialog->RetriesWaitLE->text().toInt( &ok );
		WGProcess_s->waitBetweenRetrievals = ok ? v : -1;

		WGProcess_s->checkClipboard = dialog->ClipboardCheckCB->isChecked();
		WGProcess_s->autostartClipboard = dialog->ClipboardAutoStartCB->isChecked();
		WGProcess_s->writeConfig();
	}
	delete dialog;
}

void WGMainForm::slot_show_log()
{
	viewLogAction->isOn() ? log->show() : log->hide();
}

void WGMainForm::slot_show_toolbar()
{
	viewToolBarAction->isOn() ? toolBar->show() : toolBar->hide();
}

void WGMainForm::slot_show_grid_columns()
{
	WGProcList::ShowColumns columns;
	columns.scStatus    = viewGridColStatusAction->isOn();
	columns.scTotal     = viewGridColTotalAction->isOn();
	columns.scCurrent   = viewGridColCurrentAction->isOn();
	columns.scProgress  = viewGridColProgressAction->isOn();
	columns.scSpeed     = viewGridColSpeedAction->isOn();
	columns.scURL       = viewGridColURLAction->isOn();
	columns.scLocalFile = viewGridColLocalFileAction->isOn();

	procList->updateShowColumns( columns );
}

void WGMainForm::slot_proc_select( QListViewItem* item )
{
	unsigned int i;

	for ( i = 0; i < WGProcess_s->count(); i++ )
		WGProcess_s->at( i )->disconnect( SIGNAL(signal_log_change(WGUnicodeLog&, const bool)), log, SLOT(insertOutStrList(WGUnicodeLog&, const bool)) );

	log->clear();
	if ( item ) {
		WGUnicodeLog strList = ((WGProcListItem*)item)->getProc()->getFullLog();
		unsigned int line_count = strList.unicode_count();
		for ( i = 0; i < line_count; i++ ) {
			WGLogViewItem* item_log = new WGLogViewItem( log );
			item_log->setText( column_log, strList.unicode_at(i) );
		}
		log->verticalScrollBar()->setValue( log->verticalScrollBar()->maxValue() );
		log->focusLastItem( true );
	}

	updateControls();

	if ( item ) connect( ((WGProcListItem*)item)->getProc(), SIGNAL(signal_log_change(WGUnicodeLog&, const bool)), log, SLOT(insertOutStrList(WGUnicodeLog&, const bool)) );
}

void WGMainForm::slot_proc_changed( QListViewItem* )
{
	if ( !procList->childCount() ) {
		log->clear();
		updateControls();
	}
}

void WGMainForm::updateControls()
{
	WGProcListItem* item_proc = (WGProcListItem*)procList->selectedItem();
	bool flag = item_proc == NULL;
	if ( item_proc ) {
		WGProcess* proc = item_proc->getProc();
		if ( proc && proc->getPause() ) {
				downloadStartPauseAction->setIconSet( QIconSet(image_start) );
				downloadStartPauseAction->setMenuText( "St&art Download" );
		} else {
			flag = true;
		}
	}
	if ( flag ) {
		downloadStartPauseAction->setIconSet( image_pause );
		downloadStartPauseAction->setMenuText( "P&ause Download" );
	}

	downloadStartPauseAction->setEnabled( procList->selectedItem() );
	downloadDeleteAction->setEnabled( procList->selectedItem() );

	slot_show_log();
	slot_show_toolbar();
}

void WGMainForm::slot_clipboard_changed()
{
	QString s = qApp->clipboard()->text();
	if ( !s.isEmpty() ) {
		s = s.stripWhiteSpace();
		int length = (int)s.length();
		if ( (   s.find( "ftp://" ) == 0 ||
		       ( s.find( "http://" ) == 0 &&
		         ( s.find(".html", length-5) != length-5 &&
			       s.find(".htm", length-4) != length-4
		         )
		       )
		     ) && s.find( "/", length-1 ) != length-1
		   ) {
			newDownload( s, WGProcess_s->autostartClipboard );
		}
	}
}

void WGMainForm::newDownload( QString url, const bool autostart )
{
	if ( !optionsDownloadDlgShowing ) {

		bool doDownload = autostart;
		WGDownloadOptionsDlg* dialog = NULL;

		if ( !autostart ) {
			dialog = new WGDownloadOptionsDlg( this, NULL, true );
			optionsDownloadDlgShowing = true;
			dialog->URLLE->setText( url );
			dialog->DirectoryLE->setText( WGProcess_s->saveToDir );
			dialog->RestartCB->setChecked( WGProcess_s->doRestart );
			dialog->DontUseSuffixCB->setChecked( WGProcess_s->dontUseSuffix );
			dialog->OlderCB->setChecked( WGProcess_s->dontRetrieveOldFile );
			dialog->PasswordCB->setChecked( WGProcess_s->useUsernameAndPassword );
			dialog->PasswordUsrLE->setText( WGProcess_s->username );
			dialog->PasswordPassLE->setText( WGProcess_s->password );
			dialog->LogCB->setChecked( WGProcess_s->writeLog );
			dialog->LogDirectoryLE->setText( WGProcess_s->logDirectory );
			if ( WGProcess_s->retriesNumber != -1 ) {
				dialog->RetriesNumberLE->setText( QString::number( WGProcess_s->retriesNumber ));
			} else {
				dialog->RetriesNumberLE->setText( "" );
			}
			if ( WGProcess_s->waitBetweenRetrievals != -1 ) {
				dialog->RetriesWaitLE->setText( QString::number( WGProcess_s->waitBetweenRetrievals ));
			} else {
				dialog->RetriesWaitLE->setText( "" );
			}
			dialog->updateControls();
			doDownload = dialog->exec() == QDialog::Accepted;
		}
		if ( doDownload ) {
			
			if ( dialog ) url = dialog->URLLE->text().stripWhiteSpace();

			if ( !url.isEmpty() ) {

				WGProcess* proc = WGProcess_s->newProcess();
				proc->setURL( url );
				if ( dialog ) {
					proc->saveToDir = dialog->DirectoryLE->text();
					proc->doRestart = dialog->RestartCB->isChecked();
					proc->dontUseSuffix = dialog->DontUseSuffixCB->isChecked();
					proc->dontRetrieveOldFile = dialog->OlderCB->isChecked();
					proc->useUsernameAndPassword = dialog->PasswordCB->isChecked();
					proc->username = dialog->PasswordUsrLE->text();
					proc->password = dialog->PasswordPassLE->text();
					proc->writeLog = dialog->LogCB->isChecked();
					proc->logDirectory = dialog->LogDirectoryLE->text();
					
					bool ok = true;
					int v = dialog->RetriesNumberLE->text().toInt( &ok );
					proc->retriesNumber = ok ? v : -1;

					ok = true;
					v = dialog->RetriesWaitLE->text().toInt( &ok );
					proc->waitBetweenRetrievals = ok ? v : -1;
					
				} else {
					proc->saveToDir = WGProcess_s->saveToDir;
					proc->doRestart = WGProcess_s->doRestart;
					proc->dontUseSuffix = WGProcess_s->dontUseSuffix;
					proc->dontRetrieveOldFile = WGProcess_s->dontRetrieveOldFile;
					proc->writeLog = WGProcess_s->writeLog;
					proc->logDirectory = WGProcess_s->logDirectory;
					proc->retriesNumber = WGProcess_s->retriesNumber;
					proc->waitBetweenRetrievals = WGProcess_s->waitBetweenRetrievals;
				}
				connect( proc, SIGNAL(signal_start(WGProcess*)), procList, SLOT(slot_proc_start(WGProcess*)) );
				connect( proc, SIGNAL(signal_close(WGProcess*)), procList, SLOT(slot_proc_close(WGProcess*)) );
				connect( proc, SIGNAL(signal_local_fine_name_change(WGProcess*)), procList, SLOT(slot_local_fine_name_change(WGProcess*)) );
				connect( proc, SIGNAL(signal_file_size(WGProcess*, const unsigned long int)), procList, SLOT(slot_get_file_size(WGProcess*, const unsigned long int)) );
				connect( proc, SIGNAL(signal_file_progress(WGProcess*)), procList, SLOT(slot_get_file_progress(WGProcess*)) );
				connect( proc, SIGNAL(signal_change_status(WGProcess*, WGProcessStatus)), procList, SLOT(slot_change_proc_status(WGProcess*, WGProcessStatus)) );
				connect( proc, SIGNAL(signal_change_status(WGProcess*, WGProcessStatus)), this, SLOT(slot_change_proc_status(WGProcess*, WGProcessStatus)) );

				proc->start();
			}
		}
		if ( dialog ) {
			delete dialog;
			optionsDownloadDlgShowing = false;
		}
		updateControls();
	}
}

void WGMainForm::slot_about()
{
	WGAboutDialogBase dialog( this, NULL, true );
	dialog.setIcon( QPixmap((const char**)image_about_data) );
	dialog.exec();
}

void WGMainForm::slot_about_wget()
{
	WGAboutWgetDialog dialog( this, NULL, true );
	WGProcess* proc = WGProcess_s->newWgetVersionProcess();
	connect( proc, SIGNAL(signal_log_change(WGUnicodeLog&, const bool)), &dialog, SLOT(insertOutStrList(WGUnicodeLog&, const bool)) );
	proc->version();
	dialog.setIcon( QPixmap((const char**)image_about_data) );
	dialog.exec();
	WGProcess_s->deleteWgetVersionProcess( proc );
}

void WGMainForm::slot_change_proc_status( WGProcess*, WGProcessStatus )
{
	updateControls();
}
