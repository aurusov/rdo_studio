/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      tracer_ctrl.h
  \author    Захаров Павел
  \date      12.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_TRACER_CTRLS_TRACER_CTRL_H_
#define _RDO_STUDIO_MFC_TRACER_CTRLS_TRACER_CTRL_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtGui/qwidget.h>
#include <QtGui/qevent.h>
#include <QtGui/qabstractscrollarea.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/tracer/ctrls/tracer_ctrl_style.h"
// --------------------------------------------------------------------------------

namespace rdoTracerLog {

// --------------------------------------------------------------------------------
// -------------------- RDOLogCtrl
// --------------------------------------------------------------------------------
typedef std::list< tstring > stringList;

class RDOLogCtrl: public QWidget
{
Q_OBJECT

friend class RDOLogCtrlFindInList;

protected:
	CMutex mutex;
	
	int lineHeight;
	int charWidth;
	int maxStrWidth;
	
	int xPos;
	int yPos;
	int xMax;
	int yMax;
	int xPageSize;
	int yPageSize;
	
	CRect clipRect;
	QRect prevClientRect;
	QRect newClientRect;
	QRect prevWindowRect;
	
	int lastViewableLine;
	int selectedLine;
	int fullRepaintLines;
	rbool focusOnly;

	stringList strings;
	int stringsCount;
	stringList::iterator yPos_iterator;
	void setYPosIterator( const int prev_yPos );
	stringList::iterator findString( int index );
	stringList::reverse_iterator reverse_findString( int index );
	stringList::const_iterator const_findString( int index ) const;
	stringList::const_reverse_iterator const_reverse_findString( int index ) const;

	int   firstFoundLine;
	int   posFind;
	rbool bHaveFound;
	rbool bSearchDown;
	rbool bMatchCase;
	rbool bMatchWholeWord;
	tstring findStr;
	void find( int& result, const rbool searchDown, const rbool matchCase, const rbool matchWholeWord );

	RDOLogStyle*  logStyle;
	virtual rbool getItemColors( const int index, RDOLogColorPair* &colors ) const;
	virtual rbool getItemColors( CREF(tstring) item, RDOLogColorPair* &colors ) const;
	
	void  recalcWidth( const int newMaxStrWidth );
	void  updateScrollBars();

	rbool scrollVertically  (int pos);
	rbool scrollHorizontally(int pos);
	
	rbool isVisible( const int index ) const;
	rbool isFullyVisible( const int index ) const;

	QRect getLineRect(int index) const;
	void  repaintLine ( const int index );

	rbool canCopy() const { return selectedLine != -1; };

	void  updateWindow();

	rbool drawLog;

	QFont m_font;
	void setFont(rbool needRedraw);

	PTR(QAbstractScrollArea) m_pScrollArea;
	int m_prevVertSBValue;
	int m_prevHorzSBValue;

protected:
	afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );

private:
	typedef  QWidget  parent_type;
	virtual void resizeEvent    (QResizeEvent* pEvent);
	virtual void paintEvent     (QPaintEvent*  pEvent);
	virtual void keyPressEvent  (QKeyEvent*    pEvent);
	virtual void wheelEvent     (QWheelEvent*  pEvent);
	virtual void mousePressEvent(QMouseEvent*  pEvent);

	REF(QScrollBar) getVertScrollBar();
	REF(QScrollBar) getHorzScrollBar();

private slots:
	void onVertScrollBarValueChanged(int value);
	void onHorzScrollBarValueChanged(int value);

public:
	RDOLogCtrl(PTR(QAbstractScrollArea) pParent, PTR(RDOLogStyle) pStyle = NULL);
	virtual ~RDOLogCtrl();

	virtual void addStringToLog( const tstring logStr );
	
	rbool getFocusOnly() const { return focusOnly; }
	virtual void setFocusOnly( const rbool value ) { focusOnly = value; }

	virtual void getString( const int index, tstring& str ) const;
	virtual int getSelectedIndex() const;
	virtual void getSelected( tstring& str ) const;
	virtual rbool makeLineVisible( const int index );
	virtual void selectLine( const int index );
	virtual void copy();
	virtual void findNext()     { int res; find( res, bSearchDown, bMatchCase, bMatchWholeWord ); selectLine( res );  };
	virtual void findPrevious() { int res; find( res, !bSearchDown, bMatchCase, bMatchWholeWord ); selectLine( res ); };
	virtual void clear();
	
	virtual CREF(RDOLogStyle) getStyle() const;
	virtual void setStyle( RDOLogStyle* style, const rbool needRedraw = true );

	void setText( tstring text );

	void  setDrawLog( rbool value );
	rbool getDrawLog() const { return drawLog; };

	int getStringsCount() const { return stringsCount; };
};

}; // namespace rdoTracerLog

#endif // _RDO_STUDIO_MFC_TRACER_CTRLS_TRACER_CTRL_H_
