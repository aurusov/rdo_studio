#include "log_view.h"

#include <qheader.h>
#include <qfont.h>
#include <qpen.h>
#include <qcolor.h>

// -------------------------------------------------------
// -------- WGLogView
// -------------------------------------------------------
#define column_margin 0

WGLogView::WGLogView( QWidget* parent, const char* name, WFlags fl ):
	QListView( parent, name, fl ),
	canFocusLastItem( true )
{
	QFont f( font() );
	f.setFamily( "Courier" );
	setFont( f );

	setFrameShape( QFrame::Box );
	setFrameShadow( QFrame::Plain );
	setLineWidth( 1 );

	setHScrollBarMode( QListView::AlwaysOff );
	setSorting( -1 );
	addColumn( "", 15 );
	addColumn( "" );
	setRootIsDecorated( false );
	setMultiSelection( false );
	setSelectionMode( QListView::NoSelection );
	setTreeStepSize( 0 );

	header()->hide();
	disconnect( header(), SIGNAL(sectionClicked(int)), this, SLOT(changeSortColumn(int)) );

	connect( verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarValueChanged(int)) );
}

WGLogView::~WGLogView()
{
}

bool WGLogView::event( QEvent* event )
{
	bool res = QListView::event( event );
	if ( event->type() == QEvent::Paint ) repaintViewport();
	return res;
}

void WGLogView::resizeEvent( QResizeEvent* event )
{
	QListView::resizeEvent( event );
	repaintViewport();
}

void WGLogView::repaintViewport()
{
	int new_w  = clipper()->width() - header()->sectionSize( column_margin );
	header()->resizeSection( column_log, new_w );
	QListViewItem* item = firstChild();
	QRect rect;
	while ( item ) {
		rect = itemRect( item );
		if ( rect.top() > -item->height() ) item->repaint();
		item = item->itemBelow();
	}
}

void WGLogView::insertOutStrList( WGUnicodeLog& strList, const bool new_line )
{
	for ( unsigned int i = 0; i < strList.unicode_count(); i++ ) {
		if ( !new_line && i == 0 && lastItem() ) {
			WGLogViewItem* item = (WGLogViewItem*)lastItem();
			item->setText( column_log, item->text(column_log) + strList.unicode_at(i) );
		} else {
			WGLogViewItem* item = new WGLogViewItem( this );
			item->setText( column_log, strList.unicode_at(i) );
		}
	}
	focusLastItem();
}

void WGLogView::vertScrollBarValueChanged( int )
{
	canFocusLastItem = verticalScrollBar()->value() == verticalScrollBar()->maxValue();
}

void WGLogView::focusLastItem( const bool always )
{
	if ( (canFocusLastItem || always ) && lastItem() ) {
		ensureItemVisible( lastItem() );
		setCurrentItem( lastItem() );
	}
}

// -------------------------------------------------------
// -------- WGLogViewItem
// -------------------------------------------------------
WGLogViewItem::WGLogViewItem( const WGLogView* const parent ): QListViewItem( (QListView*)parent, parent->lastItem() )
{
	setSelectable( false );
}

WGLogViewItem::~WGLogViewItem()
{
}

void WGLogViewItem::paintCell( QPainter* painter, const QColorGroup&, int column, int width, int )
{
	if ( painter ) {

		WGLogView* logList = (WGLogView*)listView();
		int w = width;
		int h = height();

		painter->fillRect( 0, 0, w, h, logList->paletteBackgroundColor() );

		if ( column == column_log ) {
			QString propName = text( column_log );
			painter->setPen( logList->paletteForegroundColor() );
			painter->drawText( 0, 0, w, h - 1, AlignVCenter, propName );
		}
	}
}

void WGLogViewItem::paintFocus( QPainter*, const QColorGroup&, const QRect& )
{
	return;
}


