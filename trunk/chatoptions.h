#ifndef CHATOPTIONS_H
#define CHATOPTIONS_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "chatlistboxctrl.h"
#include "chatsound.h"
#include "chatstatusmode.h"
#include <rdocolorcombobox.h>

// ----------------------------------------------------------------------------
// ---------- CChatGeneralOptions
// ----------------------------------------------------------------------------
class CChatGeneralOptions: public CPropertyPage
{
private:
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

public:
	CChatGeneralOptions();

protected:
	//{{AFX_VIRTUAL(CChatGeneralOptions)
	protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	public:
	virtual void OnOK();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatGeneralOptions)
	afx_msg void OnUseTrayCBChanged();
	afx_msg void OnUsePopupCBChanged();
	afx_msg void OnFgColorCBChanged();
	afx_msg void OnBgColorCBChanged();
	afx_msg void OnFgColorBClick();
	afx_msg void OnBgColorBClick();
	afx_msg void OnUpdateModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ----------------------------------------------------------------------------
// ---------- CChatSoundOptions
// ----------------------------------------------------------------------------
class CChatSoundOptions: public CPropertyPage
{
private:
	int useSound;

	CImageList       imageList;
	CChatListBoxCtrl listBox;
	CChatSoundList   sounds;

public:
	CChatSoundOptions();
	virtual ~CChatSoundOptions();

protected:
	//{{AFX_VIRTUAL(CChatSoundOptions)
	protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	public:
	virtual void OnOK();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatSoundOptions)
	afx_msg void OnListBoxSelectChange();
	afx_msg void OnListBoxDoubleClick();
	afx_msg void OnRadioChanged();
	afx_msg void OnEditChanged();
	afx_msg void OnButtonClicked();
	afx_msg void OnUpdateModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ----------------------------------------------------------------------------
// ---------- CChatStatusModeOptions
// ----------------------------------------------------------------------------
class CChatStatusModeOptions: public CPropertyPage
{
private:
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

public:
	CChatStatusModeOptions();
	virtual ~CChatStatusModeOptions();

protected:
	//{{AFX_VIRTUAL(CChatStatusModeOptions)
	protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual BOOL OnInitDialog();
	public:
	virtual void OnOK();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatStatusModeOptions)
	afx_msg void OnListBoxSelectChange();
	afx_msg void OnNameChanged();
	afx_msg void OnInfoChanged();
	afx_msg void OnAutoAwayChanged();
	afx_msg void OnAutoNotAvailibaleChanged();
	afx_msg void OnUpdateModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ----------------------------------------------------------------------------
// ---------- CChatOptions
// ----------------------------------------------------------------------------
class CChatOptions: public CPropertySheet
{
private:
	CChatGeneralOptions*    generalOptions;
	CChatSoundOptions*      soundOptions;
	CChatStatusModeOptions* statusModeOptions;

public:
	CChatOptions();
	virtual ~CChatOptions();

protected:
	//{{AFX_VIRTUAL(CChatOptions)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChatOptions)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // CHATOPTIONS_H
