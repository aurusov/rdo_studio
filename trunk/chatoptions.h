#ifndef CHATOPTIONS_H
#define CHATOPTIONS_H
#pragma once

#include "chatlistboxctrl.h"
#include "rdocolorcombobox.h"
#include "chatsound.h"
#include "chatstatusmode.h"

// ----------------------------------------------------------------------------
// ---------- CChatGeneralOptions
// ----------------------------------------------------------------------------
class CChatGeneralOptions: public CPropertyPage
{
protected:
	CString userName;

	int useTray;
	int notifyAboutMessage;

	int closeButtonAction;
	int minimizeButtonAction;

	int usePopup;
	int popupPosition;
	int popupDelay;

	CEdit edit;
	CSpinButtonCtrl spin;
	RDOColorComboBox fgColorCB;
	RDOColorComboBox bgColorCB;

	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnUseTrayCBChanged();

	afx_msg void OnUsePopupCBChanged();
	afx_msg void OnFgColorCBChanged();
	afx_msg void OnBgColorCBChanged();
	afx_msg void OnFgColorBClick();
	afx_msg void OnBgColorBClick();

	afx_msg void OnUpdateModify();

	DECLARE_MESSAGE_MAP()

public:
	CChatGeneralOptions();
};

// ----------------------------------------------------------------------------
// ---------- CChatSoundOptions
// ----------------------------------------------------------------------------
class CChatSoundOptions: public CPropertyPage
{
protected:
	int useSound;

	CImageList       imageList;
	CChatListBoxCtrl listBox;
	CChatSoundList   sounds;

	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnListBoxSelectChange();
	afx_msg void OnListBoxDoubleClick();
	afx_msg void OnRadioChanged();
	afx_msg void OnEditChanged();
	afx_msg void OnButtonClicked();
	afx_msg void OnUpdateModify();

	DECLARE_MESSAGE_MAP()

public:
	CChatSoundOptions();
	virtual ~CChatSoundOptions();
};

// ----------------------------------------------------------------------------
// ---------- CChatStatusModeOptions
// ----------------------------------------------------------------------------
class CChatStatusModeOptions: public CPropertyPage
{
protected:
	CImageList          imageList;
	CChatListBoxCtrl    listBox;
	CChatStatusModeList statusModes;

	int useAutoAway;
	int autoAwayMinuts;
	int useAutoNotAvailible;
	int autoNotAvailibleMinuts;

	CEdit           autoAwayEdit;
	CSpinButtonCtrl autoAwaySpin;
	CEdit           autoNotAvailibaleEdit;
	CSpinButtonCtrl autoNotAvailibaleSpin;

	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	afx_msg void OnListBoxSelectChange();
	afx_msg void OnNameChanged();
	afx_msg void OnInfoChanged();
	afx_msg void OnAutoAwayChanged();
	afx_msg void OnAutoNotAvailibaleChanged();
	afx_msg void OnUpdateModify();

	DECLARE_MESSAGE_MAP()

public:
	CChatStatusModeOptions();
	virtual ~CChatStatusModeOptions();
};

// ----------------------------------------------------------------------------
// ---------- CChatOptions
// ----------------------------------------------------------------------------
class CChatOptions: public CPropertySheet
{
protected:
	CChatGeneralOptions*    generalOptions;
	CChatSoundOptions*      soundOptions;
	CChatStatusModeOptions* statusModeOptions;

public:
	CChatOptions();
	virtual ~CChatOptions();
};

#endif // CHATOPTIONS_H
