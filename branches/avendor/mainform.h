#ifndef MAINFORM_H
#define MAINFORM_H

#include "processes_list.h"

#include <qmainwindow.h>
#include <qwidget.h>
#include <qcstring.h>
#include <qstring.h>
#include <qlistview.h>
#include <qrect.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qpixmap.h>
#include <qiconset.h>

// -------------------------------------------------------
// -------- WGMainForm
// -------------------------------------------------------
class WGProcList;
class WGLogView;

class QMenuBar;
class QPopupMenu;
class QToolBar;
class QAction;
class QListView;
class QTimer;

class WGMainForm: public QMainWindow
{ Q_OBJECT

private:
	WGProcList* procList;
	WGLogView* log;

	QTimer* timer;

	QMenuBar* menubar;
	QPopupMenu* fileMenu;
	QPopupMenu* downloadMenu;
	QPopupMenu* viewMenu;
	QPopupMenu* viewGridMenu;
	QPopupMenu* helpMenu;
	QToolBar* toolBar;
	QAction* fileOpenURLListAction;
	QAction* fileExitAction;
	QAction* downloadNewAction;
	QAction* downloadOptionsAction;
	QAction* downloadStartPauseAction;
	QAction* downloadDeleteAction;
	QAction* viewLogAction;
	QAction* viewToolBarAction;
	QAction* viewGridColStatusAction;
	QAction* viewGridColTotalAction;
	QAction* viewGridColCurrentAction;
	QAction* viewGridColProgressAction;
	QAction* viewGridColSpeedAction;
	QAction* viewGridColURLAction;
	QAction* viewGridColLocalFileAction;
	QAction* helpAboutAction;
	QAction* helpAboutWGetAction;

	QPixmap image_start;
	QIconSet image_pause;

	bool optionsDownloadDlgShowing;

	void updateControls();
	void newDownload( QString url = "", const bool autostart = false );

protected slots:
	void slot_open_url_list();
	void slot_exit();
	void slot_options_default();
	void slot_new();
	void slot_start_pause();
	void slot_delete();
	void slot_show_log();
	void slot_show_toolbar();
	void slot_show_grid_columns();
	void slot_about();
	void slot_about_wget();
	void slot_proc_select( QListViewItem* item );
	void slot_proc_changed( QListViewItem* item );
	void slot_clipboard_changed();
	void slot_change_proc_status( WGProcess* proc, WGProcessStatus status );

public:
	WGMainForm( QWidget* parent = NULL, const char* name = NULL, WFlags fl = 0 );
	~WGMainForm();
};

#endif // MAINFORM_H


