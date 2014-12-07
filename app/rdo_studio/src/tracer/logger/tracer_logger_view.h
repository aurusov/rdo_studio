#ifndef _RDO_STUDIO_TRACER_LOGGER_VIEW_H_
#define _RDO_STUDIO_TRACER_LOGGER_VIEW_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QWidget>
#include <QEvent>
#include <QAbstractScrollArea>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/tracer/logger/tracer_logger_view_style.h"
#include "app/rdo_studio/src/action_activator/action_activator_widget.h"
#include "app/rdo_studio/src/dialog/find_dialog.h"
#include "ui/qt/headers/scroll_bar/scroll_bar.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace tracer {

// --------------------------------------------------------------------------------
// -------------------- LogView
// --------------------------------------------------------------------------------
class LogView: public ActionActivatorWidget
{
Q_OBJECT

public:
	LogView(QAbstractScrollArea* pParent, LogStyle* pStyle);
	virtual ~LogView();

	void push_back(const std::string& log);
	void setText(std::string text);
	void clear();
	void selectLine(int index);

	bool getFocusOnly() const;
	void setFocusOnly(bool value);

	const LogStyle& getStyle() const;
	void setStyle(LogStyle* style, bool needRedraw = true);

	bool getDrawLog() const;
	void setDrawLog(bool value);

private:
	typedef  ActionActivatorWidget  parent_type;

	class StringList
	{
	public:
		typedef  std::list<QString>            List;
		typedef  List::const_iterator          const_iterator;
		typedef  List::const_reverse_iterator  const_reverse_iterator;

		StringList();

		void push_back(const QString& value);

		const_iterator begin() const;
		const_iterator end  () const;

		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend  () const;

		void clear();
		int count() const;
		int maxLegth() const;
		void setCursor(int pos, int max);

		const_iterator findString(int index) const;
		const_reverse_iterator rFindString(int index) const;

	private:
		List m_list;
		int m_count;
		List::iterator m_cursorIt;
		int m_cursor;
		int m_maxLegth;
	};

	struct SubitemColors
	{
		typedef std::map<int, LogColorPair> List;

		List         m_colorList;
		bool         m_addingSubitems;
		LogColorPair m_parentColor;

		SubitemColors();
		SubitemColors(const SubitemColors& subitemColors);
	};

	struct ScrollMetricVert: public ScrollMetric
	{
		int lastViewableLine;

		ScrollMetricVert();
		bool isVisible(int index) const;
		bool applyInc(int delta);
	};

	QAbstractScrollArea* m_pScrollArea;
	StringList m_strings;
	LogStyle* m_logStyle;
	int m_lineHeight;
	int m_charWidth;
	int m_selectedLine;
	QMenu* m_pPopupMenu;
	ScrollMetric m_SM_X;
	ScrollMetricVert m_SM_Y;
	SubitemColors m_subitemColors;
	QRect m_clientRect;
	QRect m_prevWindowRect;
	int m_fullRepaintLines;
	bool m_focusOnly;
	FindDialog* m_pFindDialog;
	FindDialog::Settings m_findSettings;
	bool m_drawLog;
	QFont m_font;

	bool getItemColors(const QString& item, LogColorPair &colors) const;
	bool getItemColors(int index, LogColorPair &colors) const;

	int selectedLine() const;
	void setSelectedLine(int selectedLine);

	QString     getString           (int index) const;
	QString     getSelected         () const;

	QScrollBar& getVertScrollBar    ();
	QScrollBar& getHorzScrollBar    ();
	void        updateScrollBars    ();
	bool        scrollVertically    (int inc);
	bool        scrollHorizontally  (int inc);

	bool        makeLineVisible     (int index);
	bool        isFullyVisible      (int index) const;
	QRect       getLineRect         (int index) const;
	void        repaintLine         (int index);

	void        setFont             ();
	void        updateActionFind    (bool activated);
	void        updateActionEditCopy(bool activated);
	void        updateCoordStatusBar(bool activated);
	bool        canCopy             () const;

	int find(bool searchDown);
	void onFindDlgFind(const FindDialog::Settings& settings);
	void onFindDlgClose();

	virtual void resizeEvent     (QResizeEvent* pEvent);
	virtual void paintEvent      (QPaintEvent*  pEvent);
	virtual void keyPressEvent   (QKeyEvent*    pEvent);
	virtual void wheelEvent      (QWheelEvent*  pEvent);
	virtual void mousePressEvent (QMouseEvent*  pEvent);
	virtual void contextMenuEvent(QContextMenuEvent* pEvent);

	virtual void onUpdateActions(bool activated);

private slots:
	void onVertScrollBarValueChanged(int value);
	void onHorzScrollBarValueChanged(int value);
	void onEditCopy                 ();
	void onSearchFind               ();
	void onSearchFindNext           ();
	void onSearchFindPrevious       ();
	void onHelpContext              ();
};

}}} // namespace rdo::gui::tracer

#endif // _RDO_STUDIO_TRACER_LOGGER_VIEW_H_
