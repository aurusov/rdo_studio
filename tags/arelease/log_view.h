#ifndef LOG_VIEW_H
#define LOG_VIEW_H

#include "unicode_log.h"

#include <qlistview.h>
#include <qevent.h>
#include <qrect.h>
#include <qpainter.h>
#include <qpalette.h>

// -------------------------------------------------------
// -------- WGLogView
// -------------------------------------------------------
#define column_log  1

class WGLogView: public QListView
{ Q_OBJECT

friend class WGLogViewItem;

private:
	bool canFocusLastItem;
  
protected:
	virtual bool event( QEvent* event );
	virtual void resizeEvent( QResizeEvent* event );
	void repaintViewport();

public:
	WGLogView( QWidget* parent = NULL, const char* name = NULL, WFlags fl = 0 );
	~WGLogView();

	void focusLastItem( const bool always = false );

protected slots:
	void vertScrollBarValueChanged( int value );

public slots:
	void insertOutStrList( WGUnicodeLog& strList, const bool new_line = true );

};

// -------------------------------------------------------
// -------- WGLogViewItem
// -------------------------------------------------------
class WGLogViewItem: public QListViewItem
{
friend class WGLogView;

public:
	WGLogViewItem( const WGLogView* const parent );
	~WGLogViewItem();

protected:
	virtual void paintCell( QPainter* painter, const QColorGroup& cg, int column, int width, int align );
	virtual void paintFocus( QPainter* painter, const QColorGroup& cg, const QRect& rect );
};

#endif // LOG_VIEW_H
