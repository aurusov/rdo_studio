#include "processes_list.h"

#include "pixmap/circle_green.xpm"
#include "pixmap/stop.xpm"
#include "pixmap/pause.xpm"
#include "pixmap/ok.xpm"
#include "pixmap/faild.xpm"

#include <qvariant.h>
#include <qheader.h>
#include <qtextcodec.h>

// -------------------------------------------------------
// -------- WGProcList
// -------------------------------------------------------
#define	column_status_index        0
#define	column_total_index         1
#define	column_current_index       2
#define	column_progress_index      3
#define	column_speed_index         4
#define	column_url_index           5
#define	column_local_file_index    6

WGProcList::WGProcList( QWidget* parent, const char* name, WFlags fl ):
	QListView( parent, name, fl ),
	column_status_width( 40 ),
	column_total_width( 40 ),
	column_current_width( 40 ),
	column_progress_width( 100 ),
	column_speed_width( 40 ),
	column_url_width( 40 ),
	column_local_file_width( 40 ),
	image_circle_green( (const char**)image_circle_green_data ),
	image_pause( (const char**)image_pause_data ),
	image_stop( (const char**)image_stop_data ),
	image_ok( (const char**)image_ok_data ),
	image_faild( (const char**)image_faild_data )
{
	setFrameShape( QFrame::Box );
	setFrameShadow( QFrame::Plain );
	setLineWidth( 1 );

	addColumn( "Status" );
	addColumn( "Total" );
	addColumn( "Current" );
	addColumn( "Progress" );
	addColumn( "Speed" );
	addColumn( "URL" );
	addColumn( "Local file" );

	setColumnAlignment( column_status_index    , Qt::AlignLeft  );
	setColumnAlignment( column_total_index     , Qt::AlignRight );
	setColumnAlignment( column_current_index   , Qt::AlignRight );
	setColumnAlignment( column_progress_index  , Qt::AlignRight );
	setColumnAlignment( column_speed_index     , Qt::AlignRight );
	setColumnAlignment( column_url_index       , Qt::AlignLeft  );
	setColumnAlignment( column_local_file_index, Qt::AlignLeft  );

	setColumnWidth( column_progress_index, column_progress_width );

	setAllColumnsShowFocus( true );
	setItemMargin( 2 );
	setSorting( -1 );

	connect( header(), SIGNAL(sizeChange(int, int, int)), this, SLOT(headerSizeChange(int, int, int)) );
}

WGProcList::~WGProcList()
{
}

void WGProcList::repaintViewport()
{
	QListViewItem* item = firstChild();
	QRect rect;
	while ( item ) {
		rect = itemRect( item );
		if ( rect.top() > -item->height() ) item->repaint();
		item = item->itemBelow();
	}
}

void WGProcList::headerSizeChange( int section, int, int )
{
	if ( section == column_progress_index ) repaintViewport();
}

WGProcListItem* WGProcList::findProcListItem( WGProcess* proc )
{
	WGProcListItem* item = (WGProcListItem*)firstChild();
	while ( item ) {
		if ( item->proc == proc ) return item;
		item = (WGProcListItem*)item->itemBelow();
	}
	return NULL;
}

void WGProcList::slot_proc_start( WGProcess* proc )
{
	WGProcListItem* item = findProcListItem( proc );
	if ( !item ) {
		WGProcListItem* item = new WGProcListItem( this, proc );
		setSelected( item, true );
		ensureItemVisible( item );
	}
}

void WGProcList::slot_proc_close( WGProcess* proc )
{
	WGProcListItem* item = findProcListItem( proc );
	if ( item ) {
		WGProcListItem* item2 = (WGProcListItem*)item->itemBelow();
		if ( !item2 ) item2 = (WGProcListItem*)item->itemAbove();
		if ( item2 ) setSelected( item2, true );
		delete item;
	}
}

void WGProcList::slot_local_fine_name_change( WGProcess* proc )
{
	WGProcListItem* item = findProcListItem( proc );
	if ( item ) {
		item->setText( column_local_file_index, QTextCodec::codecForLocale()->toUnicode( " " + proc->getLocalFileName() + " " ));
	}
}

void WGProcList::slot_get_file_size( WGProcess* proc, const unsigned long int file_size )
{
	WGProcListItem* item = findProcListItem( proc );
	if ( item ) {
		item->setText( column_total_index, fileSizeToString( file_size ) );
	}
}

void WGProcList::slot_get_file_progress( WGProcess* proc )
{
	WGProcListItem* item = findProcListItem( proc );
	if ( item ) {
		item->setText( column_current_index, fileSizeToString( proc->getFileCurrentSize() ));
		item->repaint();
	}
}

QString WGProcList::fileSizeToString( const unsigned long int file_size )
{
	QString str;
	if ( file_size == 0 ) {
		str = "??";
	} else {
		if ( file_size < 1024 ) {
			str.sprintf( "%lu byte", file_size );
		} else {
			if ( file_size < 1024*1024 ) {
				str.sprintf( "%4.1f Kb", double((double)file_size / 1024.0) );
			} else {
				if ( file_size < 1024*1024*1024 ) {
					str.sprintf( "%4.1f Mb", double((double)file_size / 1024.0 / 1024.0) );
				} else {
					str.sprintf( "%4.1f Gb", double((double)file_size / 1024.0 / 1024.0 / 1024.0) );
				}
			}
		}
	}
	return str;
}

void WGProcList::slot_update_file_speed()
{
	WGProcListItem* item = (WGProcListItem*)firstChild();
	QString str;
	while ( item ) {
		unsigned long int speed = item->proc->getLastSpeed();
		if ( speed < 1024 ) {
			str.sprintf( "%lu byte/s", speed );
		} else {
			if ( speed < 1024*1024 ) {
				str.sprintf( "%4.2f Kb/s", double((double)speed / 1024.0) );
			} else {
				if ( speed < 1024*1024*1024 ) {
					str.sprintf( "%4.2f Mb/s", double((double)speed / 1024.0 / 1024.0) );
				} else {
					str.sprintf( "%4.2f Gb/s", double((double)speed / 1024.0 / 1024.0 / 1024.0) );
				}
			}
		}
		item->setText( column_speed_index, str );
		item = (WGProcListItem*)item->itemBelow();
	}
}

void WGProcList::slot_change_proc_status( WGProcess* proc, WGProcessStatus status )
{
	WGProcListItem* item = findProcListItem( proc );
	if ( item ) {
		QString s;;
		QPixmap* pixmap;
		switch ( status ) {
			case psStart                     : s = "start";                        pixmap = &image_circle_green; break;
			case psConnect                   : s = "connect";                      pixmap = &image_circle_green; break;
			case psLogin                     : s = "login";                        pixmap = &image_circle_green; break;
			case psDownload                  : s = "download";                     pixmap = &image_circle_green; break;
			case psFinish                    : s = "finish";                       pixmap = &image_circle_green; break;
			case psSaved                     : s = "saved";                        pixmap = &image_ok;           break;
			case psPause                     : s = "pause";                        pixmap = &image_pause;        break;
			case psWgetNotFound              : s = "wget not found";               pixmap = &image_stop;         break;
			case psURLNotFound               : s = "url not found";                pixmap = &image_faild;        break;
			case psHostNotFound              : s = "host not found";               pixmap = &image_faild;        break;
			case psInvalidHostName           : s = "invalid host name";            pixmap = &image_faild;        break;
			case psFileAlreadyExist          : s = "file already exist";           pixmap = &image_ok;           break;
			case psNoSuchFileOrDirectory     : s = "no such file or directory";    pixmap = &image_faild;        break;
			case psNoSuchFile                : s = "no such file";                 pixmap = &image_faild;        break;
			case psNoMatchesOnPattern        : s = "no matches on pattern";        pixmap = &image_faild;        break;
			case psInvalidPortSpecification  : s = "invalid port specification";   pixmap = &image_faild;        break;
			case psUnknownUnsupportedProtocol: s = "unknown/unsupported protocol"; pixmap = &image_faild;        break;
			case psLoginIncorrect            : s = "login incorrect";              pixmap = &image_faild;        break;
			case psUnknownError              : s = "unknown error";                pixmap = &image_faild;        break;
			default                          : s = "";                             pixmap = &image_ok;           break;
		}
		item->setText( column_status_index, s );
		if ( pixmap ) item->setPixmap( column_status_index, *pixmap );
	}
}

void WGProcList::updateShowColumns( const ShowColumns cols )
{
	QHeader* h = header();

	int status_width     = columnWidth( column_status_index );
	int total_width      = columnWidth( column_total_index );
	int current_width    = columnWidth( column_current_index );
	int progress_width   = columnWidth( column_progress_index );
	int speed_width      = columnWidth( column_speed_index );
	int url_width        = columnWidth( column_url_index );
	int local_file_width = columnWidth( column_local_file_index );

	if ( status_width     ) column_status_width     = status_width;
	if ( total_width      ) column_total_width      = total_width;
	if ( current_width    ) column_current_width    = current_width;
	if ( progress_width   ) column_progress_width   = progress_width;
	if ( speed_width      ) column_speed_width      = speed_width;
	if ( url_width        ) column_url_width        = url_width;
	if ( local_file_width ) column_local_file_width = local_file_width;

	if ( cols.scStatus ) {
		setColumnWidth( column_status_index, column_status_width );
		h->setResizeEnabled( true, column_status_index );
	} else {
		setColumnWidth( column_status_index, 0 );
		h->setResizeEnabled( false, column_status_index );
	}

	if ( cols.scTotal ) {
		setColumnWidth( column_total_index, column_total_width );
		h->setResizeEnabled( true, column_total_index );
	} else {
		setColumnWidth( column_total_index, 0 );
		h->setResizeEnabled( false, column_total_index );
	}

	if ( cols.scCurrent ) {
		setColumnWidth( column_current_index, column_current_width );
		h->setResizeEnabled( true, column_current_index );
	} else {
		setColumnWidth( column_current_index, 0 );
		h->setResizeEnabled( false, column_current_index );
	}

	if ( cols.scProgress ) {
		setColumnWidth( column_progress_index, column_progress_width );
		h->setResizeEnabled( true, column_progress_index );
	} else {
		setColumnWidth( column_progress_index, 0 );
		h->setResizeEnabled( false, column_progress_index );
	}

	if ( cols.scSpeed ) {
		setColumnWidth( column_speed_index, column_speed_width );
		h->setResizeEnabled( true, column_speed_index );
	} else {
		setColumnWidth( column_speed_index, 0 );
		h->setResizeEnabled( false, column_speed_index );
	}

	if ( cols.scURL ) {
		setColumnWidth( column_url_index, column_url_width );
		h->setResizeEnabled( true, column_url_index );
	} else {
		setColumnWidth( column_url_index, 0 );
		h->setResizeEnabled( false, column_url_index );
	}

	if ( cols.scLocalFile ) {
		setColumnWidth( column_local_file_index, column_local_file_width );
		h->setResizeEnabled( true, column_local_file_index );
	} else {
		setColumnWidth( column_local_file_index, 0 );
		h->setResizeEnabled( false, column_local_file_index );
	}
}

// -------------------------------------------------------
// -------- WGProcListItem
// -------------------------------------------------------
WGProcListItem::WGProcListItem( const WGProcList* const parent, WGProcess* process ):
	QListViewItem( (QListView*)parent, parent->lastItem() ),
	proc( process )
{
	setText( column_url_index, " " + proc->getURLWithHidenPassword() + " " );
}

WGProcListItem::~WGProcListItem()
{
}

void WGProcListItem::paintCell( QPainter* painter, const QColorGroup& cg, int column, int width, int align )
{
	if ( painter && column == column_progress_index ) {

		int w = width;
		int h = height();

		double progress = proc->getProgress();
		QString progress_str;
		progress_str.sprintf( "%d%%", int(progress * 100.0) );
		int progress_w = painter->fontMetrics().width( "100%" ) + 10;

		bool selected = isSelected();
		painter->fillRect( 0, 0, w, h, selected ? cg.highlight() : cg.base() );

		if ( w - 9 - progress_w > 0 ) {
			painter->fillRect( 7, 5, (w - 9 - progress_w) * progress, h - 10, Qt::darkGreen );
			painter->setPen( selected ? Qt::white : Qt::black );
			painter->drawRect( 6, 4, w - 7 - progress_w, h - 8 );
		}
		painter->setPen( selected ? cg.highlightedText() : cg.text() );
		painter->drawText( 6, 4, w - 12, h - 8, AlignRight | AlignVCenter, progress_str );

	} else {
		if ( width ) QListViewItem::paintCell( painter, cg, column, width, align );
	}
}

int WGProcListItem::width( const QFontMetrics& fm, const QListView* lv, int column ) const
{
	if ( lv->columnWidth( column ) == 0 ) {
		return 0;
	} else {
		return QListViewItem::width( fm, lv, column );
	}
}

WGProcess* WGProcListItem::getProc() const
{
	return proc;
}

