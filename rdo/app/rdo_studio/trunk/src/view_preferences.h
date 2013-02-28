/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/view_preferences.h
  \author    Романов Ярослав (robot.xet@gmail.com)
  \date      27.11.2012
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_VIEW_PREFERENCES_H_
#define _RDO_STUDIO_VIEW_PREFERENCES_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtWidgets/qdialog.h>
#include <QtWidgets/qcolordialog.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/projects/common/bin/rdo_studio/generated/ui_view_preferenses.h"
#include "app/rdo_studio/rdo_edit/rdoeditoredit.h"
#include "app/rdo_studio/edit_ctrls/rdobuildedit.h"
#include "app/rdo_studio/edit_ctrls/rdodebugedit.h"
#include "app/rdo_studio/edit_ctrls/rdofindedit.h"
#include "app/rdo_studio/rdo_edit/rdoeditoreditstyle.h"
#include "app/rdo_studio/rdo_edit/rdoeditorresultsstyle.h"
#include "app/rdo_studio/edit_ctrls/rdobuildeditstyle.h"
#include "app/rdo_studio/edit_ctrls/rdofindeditstyle.h"
#include "app/rdo_studio/rdo_edit/rdoeditorresults.h"
#include "app/rdo_studio/src/tracer/logger/tracer_logger_main_wnd.h"
#include "app/rdo_studio/src/frame/frame_view.h"
#include "app/rdo_studio/src/frame/frame_options_view.h"
#include "app/rdo_studio/src/tracer/chart/chart_view_style.h"
#include "app/rdo_studio/src/frame/frame_style.h"
#include "app/rdo_studio/src/tracer/chart/chart_view.h"
// --------------------------------------------------------------------------------

class ViewPreferences
	: public QDialog
	, private Ui::ViewPreferencesDialog
{
Q_OBJECT
public:
	explicit ViewPreferences(PTR(QWidget) pParent = NULL);

	void updateDialog();
	void updatePreview();
	void updateStyleTab();

private slots:
	void onOkButton();
	void onCancelButton();
	void onApplyButton();

	void onSetup(int state);
	void onCheckInFuture(int state);
	void onOpenLastProject(int state);
	void onShowFullName(int state);

	void onCodeCompUse(int state);
	void onCodeCompShowFullList(bool state);
	void onMarginFold(int state);
	void onMarginBookmark(int state);
	void onMarginLineNumber(int state);

	void onUseTabSymbol(int state);
	void onIndentAsTab(int state);
	void onAutoIndent(int state);
	void onEraseWithTab(bool state);
	void onTabSize(const QString& text);
	void onIndentSize(const QString& text);

	void onTreeWidgetItemActivated(QTreeWidgetItem* item, int column);
	void onSwitchPreviewComboBox(int index);
	void onFontSize(int index);
	void onFontType(int index);
	void onFontBold(int state);
	void onFontItalic(int state);
	void onFontUnderline(int state);
	void onHorzScroll(int state);
	void onWordWrap(int state);
	void onBookmark(int index);
	void onFold(int index);
	void onComment(int state);
	void onWarning(int state);
	void onHorzIndent(const QString& text);
	void onVertIndent(const QString& text);
	void onFgColor(int index);
	void onBgColor(int index);
	void onFgColorDialog();
	void onBgColorDialog();
	void onFgColorSelected(const QColor& color);
	void onBgColorSelected(const QColor& color);
	void onTitleSize(int index);
	void onLegendSize(int index);
	void onTickWidth(const QString& text);

private:
	enum ItemType
	{
		IT_ROOT = 0,
		IT_EDITOR,
		IT_BUILD,
		IT_DEBUG,
		IT_LOG,
		IT_RESULT,
		IT_FIND,
		IT_CHART,
		IT_FRAME,
		IT_EDITOR_PLAINTEXT,
		IT_EDITOR_IDENTIFICATOR,
		IT_EDITOR_KEYWORD,
		IT_EDITOR_FUNCTION,
		IT_EDITOR_TRACE,
		IT_EDITOR_COLOR,
		IT_EDITOR_COMMENT,
		IT_EDITOR_NUMBER,
		IT_EDITOR_STRING,
		IT_EDITOR_OPERATOR,
		IT_EDITOR_CARET,
		IT_EDITOR_TEXTSELECTION,
		IT_EDITOR_BOOKMARK,
		IT_EDITOR_FOLD,
		IT_EDITOR_ERROR,
		IT_BUILD_TEXT,
		IT_BUILD_SELECTEDLINE,
		IT_LOG_ES,
		IT_LOG_EB,
		IT_LOG_EF,
		IT_LOG_EI,
		IT_LOG_ER,
		IT_LOG_RC,
		IT_LOG_RE,
		IT_LOG_RK,
		IT_LOG_V,
		IT_LOG_STATUS,
		IT_LOG_DPS,
		IT_LOG_SB,
		IT_LOG_SO,
		IT_LOG_STN,
		IT_LOG_STD,
		IT_LOG_STR,
		IT_LOG_SRC,
		IT_LOG_SRE,
		IT_LOG_SRK,
		IT_LOG_SD,
		IT_LOG_SES,
		IT_LOG_SEN,
		IT_LOG_SEM,
		IT_LOG_SEF,
		IT_LOG_SEU,
		IT_FIND_SEARCHTEXT,
		IT_CHART_AXIS,
		IT_CHART_TITLE,
		IT_CHART_LEGEND,
		IT_CHART_CHART,
		IT_CHART_TIME,
		IT_FRAME_BORDER,
		IT_FRAME_BACKGROUND
	};

	class StyleItem;

	class StyleProperty
	{
	public:
		StyleItem* item;
		int identificator;

		rdoStyle::RDOStyleFont::style& font_style;

		QColor& fg_color;
		QColor& bg_color;
		QColor& fg_disable_color;
		QColor& bg_disable_color;

		StyleProperty(StyleItem* item, int identificator, rdoStyle::RDOStyleFont::style& font_style, QColor& fg_color, QColor& bg_color, QColor& fg_disable_color = null_fg_color, QColor& bg_disable_color = null_bg_color)
			: item(item)
			, identificator(identificator)
			, font_style(font_style)
			, fg_color(fg_color)
			, bg_color(bg_color)
			, fg_disable_color(fg_disable_color)
			, bg_disable_color(bg_disable_color)
		{}
	};

	typedef std::list<PTR(StyleProperty)> PropertyList;

	class StyleItem
	{
	public:
		ItemType                       type;
		int&                           font_size;
		tstring&                       font_name;
		rbool&                         wordwrap;
		rbool&                         horzscrollbar;
		rbool&                         warning;
		rdoEditCtrl::RDOBookmarkStyle& bookmarkstyle;
		rdoEditor::RDOFoldStyle&       foldstyle;
		rbool&                         commentfold;

		PropertyList properties;

		StyleItem(ItemType type, int& font_size, tstring& font_name, rbool& wordwrap = null_wordwrap, rbool& horzscrollbar = null_horzscrollbar, rdoEditCtrl::RDOBookmarkStyle& bookmarkstyle = null_bookmarkstyle, rdoEditor::RDOFoldStyle& foldstyle = null_foldstyle, rbool& commentfold = null_commentfold, rbool& warning = null_warning)
			: type(type)
			, font_size(font_size)
			, font_name(font_name)
			, wordwrap(wordwrap)
			, horzscrollbar(horzscrollbar)
			, warning(warning)
			, bookmarkstyle(bookmarkstyle)
			, foldstyle(foldstyle)
			, commentfold(commentfold)
		{}
	};

	typedef std::list<PTR(StyleItem)> StyleItemList;

	StyleItemList style_list;

	int         all_font_size;
	tstring     all_font_name;
	QColor      all_fg_color;
	QColor      all_bg_color;

	rdoStyle::RDOStyleFont::style        null_font_style;
	static rbool                         null_wordwrap;
	static rbool                         null_horzscrollbar;
	static rbool                         null_warning;
	static rbool                         null_commentfold;
	static rdoEditCtrl::RDOBookmarkStyle null_bookmarkstyle;
	static rdoEditor::RDOFoldStyle       null_foldstyle;
	static QColor                        null_fg_color;
	static QColor                        null_bg_color;

	rbool m_setup;
	rbool m_checkInFuture;
	rbool m_openLastProject;
	rbool m_showFullName;

	rdoEditor::RDOEditorEditStyle    style_editor;
	rdoEditCtrl::RDOBuildEditStyle   style_build;
	rdoEditCtrl::RDOBaseEditStyle    style_debug;
	rdo::gui::tracer::LogStyle       style_trace;
	rdoEditor::RDOEditorResultsStyle style_results;
	rdoEditCtrl::RDOFindEditStyle    style_find;
	rdo::gui::tracer::ChartViewStyle style_chart;
	RDOStudioFrameStyle              style_frame;

	PTR(rdoEditor::RDOEditorEdit)     preview_editor;
	PTR(rdoEditCtrl::RDOBuildEdit)    preview_build;
	PTR(rdoEditCtrl::RDODebugEdit)    preview_debug;
	PTR(rdo::gui::tracer::LogMainWnd) preview_trace;
	PTR(rdoEditor::RDOEditorResults)  preview_results;
	PTR(rdoEditCtrl::RDOFindEdit)     preview_find;

	PTR(rdo::gui::tracer::ChartDoc)     preview_chart_doc;
	PTR(rdo::gui::tracer::ChartView)    preview_chart;
	std::vector<rdo::gui::tracer::Time> preview_times;
	rdo::gui::tracer::LPSerie           preview_serie;

	PTR(FrameOptionsView)             preview_frame;

	typedef PTR(QTreeWidgetItem) treeItem;

	//Все окна
	treeItem m_pRoot;
	treeItem m_pText;
	treeItem m_pCompile;
	treeItem m_pDebug;
	treeItem m_pTrace;
	treeItem m_pResult;
	treeItem m_pSearch;
	treeItem m_pChart;
	treeItem m_pAnimation;

	//Исходный текст
	treeItem m_pPlainText;
	treeItem m_pVariable;
	treeItem m_pKeyword;
	treeItem m_pFunction;
	treeItem m_pTraceText;
	treeItem m_pColor;
	treeItem m_pComment;
	treeItem m_pNumber;
	treeItem m_pString;
	treeItem m_pOperator;
	treeItem m_pCaret;
	treeItem m_pSelection;
	treeItem m_pBookmark;
	treeItem m_pGroup;
	treeItem m_pError;

	//Окно компиляции
	treeItem m_pTextCompile;
	treeItem m_pSelectedString;
	treeItem m_pCaretCompile;
	treeItem m_pSelectionCompile;
	treeItem m_pBookmarkCompile;

	//Окно отладки
	treeItem m_pTextDebug;
	treeItem m_pCaretDebug;
	treeItem m_pSelectionDebug;
	treeItem m_pBookmarkDebug;

	//Окно трассировки
	treeItem m_pES;
	treeItem m_pEB;
	treeItem m_pEF;
	treeItem m_pEI;
	treeItem m_pER;
	treeItem m_pRC;
	treeItem m_pRE;
	treeItem m_pRK;
	treeItem m_pV;
	treeItem m_pStatus;
	treeItem m_pDPS;
	treeItem m_pSB;
	treeItem m_pSO;
	treeItem m_pSTN;
	treeItem m_pSTD;
	treeItem m_pSTR;
	treeItem m_pSRC;
	treeItem m_pSRE;
	treeItem m_pSRK;
	treeItem m_pSD;
	treeItem m_pSES;
	treeItem m_pSEN;
	treeItem m_pSEM;
	treeItem m_pSEF;
	treeItem m_pSEU;

	//Окно результатов
	treeItem m_pPlainTextResult;
	treeItem m_pVariableResult;
	treeItem m_pKeywordResult;
	treeItem m_pNumberResult;
	treeItem m_pStringResult;
	treeItem m_pOperatorResult;
	treeItem m_pCaretResult;
	treeItem m_pSelectionResult;
	treeItem m_pBookmarkResult;

	//Окно поиска
	treeItem m_pTextSearch;
	treeItem m_pStringSearch;
	treeItem m_pSelectedStringSearch;
	treeItem m_pCaretSearch;
	treeItem m_pSelectionSearch;
	treeItem m_pBookmarkSearch;

	//Окно графиков
	treeItem m_pAxis;
	treeItem m_pTitle;
	treeItem m_pLegend;
	treeItem m_pGraph;
	treeItem m_pTime;

	//Окно анимации
	treeItem m_pEdgingColor;
	treeItem m_pBackgroundColor;

	PTR(QColorDialog) fgColorDlg;
	PTR(QColorDialog) bgColorDlg;

	void createStyles();
	void createPreview();
	void createTree();
	void insertColors(QComboBox* colorBox);
	void insertColor (const QColor& color, const QString& colorName, QComboBox* colorBox);
	PTR(StyleProperty) getStyleProperty();
	PTR(StyleItem) getStyleItem();
	PTR(QTreeWidgetItem) createTreeItem (PTR(QTreeWidgetItem) parent, CREF(QString) name, ItemType itemType);
	
	void apply();
	void checkAllData();
};

#endif // _RDO_STUDIO_VIEW_PREFERENCES_H_