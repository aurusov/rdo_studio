#ifndef PROCESSES_LIST_H
#define PROCESSES_LIST_H

#include "wget_process.h"

#include <qlistview.h>
#include <qrect.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qpixmap.h>

// -------------------------------------------------------
// -------- WGProcList
// -------------------------------------------------------
class WGProcListItem;

class WGProcList: public QListView
{ Q_OBJECT

friend class WGProcListItem;

private:
	int column_status_width;
	int column_total_width;
	int column_current_width;
	int column_progress_width;
	int column_speed_width;
	int column_url_width;
	int column_local_file_width;

	QPixmap image_circle_green;
	QPixmap image_pause;
	QPixmap image_stop;
	QPixmap image_ok;
	QPixmap image_faild;

protected:
	void repaintViewport();
	QString fileSizeToString( const unsigned long int file_size );
	WGProcListItem* findProcListItem( WGProcess* proc );

public:
	WGProcList( QWidget* parent = NULL, const char* name = NULL, WFlags fl = 0 );
	~WGProcList();

	typedef struct {
		bool scStatus;
		bool scTotal;
		bool scCurrent;
		bool scProgress;
		bool scSpeed;
		bool scURL;
		bool scLocalFile;
	} ShowColumns;

	void updateShowColumns( const ShowColumns cols );

protected slots:
	void headerSizeChange( int section, int oldSize, int newSize );

public slots:
	void slot_proc_start( WGProcess* proc );
	void slot_proc_close( WGProcess* proc );
	void slot_local_fine_name_change( WGProcess* proc );
	void slot_get_file_size( WGProcess* proc, const unsigned long int file_size );
	void slot_get_file_progress( WGProcess* proc );
	void slot_update_file_speed();
	void slot_change_proc_status( WGProcess* proc, WGProcessStatus status );

};

// -------------------------------------------------------
// -------- WGProcListItem
// -------------------------------------------------------
class WGProcListItem: public QListViewItem
{

friend class WGProcList;

private:
	WGProcess* proc;

protected:
	virtual void paintCell( QPainter* painter, const QColorGroup& cg, int column, int width, int align );
	virtual int width( const QFontMetrics& fm, const QListView* lv, int column ) const;
	
public:
	WGProcListItem( const WGProcList* const parent, WGProcess* process );
	~WGProcListItem();

	WGProcess* getProc() const;

};

#endif // PROCESSES_LIST_H
