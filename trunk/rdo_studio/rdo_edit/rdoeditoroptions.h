#ifndef RDOEDITOROPTIONS_H
#define RDOEDITOROPTIONS_H
#pragma once

/*
#include <rdocolorcombobox.h>

#include "rdoeditoredit.h"
#include "rdoeditoreditstyle.h"
#include "rdoeditormainfrm.h"

// ----------------------------------------------------------------------------
// ---------- RDOEditorEditorOptions
// ----------------------------------------------------------------------------
class RDOEditorOptions;

class RDOEditorEditorOptions: public CPropertyPage
{
protected:
	RDOEditorOptions* sheet;

	int tabSize;
	int indentSize;
	int useTabs;
	int tabIndents;
	int backspaceUntabs;
	int autoIndent;

	int debug;
	int prev_debug;

	int canClearBuffer;
	int clearBufferDelay;
	int prev_canClearBuffer;
	int prev_clearBufferDelay;

	int wordWrap;
	int showHorzScrollBar;

	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );

	afx_msg void OnUseAutoClearBufferCBChanged();
	afx_msg void OnUseWordWrapCBChanged();
	afx_msg void OnUpdateModify();

	DECLARE_MESSAGE_MAP()

public:
	RDOEditorEditorOptions( RDOEditorOptions& _sheet, RDODebug _debug, bool _canClearBuffer, int _clearBufferDelay );
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorColorsOptions
// ----------------------------------------------------------------------------
class RDOEditorColorsOptions: public CPropertyPage
{
friend class RDOEditorOptions;
friend class RDOEditorEditorOptions;

protected:
	RDOEditorOptions* sheet;

	RDOEditorEdit edit;

	RDOEditorEditTheme currentTheme;
	RDOEditorEditTheme defaultTheme;
	RDOEditorEditTheme cppTheme;
	RDOEditorEditTheme pascalTheme;
	RDOEditorEditTheme htmlTheme;
	RDOEditorEditTheme classicTheme;
	RDOEditorEditTheme twilightTheme;
	RDOEditorEditTheme oceanTheme;

	RDOColorComboBox fgColorCB;
	RDOColorComboBox bgColorCB;

	void updateBookmarkStyle() const;
	void updateFoldStyle() const;
	void updateEdit();

	CStringList fontList;
	static int CALLBACK EnumFontFamExProc( ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* lpntme, DWORD FontType, LPARAM lParam );

	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );

	afx_msg void OnSize( UINT nType, int cx, int cy );

	afx_msg void OnThemeCBChanged();
	afx_msg void OnStyleLBChanged();
	afx_msg void OnFontNameCBChanged();
	afx_msg void OnFontSizeCBChanged();

	afx_msg void OnFontBoldBClick();
	afx_msg void OnFontItalicBClick();
	afx_msg void OnFontUnderlineBClick();

	afx_msg void OnFgColorCBChanged();
	afx_msg void OnBgColorCBChanged();
	afx_msg void OnFgColorBClick();
	afx_msg void OnBgColorBClick();

	afx_msg void OnBookmarkStyleCBChanged();
	afx_msg void OnFoldStyleCBChanged();

	DECLARE_MESSAGE_MAP()

public:
	RDOEditorColorsOptions( RDOEditorOptions& _sheet );
};

// ----------------------------------------------------------------------------
// ---------- RDOEditorOptions
// ----------------------------------------------------------------------------
class RDOEditorOptions: public CPropertySheet
{
friend class RDOEditorEditorOptions;
friend class RDOEditorColorsOptions;

protected:
	RDOEditorEditStyle prevStyle;

	RDOEditorEditorOptions* editorOptions;
	RDOEditorColorsOptions* colorOptions;

	void apply();
	static int CALLBACK AddContextHelpProc(HWND hwnd, UINT message, LPARAM lParam);
	void onHelpButton();

	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

	DECLARE_MESSAGE_MAP()

public:
	RDOEditorOptions( const RDOEditorEditStyle& style );
	virtual ~RDOEditorOptions();

	RDOEditorEditStyle editorStyle;
};
*/
#endif // RDOEDITOROPTIONS_H
