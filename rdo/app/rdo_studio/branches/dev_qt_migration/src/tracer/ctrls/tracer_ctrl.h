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
class RDOLogCtrl: public QWidget
{
Q_OBJECT

friend class RDOLogCtrlFindInList;

public:
	RDOLogCtrl(PTR(QAbstractScrollArea) pParent, PTR(RDOLogStyle) pStyle = NULL);
	virtual ~RDOLogCtrl();

	virtual void addStringToLog(CREF(tstring) logStr);
	
	rbool getFocusOnly() const { return focusOnly; }
	virtual void setFocusOnly(rbool value) { focusOnly = value; }

	virtual void getString(int index, tstring& str) const;
	virtual int getSelectedIndex() const;
	virtual void getSelected(tstring& str) const;
	virtual rbool makeLineVisible(int index);
	virtual void selectLine(int index);
	virtual void copy();
	virtual void findNext()     { int res; find(res, bSearchDown, bMatchCase, bMatchWholeWord); selectLine(res);  };
	virtual void findPrevious() { int res; find(res, !bSearchDown, bMatchCase, bMatchWholeWord); selectLine(res); };
	virtual void clear();
	
	virtual CREF(RDOLogStyle) getStyle() const;
	virtual void setStyle(RDOLogStyle* style, rbool needRedraw = true);

	void setText(tstring text);

	void  setDrawLog(rbool value);
	rbool getDrawLog() const { return drawLog; };

protected:
	class StringList
	{
	public:
		typedef  std::list<tstring>            List;
		typedef  List::iterator                iterator;
		typedef  List::const_iterator          const_iterator;
		typedef  List::reverse_iterator        reverse_iterator;
		typedef  List::const_reverse_iterator  const_reverse_iterator;

		StringList();

		void push_back(CREF(tstring) value);

		const_iterator begin() const;
		iterator       begin();
		const_iterator end  () const;
		iterator       end  ();

		const_reverse_iterator rbegin() const;
		reverse_iterator       rbegin();
		const_reverse_iterator rend  () const;
		reverse_iterator       rend  ();

		void               clear    ();
		rsint              count    () const;
		tstring::size_type maxLegth () const;
		void               setCursor(rsint pos, rsint max);

		const_iterator   findString (int index) const;
		iterator         findString (int index);
		reverse_iterator rFindString(int index);

	private:
		List                m_list;
		rsint               m_count;
		List::iterator      m_cursorIt;
		rsint               m_cursor;
		tstring::size_type  m_maxLegth;

		void seek(rsint delta, REF(StringList::const_iterator) it) const;
	};

	CMutex     mutex;
	StringList m_strings;

	int selectedLine;

	int     firstFoundLine;
	rbool   bHaveFound;
	tstring findStr;

	RDOLogStyle*  logStyle;
	virtual rbool getItemColors(CREF(tstring) item, RDOLogColorPair* &colors) const;

	rbool canCopy() const { return selectedLine != -1; };

private:
	int lineHeight;
	int charWidth;

	struct ScrollMetric
	{
		rsint position;
		rsint posMax;
		rsint pageSize;

		ScrollMetric();

		rbool applyInc(rsint delta);
	};

	struct ScrollMetricVert: public ScrollMetric
	{
		rsint lastViewableLine;

		ScrollMetricVert();
		rbool isVisible(rsint index) const;
		rbool applyInc (rsint delta);
	};

	ScrollMetric      m_SM_X;
	ScrollMetricVert  m_SM_Y;

	QRect m_clientRect;
	QRect m_prevWindowRect;

	int fullRepaintLines;
	rbool focusOnly;

	int   posFind;
	rbool bSearchDown;
	rbool bMatchCase;
	rbool bMatchWholeWord;
	void find(int& result, rbool searchDown, rbool matchCase, rbool matchWholeWord);

	virtual rbool getItemColors(int index, RDOLogColorPair* &colors) const;
	
	void  updateScrollBars();

	rbool scrollVertically  (int pos);
	rbool scrollHorizontally(int pos);
	
	rbool isFullyVisible(int index) const;

	QRect getLineRect(int index) const;
	void  repaintLine (int index);

	void  updateWindow();

	rbool drawLog;

	QFont m_font;
	void setFont();

	PTR(QAbstractScrollArea) m_pScrollArea;

	typedef  QWidget  parent_type;
	virtual void resizeEvent    (QResizeEvent* pEvent);
	virtual void paintEvent     (QPaintEvent*  pEvent);
	virtual void focusInEvent   (QFocusEvent*  pEvent);
	virtual void focusOutEvent  (QFocusEvent*  pEvent);
	virtual void keyPressEvent  (QKeyEvent*    pEvent);
	virtual void wheelEvent     (QWheelEvent*  pEvent);
	virtual void mousePressEvent(QMouseEvent*  pEvent);

	REF(QScrollBar) getVertScrollBar();
	REF(QScrollBar) getHorzScrollBar();

private slots:
	void onVertScrollBarValueChanged(int value);
	void onHorzScrollBarValueChanged(int value);
};

}; // namespace rdoTracerLog

#endif // _RDO_STUDIO_MFC_TRACER_CTRLS_TRACER_CTRL_H_
