#ifndef RDOEDITOROPTIONS_H
#define RDOEDITOROPTIONS_H
#pragma once
/*
#include <rdocolorcombobox.h>
#include "rdotracerlogctrl.h"
#include "rdotracerlogstyle.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerLogOptions
// ----------------------------------------------------------------------------
class RDOTracerOptions;

class RDOTracerLogOptions: public CPropertyPage
{
protected:
	RDOTracerOptions* sheet;

	RDOTracerLogCtrl log;
	void onLogSelChange();

	RDOColorComboBox fgColorCB;
	RDOColorComboBox bgColorCB;

	RDOTracerLogStyle currentStyle;
	RDOTracerLogStyle defaultStyle;

	void updateFont();
	void updateButtons();
	void updateLog();

	CStringList fontList;
	static int CALLBACK EnumFontFamExProc( ENUMLOGFONTEX* lpelfe, NEWTEXTMETRICEX* lpntme, DWORD FontType, LPARAM lParam );

	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL OnNotify( WPARAM wParam, LPARAM lParam, LRESULT* pResult );

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

	afx_msg void OnResetBClick();
	afx_msg void OnDefaultBClick();

	afx_msg void OnUpdateModify();
	
	afx_msg LRESULT onLogSelChange( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

public:
	RDOTracerLogOptions( RDOTracerOptions& _sheet );
};

// ----------------------------------------------------------------------------
// ---------- RDOTracerOptions
// ----------------------------------------------------------------------------
class RDOTracerOptions: public CPropertySheet
{
friend class RDOTracerLogOptions;

protected:
	RDOTracerLogStyle prevStyle;

	RDOTracerLogOptions* logOptions;

	void apply();
	static int CALLBACK AddContextHelpProc(HWND hwnd, UINT message, LPARAM lParam);
	void onHelpButton();

	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);

	DECLARE_MESSAGE_MAP()

public:
	RDOTracerOptions( const RDOTracerLogStyle& style );
	virtual ~RDOTracerOptions();

	RDOTracerLogStyle logStyle;
};
*/
#endif // RDOEDITOROPTIONS_H
