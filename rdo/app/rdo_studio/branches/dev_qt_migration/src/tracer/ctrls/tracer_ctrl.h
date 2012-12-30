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

private:
	class StringList
	{
	public:
		typedef  std::list<tstring>            List;
		typedef  List::iterator                iterator;
		typedef  List::const_iterator          const_iterator;
		typedef  List::reverse_iterator        reverse_iterator;
		typedef  List::const_reverse_iterator  const_reverse_iterator;

		StringList()
			: m_count   (0)
			, m_cursor  (0)
			, m_cursorIt(m_list.end())
		{}

		void push_back(CREF(tstring) value)
		{
			m_list.push_back(value);
			++m_count;

			if (m_count == 1)
			{
				m_cursorIt = m_list.begin();
			}
		}

		iterator begin()
		{
			return m_list.end();
		}

		const_iterator begin() const
		{
			return const_cast<StringList*>(this)->begin();
		}

		const_reverse_iterator rbegin() const
		{
			return m_list.rbegin();
		}

		reverse_iterator rbegin()
		{
			return m_list.rbegin();
		}

		const_reverse_iterator rend() const
		{
			return m_list.rend();
		}

		reverse_iterator rend()
		{
			return m_list.rend();
		}

		iterator end()
		{
			return m_list.end();
		}

		const_iterator end() const
		{
			return const_cast<StringList*>(this)->end();
		}

		void clear()
		{
			m_list.clear();
			m_count = 0;
		}

		rsint count() const
		{
			return m_count;
		}

		void setCursor(rsint pos, rsint max)
		{
			if (pos == m_cursor)
				return;

			if (pos == 0)
			{
				m_cursorIt = m_list.begin();
			}
			else if (pos == max)
			{
				m_cursorIt = m_list.end();
				for (int i = m_count; i > max; --i)
				{
					--m_cursorIt;
				}
			}
			else
			{
				int delta = pos - m_cursor;
				seek(delta, m_cursorIt);
			}
			m_cursor = pos;
		}

		iterator findString(int index)
		{
			iterator res;

			if (index == 0)
			{
				res = m_list.begin();
			}
			else if (index == m_cursor)
			{
				res = m_cursorIt;
			}
			else if (index == m_count - 1)
			{
				res = m_list.end();
				--res;
			}
			else
			{
				int deltaPos = index - m_cursor;
				int deltaEnd = index - (m_count - 1);
				int mod_deltaPos = deltaPos >= 0 ? deltaPos : -1 * deltaPos;
				int mod_deltaEnd = deltaEnd >= 0 ? deltaEnd : -1 * deltaEnd;
				int delta = (std::min)(index, mod_deltaPos);
				delta = (std::min)(delta, mod_deltaEnd);
				if (delta == index)
				{
					res = m_list.begin();
				}
				else if (delta == mod_deltaPos)
				{
					res = m_cursorIt;
					delta = deltaPos;
				}
				else if (delta == mod_deltaEnd)
				{
					res = m_list.end();
					--res;
					delta = deltaEnd;
				}
				seek(delta, res);
			}

			return res;
		}

		const_iterator findString(int index) const
		{
			return const_cast<StringList*>(this)->findString(index);
		}

		reverse_iterator rFindString(int index)
		{
			reverse_iterator rit(findString(index));
			return rit;
		}

	private:
		List            m_list;
		rsint           m_count;
		List::iterator  m_cursorIt;
		rsint           m_cursor;

		void seek(rsint delta, REF(StringList::const_iterator) it) const
		{
			ASSERT(it != m_list.end());

			if (delta > 0)
			{
				for (int i = 0; i < delta; i++)
				{
					++it;
				}
			}
			else
			{
				for (int i = delta; i < 0; i++)
				{
					--it;
				}
			}
		}
	};

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

	StringList strings;

	int   firstFoundLine;
	int   posFind;
	rbool bHaveFound;
	rbool bSearchDown;
	rbool bMatchCase;
	rbool bMatchWholeWord;
	tstring findStr;
	void find(int& result, rbool searchDown, rbool matchCase, rbool matchWholeWord);

	RDOLogStyle*  logStyle;
	virtual rbool getItemColors(int index, RDOLogColorPair* &colors) const;
	virtual rbool getItemColors(CREF(tstring) item, RDOLogColorPair* &colors) const;
	
	void  recalcWidth(int newMaxStrWidth);
	void  updateScrollBars();

	rbool scrollVertically  (int pos);
	rbool scrollHorizontally(int pos);
	
	rbool isVisible(int index) const;
	rbool isFullyVisible(int index) const;

	QRect getLineRect(int index) const;
	void  repaintLine (int index);

	rbool canCopy() const { return selectedLine != -1; };

	void  updateWindow();

	rbool drawLog;

	QFont m_font;
	void setFont();

	PTR(QAbstractScrollArea) m_pScrollArea;
	int m_prevVertSBValue;
	int m_prevHorzSBValue;

protected:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	typedef  QWidget  parent_type;
	virtual void resizeEvent    (QResizeEvent* pEvent);
	virtual void paintEvent     (QPaintEvent*  pEvent);
	virtual void keyPressEvent  (QKeyEvent*    pEvent);
	virtual void wheelEvent     (QWheelEvent*  pEvent);
	virtual void mousePressEvent(QMouseEvent*  pEvent);

	REF(QScrollBar) getVertScrollBar();
	REF(QScrollBar) getHorzScrollBar();

	void seek(rsint delta, REF(StringList::const_iterator) it) const;

private slots:
	void onVertScrollBarValueChanged(int value);
	void onHorzScrollBarValueChanged(int value);

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
};

}; // namespace rdoTracerLog

#endif // _RDO_STUDIO_MFC_TRACER_CTRLS_TRACER_CTRL_H_
