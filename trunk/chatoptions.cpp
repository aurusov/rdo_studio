#include "stdafx.h"
#include "chatoptions.h"
#include "chatapp.h"
#include "chatmainfrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ----------------------------------------------------------------------------
// ---------- CChatGeneralOptions
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatGeneralOptions, CPropertyPage )
	//{{AFX_MSG_MAP(CChatGeneralOptions)
	ON_EN_CHANGE( IDC_USERNAME_EDIT, OnUpdateModify )
	ON_BN_CLICKED( IDC_TRAYUSE_CHECK   , OnUseTrayCBChanged )
	ON_BN_CLICKED( IDC_TRAYNOTIFY_CHECK, OnUpdateModify )
	ON_BN_CLICKED( IDC_CLOSEBUTTON_CLOSE_RADIO   , OnUpdateModify )
	ON_BN_CLICKED( IDC_CLOSEBUTTON_MINIMIZE_RADIO, OnUpdateModify )
	ON_BN_CLICKED( IDC_CLOSEBUTTON_TRAY_RADIO    , OnUpdateModify )
	ON_BN_CLICKED( IDC_MINIMIZEBUTTON_MINIMIZE_RADIO, OnUpdateModify )
	ON_BN_CLICKED( IDC_MINIMIZEBUTTON_TRAY_RADIO    , OnUpdateModify )
	ON_BN_CLICKED(    IDC_POPUPUSE_CHECK             , OnUsePopupCBChanged )
	ON_CBN_SELCHANGE( IDC_POPUPPOSITION_COMBO        , OnUpdateModify )
	ON_EN_CHANGE(     IDC_POPUPDELAY_EDIT            , OnUpdateModify )
	ON_CBN_SELCHANGE( IDC_POPUPTEXTCOLOR_COMBO       , OnFgColorCBChanged )
	ON_CBN_SELCHANGE( IDC_POPUPBACKGROUNDCOLOR_COMBO , OnBgColorCBChanged )
	ON_BN_CLICKED(    IDC_POPUPTEXTCOLOR_BUTTON      , OnFgColorBClick )
	ON_BN_CLICKED(    IDC_POPUPBACKGROUNDCOLOR_BUTTON, OnBgColorBClick )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatGeneralOptions::CChatGeneralOptions(): CPropertyPage( IDD_OPTIONS_GENERAL_DIALOG )
{
	userName = chatApp.getUserName().c_str();

	useTray            = chatApp.mainFrame->getUseTray() ? 1 : 0;
	notifyAboutMessage = chatApp.mainFrame->getNotifyAboutMessage() ? 1 : 0;

	closeButtonAction    = chatApp.mainFrame->getCloseButtonAction();
	minimizeButtonAction = chatApp.mainFrame->getMinimizeButtonAction();

	usePopup      = chatApp.mainFrame->popupMessage.getUsePopup() ? 1 : 0;
	popupPosition = chatApp.mainFrame->popupMessage.getPosition();
	popupDelay    = chatApp.mainFrame->popupMessage.getTimerDelay();
}

void CChatGeneralOptions::DoDataExchange( CDataExchange* pDX )
{
	CPropertyPage::DoDataExchange( pDX );

	DDX_Text( pDX, IDC_USERNAME_EDIT, userName );

	DDX_Check( pDX, IDC_TRAYUSE_CHECK, useTray );
	DDX_Check( pDX, IDC_TRAYNOTIFY_CHECK, notifyAboutMessage );

	DDX_Radio( pDX, IDC_CLOSEBUTTON_CLOSE_RADIO, closeButtonAction );
	DDX_Radio( pDX, IDC_MINIMIZEBUTTON_MINIMIZE_RADIO, minimizeButtonAction );

	DDX_Check( pDX, IDC_POPUPUSE_CHECK, usePopup );
	DDX_CBIndex( pDX, IDC_POPUPPOSITION_COMBO, popupPosition );
	DDX_Text( pDX, IDC_POPUPDELAY_EDIT, popupDelay );
	DDX_Control( pDX, IDC_POPUPDELAY_EDIT, edit );
	DDX_Control( pDX, IDC_POPUPDELAY_SPIN, spin );
	DDX_Control( pDX, IDC_POPUPTEXTCOLOR_COMBO, fgColorCB );
	DDX_Control( pDX, IDC_POPUPBACKGROUNDCOLOR_COMBO, bgColorCB );
}

BOOL CChatGeneralOptions::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	spin.SetBuddy( &edit );
	spin.SetRange( 1, 99 );
	edit.SetLimitText( 2 );
	int itemHeight = static_cast<CComboBox*>(GetDlgItem( IDC_POPUPPOSITION_COMBO ))->GetItemHeight( -1 );
	fgColorCB.setItemHeight( itemHeight );
	bgColorCB.setItemHeight( itemHeight );
	fgColorCB.insertBaseColors();
	bgColorCB.insertBaseColors();

	fgColorCB.insertColor( chatApp.mainFrame->popupMessage.getTextColor() );
	bgColorCB.insertColor( chatApp.mainFrame->popupMessage.getBackgroundColor() );

	OnUseTrayCBChanged();
	OnUsePopupCBChanged();

	return true;
}

void CChatGeneralOptions::OnOK()
{
	chatApp.setUserName( static_cast<LPCTSTR>(userName) );

	chatApp.mainFrame->setUseTray( useTray ? true : false );
	chatApp.mainFrame->setNotifyAboutMessage( notifyAboutMessage ? true : false );
	chatApp.mainFrame->setCloseButtonAction( (CChatCloseButtonAction)closeButtonAction );
	chatApp.mainFrame->setMinimizeButtonAction( (CChatMinimizeButtonAction)minimizeButtonAction );

	chatApp.mainFrame->popupMessage.setUsePopup( usePopup ? true : false );
	chatApp.mainFrame->popupMessage.setPosition( (CChatPopupPosition)popupPosition );
	chatApp.mainFrame->popupMessage.setTimerDelay( popupDelay );
	chatApp.mainFrame->popupMessage.setTextColor( fgColorCB.getCurrentColor() );
	chatApp.mainFrame->popupMessage.setBackgroundColor( bgColorCB.getCurrentColor() );

	CPropertyPage::OnOK();
}

void CChatGeneralOptions::OnUseTrayCBChanged()
{
	bool use = static_cast<CButton*>(GetDlgItem( IDC_TRAYUSE_CHECK ))->GetCheck() ? true : false;
	if ( !use ) {
		if ( static_cast<CButton*>(GetDlgItem( IDC_CLOSEBUTTON_TRAY_RADIO ))->GetCheck() ) {
			static_cast<CButton*>(GetDlgItem( IDC_CLOSEBUTTON_TRAY_RADIO ))->SetCheck( false );
			static_cast<CButton*>(GetDlgItem( IDC_CLOSEBUTTON_CLOSE_RADIO ))->SetCheck( true );
		}
		if ( static_cast<CButton*>(GetDlgItem( IDC_MINIMIZEBUTTON_TRAY_RADIO ))->GetCheck() ) {
			static_cast<CButton*>(GetDlgItem( IDC_MINIMIZEBUTTON_TRAY_RADIO ))->SetCheck( false );
			static_cast<CButton*>(GetDlgItem( IDC_MINIMIZEBUTTON_MINIMIZE_RADIO ))->SetCheck( true );
		}
	}
	GetDlgItem( IDC_TRAYNOTIFY_CHECK )->EnableWindow( use );
	GetDlgItem( IDC_CLOSEBUTTON_TRAY_RADIO )->EnableWindow( use );
	GetDlgItem( IDC_MINIMIZEBUTTON_TRAY_RADIO )->EnableWindow( use );
	OnUpdateModify();
}

void CChatGeneralOptions::OnUsePopupCBChanged()
{
	bool use = static_cast<CButton*>(GetDlgItem( IDC_POPUPUSE_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_POPUPPOSITION_STATIC )->EnableWindow( use );
	GetDlgItem( IDC_POPUPPOSITION_COMBO )->EnableWindow( use );
	GetDlgItem( IDC_POPUPDELAY_STATIC )->EnableWindow( use );
	GetDlgItem( IDC_POPUPDELAY_EDIT )->EnableWindow( use );
	GetDlgItem( IDC_POPUPDELAY_SPIN )->EnableWindow( use );
	GetDlgItem( IDC_POPUPTEXTCOLOR_STATIC )->EnableWindow( use );
	GetDlgItem( IDC_POPUPTEXTCOLOR_COMBO )->EnableWindow( use );
	GetDlgItem( IDC_POPUPTEXTCOLOR_BUTTON )->EnableWindow( use );
	GetDlgItem( IDC_POPUPBACKGROUNDCOLOR_STATIC )->EnableWindow( use );
	GetDlgItem( IDC_POPUPBACKGROUNDCOLOR_COMBO )->EnableWindow( use );
	GetDlgItem( IDC_POPUPBACKGROUNDCOLOR_BUTTON )->EnableWindow( use );
	OnUpdateModify();
}

void CChatGeneralOptions::OnFgColorCBChanged()
{
	COLORREF color = fgColorCB.getCurrentColor();
	fgColorCB.insertColor( color );
	OnUpdateModify();
}

void CChatGeneralOptions::OnBgColorCBChanged()
{
	COLORREF color = bgColorCB.getCurrentColor();
	bgColorCB.insertColor( color );
	OnUpdateModify();
}

void CChatGeneralOptions::OnFgColorBClick()
{
	CColorDialog dlg( fgColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		fgColorCB.insertColor( dlg.GetColor() );
		OnFgColorCBChanged();
	}
}

void CChatGeneralOptions::OnBgColorBClick()
{
	CColorDialog dlg( bgColorCB.getCurrentColor(), CC_FULLOPEN, this );
	if ( dlg.DoModal() == IDOK ) {
		bgColorCB.insertColor( dlg.GetColor() );
		OnBgColorCBChanged();
	}
}

void CChatGeneralOptions::OnUpdateModify()
{
	UpdateData();

	SetModified( userName != chatApp.getUserName().c_str() || useTray != ( chatApp.mainFrame->getUseTray() ? 1 : 0 ) || notifyAboutMessage != ( chatApp.mainFrame->getNotifyAboutMessage() ? 1 : 0 ) || closeButtonAction != chatApp.mainFrame->getCloseButtonAction() || minimizeButtonAction != chatApp.mainFrame->getMinimizeButtonAction() || usePopup != ( chatApp.mainFrame->popupMessage.getUsePopup() ? 1 : 0 ) || popupPosition != chatApp.mainFrame->popupMessage.getPosition() || popupDelay != chatApp.mainFrame->popupMessage.getTimerDelay() || fgColorCB.getCurrentColor() != chatApp.mainFrame->popupMessage.getTextColor() || bgColorCB.getCurrentColor() != chatApp.mainFrame->popupMessage.getBackgroundColor() );
}

// ----------------------------------------------------------------------------
// ---------- CChatSoundOptions
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatSoundOptions, CPropertyPage )
	//{{AFX_MSG_MAP(CChatSoundOptions)
	ON_BN_CLICKED( IDC_SOUNDUSE_CHECK, OnUpdateModify )
	ON_LBN_SELCHANGE( IDC_SOUND_LIST, OnListBoxSelectChange )
	ON_LBN_DBLCLK   ( IDC_SOUND_LIST, OnListBoxDoubleClick  )
	ON_BN_CLICKED( IDC_SOUNDNONE_RADIO    , OnRadioChanged )
	ON_BN_CLICKED( IDC_SOUNDDEFAULT_RADIO , OnRadioChanged )
	ON_BN_CLICKED( IDC_SOUNDEXTERNAL_RADIO, OnRadioChanged )
	ON_EN_CHANGE( IDC_SOUNDEXTERNAL_EDIT, OnEditChanged )
	ON_BN_CLICKED( IDC_SOUNDEXTERMAL_BUTTON, OnButtonClicked )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatSoundOptions::CChatSoundOptions(): CPropertyPage( IDD_OPTIONS_SOUND_DIALOG )
{
	useSound  = chatApp.sounds.getUseSound() ? 1 : 0;
	imageList.Create( 16, 16, ILC_COLORDDB | ILC_MASK, 1, 1 );
	CBitmap bmp;
	bmp.LoadBitmap( IDB_SOUND );
	imageList.Add( &bmp, RGB( 255, 0, 255 ) );
}

CChatSoundOptions::~CChatSoundOptions()
{
}

void CChatSoundOptions::DoDataExchange( CDataExchange* pDX )
{
	CPropertyPage::DoDataExchange( pDX );

	DDX_Check( pDX, IDC_SOUNDUSE_CHECK, useSound );

	DDX_Control( pDX, IDC_SOUND_LIST, listBox );
}

BOOL CChatSoundOptions::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	sounds = chatApp.sounds;

	listBox.setImageList( &imageList );
	for ( int i = 0; i < sounds.count(); i++ ) {
		CChatSound* snd = sounds[i];
		listBox.addString( sounds.getName( snd->type ), snd->useSound ? 0 : -1 );
	}
	listBox.SetCurSel( 0 );

	OnListBoxSelectChange();

	return true;
}

void CChatSoundOptions::OnOK()
{
	chatApp.sounds.setUseSound( useSound ? true : false );
	chatApp.sounds = sounds;

	CPropertyPage::OnOK();
}

void CChatSoundOptions::OnListBoxSelectChange()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatSound* snd = sounds[current];
		if ( snd ) {
			static_cast<CButton*>(GetDlgItem( IDC_SOUNDNONE_RADIO ))->SetCheck( false );
			static_cast<CButton*>(GetDlgItem( IDC_SOUNDDEFAULT_RADIO ))->SetCheck( false );
			static_cast<CButton*>(GetDlgItem( IDC_SOUNDEXTERNAL_RADIO ))->SetCheck( false );
			if ( !snd->useSound ) {
				static_cast<CButton*>(GetDlgItem( IDC_SOUNDNONE_RADIO ))->SetCheck( true );
			} else {
				if ( snd->useDefault ) {
					static_cast<CButton*>(GetDlgItem( IDC_SOUNDDEFAULT_RADIO ))->SetCheck( true );
				} else {
					static_cast<CButton*>(GetDlgItem( IDC_SOUNDEXTERNAL_RADIO ))->SetCheck( true );
				}
			}
			GetDlgItem( IDC_SOUNDEXTERNAL_EDIT )->SetWindowText( snd->file.c_str() );
		}
	}
	OnUpdateModify();
}

void CChatSoundOptions::OnListBoxDoubleClick()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatSound* snd = sounds[current];
		if ( snd ) {
			snd->useSound = !snd->useSound;
			OnListBoxSelectChange();
			listBox.setItemImage( current, snd->useSound ? 0 : -1 );
		}
	}
}

void CChatSoundOptions::OnRadioChanged()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatSound* snd = sounds[current];
		if ( snd ) {
			snd->useSound   = static_cast<CButton*>(GetDlgItem( IDC_SOUNDNONE_RADIO ))->GetCheck() ? false : true;
			snd->useDefault = static_cast<CButton*>(GetDlgItem( IDC_SOUNDDEFAULT_RADIO ))->GetCheck() ? true : false;
			listBox.setItemImage( current, snd->useSound ? 0 : -1 );
		}
	}

	OnUpdateModify();
}

void CChatSoundOptions::OnEditChanged()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatSound* snd = sounds[current];
		if ( snd ) {
			CString s = snd->file.c_str();
			GetDlgItem( IDC_SOUNDEXTERNAL_EDIT )->GetWindowText( s );
			snd->file = static_cast<LPCTSTR>(s);
			OnUpdateModify();
		}
	}
}

void CChatSoundOptions::OnButtonClicked()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatSound* snd = sounds[current];
		if ( snd ) {
			std::string filter = "WAV Files (*.wav)|*.wav|All Files (*.*)|*.*||";
			CFileDialog dlg( true, "wav", snd->file.c_str(), OFN_HIDEREADONLY, filter.c_str() );
			if ( dlg.DoModal() == IDOK ) {
				snd->file = static_cast<LPCTSTR>(dlg.GetPathName());
				GetDlgItem( IDC_SOUNDEXTERNAL_EDIT )->SetWindowText( snd->file.c_str() );
			}
		}
	}
}

void CChatSoundOptions::OnUpdateModify()
{
	UpdateData();

	bool use = static_cast<CButton*>(GetDlgItem( IDC_SOUNDUSE_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_SOUND_LIST )->EnableWindow( use );
	bool selected = use && listBox.GetCurSel() != LB_ERR;
	GetDlgItem( IDC_SOUNDNONE_RADIO )->EnableWindow( selected );
	GetDlgItem( IDC_SOUNDDEFAULT_RADIO )->EnableWindow( selected );
	GetDlgItem( IDC_SOUNDEXTERNAL_RADIO )->EnableWindow( selected );
	GetDlgItem( IDC_SOUNDEXTERNAL_EDIT )->EnableWindow( selected );
	GetDlgItem( IDC_SOUNDEXTERMAL_BUTTON )->EnableWindow( selected );
	if ( selected ) {
		bool externalFlag = static_cast<CButton*>(GetDlgItem( IDC_SOUNDEXTERNAL_RADIO ))->GetCheck() ? true : false;
		GetDlgItem( IDC_SOUNDEXTERNAL_EDIT )->EnableWindow( externalFlag );
		GetDlgItem( IDC_SOUNDEXTERMAL_BUTTON )->EnableWindow( externalFlag );
	}

	SetModified( useSound != ( chatApp.sounds.getUseSound() ? 1 : 0 ) || sounds != chatApp.sounds );
}

// ----------------------------------------------------------------------------
// ---------- CChatStatusModeOptions
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatStatusModeOptions, CPropertyPage )
	//{{AFX_MSG_MAP(CChatStatusModeOptions)
	ON_LBN_SELCHANGE( IDC_STATUSMODE_LIST, OnListBoxSelectChange )
	ON_EN_CHANGE( IDC_STATUSMODENAME_EDIT, OnNameChanged )
	ON_EN_CHANGE( IDC_STATUSMODEINFO_EDIT, OnInfoChanged )
	ON_EN_CHANGE( IDC_AUTOMATICALLYSTATUSMODE_AWAY_EDIT, OnUpdateModify )
	ON_BN_CLICKED( IDC_AUTOMATICALLYSTATUSMODE_AWAY_CHECK, OnAutoAwayChanged )
	ON_EN_CHANGE( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_EDIT, OnUpdateModify )
	ON_BN_CLICKED( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_CHECK, OnAutoNotAvailibaleChanged )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatStatusModeOptions::CChatStatusModeOptions(): CPropertyPage( IDD_OPTIONS_STATUSMODE_DIALOG )
{
	useAutoAway            = chatApp.mainFrame->getUseAutoAway() ? 1 : 0;
	autoAwayMinuts         = chatApp.mainFrame->getAutoAwayMinuts();
	useAutoNotAvailible    = chatApp.mainFrame->getUseAutoNotAvailible() ? 1 : 0;
	autoNotAvailibleMinuts = chatApp.mainFrame->getAutoNotAvailibleMinuts();

	imageList.Create( 16, 16, ILC_COLORDDB | ILC_MASK, 1, 3 );
	CBitmap bmp;
	bmp.LoadBitmap( IDB_STATUSMODE );
	imageList.Add( &bmp, RGB( 255, 0, 255 ) );
}

CChatStatusModeOptions::~CChatStatusModeOptions()
{
}

void CChatStatusModeOptions::DoDataExchange( CDataExchange* pDX )
{
	CPropertyPage::DoDataExchange( pDX );

	DDX_Control( pDX, IDC_STATUSMODE_LIST, listBox );

	DDX_Check( pDX, IDC_AUTOMATICALLYSTATUSMODE_AWAY_CHECK, useAutoAway );
	DDX_Text( pDX, IDC_AUTOMATICALLYSTATUSMODE_AWAY_EDIT, autoAwayMinuts );
	DDX_Control( pDX, IDC_AUTOMATICALLYSTATUSMODE_AWAY_EDIT, autoAwayEdit );
	DDX_Control( pDX, IDC_AUTOMATICALLYSTATUSMODE_AWAY_SPIN, autoAwaySpin );

	DDX_Check( pDX, IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_CHECK, useAutoNotAvailible );
	DDX_Text( pDX, IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_EDIT, autoNotAvailibleMinuts );
	DDX_Control( pDX, IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_EDIT, autoNotAvailibaleEdit );
	DDX_Control( pDX, IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_SPIN, autoNotAvailibaleSpin );
}

BOOL CChatStatusModeOptions::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	autoAwaySpin.SetBuddy( &autoAwayEdit );
	autoAwaySpin.SetRange( 1, 99 );
	autoAwayEdit.SetLimitText( 2 );

	autoNotAvailibaleSpin.SetBuddy( &autoNotAvailibaleEdit );
	autoNotAvailibaleSpin.SetRange( 1, 99 );
	autoNotAvailibaleEdit.SetLimitText( 2 );

	statusModes = chatApp.statusModes;

	listBox.setImageList( &imageList );
	for ( int i = 0; i < statusModes.count(); i++ ) {
		CChatStatusMode* statusMode = statusModes[i];
		listBox.addString( statusModes.getDefaultName( statusMode->type ), statusMode->type );
	}
	listBox.SetCurSel( 0 );

	OnListBoxSelectChange();
	OnAutoAwayChanged();

	return true;
}

void CChatStatusModeOptions::OnOK()
{
	chatApp.statusModes = statusModes;

	chatApp.mainFrame->setUseAutoAway( useAutoAway ? true : false );
	chatApp.mainFrame->setAutoAwayMinuts( autoAwayMinuts );
	chatApp.mainFrame->setUseAutoNotAvailible( useAutoNotAvailible ? true : false );
	chatApp.mainFrame->setAutoNotAvailibleMinuts( autoNotAvailibleMinuts );

	CPropertyPage::OnOK();
}

void CChatStatusModeOptions::OnListBoxSelectChange()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatStatusMode* statusMode = statusModes[current];
		if ( statusMode ) {
			GetDlgItem( IDC_STATUSMODENAME_EDIT )->SetWindowText( statusMode->name.c_str() );
			GetDlgItem( IDC_STATUSMODEINFO_EDIT )->SetWindowText( statusMode->info.c_str() );
		}
	}
	OnUpdateModify();
}

void CChatStatusModeOptions::OnNameChanged()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatStatusMode* statusMode = statusModes[current];
		if ( statusMode ) {
			CString s = statusMode->name.c_str();
			GetDlgItem( IDC_STATUSMODENAME_EDIT )->GetWindowText( s );
			statusMode->name = static_cast<LPCTSTR>(s);
			OnUpdateModify();
		}
	}
}

void CChatStatusModeOptions::OnInfoChanged()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatStatusMode* statusMode = statusModes[current];
		if ( statusMode ) {
			CString s = statusMode->info.c_str();
			GetDlgItem( IDC_STATUSMODEINFO_EDIT )->GetWindowText( s );
			statusMode->info = static_cast<LPCTSTR>(s);
			OnUpdateModify();
		}
	}
}

void CChatStatusModeOptions::OnAutoAwayChanged()
{
	bool use = static_cast<CButton*>(GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_AWAY_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_AWAY_EDIT )->EnableWindow( use );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_AWAY_SPIN )->EnableWindow( use );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_AWAY_STATIC )->EnableWindow( use );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_CHECK )->EnableWindow( use );
	bool useNA = use && static_cast<CButton*>(GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_EDIT )->EnableWindow( useNA );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_SPIN )->EnableWindow( useNA );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_STATIC )->EnableWindow( useNA );
	OnUpdateModify();
}

void CChatStatusModeOptions::OnAutoNotAvailibaleChanged()
{
	bool use = static_cast<CButton*>(GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_EDIT )->EnableWindow( use );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_SPIN )->EnableWindow( use );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_STATIC )->EnableWindow( use );
	OnUpdateModify();
}

void CChatStatusModeOptions::OnUpdateModify()
{
	UpdateData();

	SetModified( statusModes != chatApp.statusModes || useAutoAway != ( chatApp.mainFrame->getUseAutoAway() ? 1 : 0 ) || autoAwayMinuts != chatApp.mainFrame->getAutoAwayMinuts() || useAutoNotAvailible != ( chatApp.mainFrame->getUseAutoNotAvailible() ? 1 : 0 ) || autoNotAvailibleMinuts != chatApp.mainFrame->getAutoNotAvailibleMinuts() );
}

// ----------------------------------------------------------------------------
// ---------- CChatOptions
// ----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP( CChatOptions, CPropertySheet )
	//{{AFX_MSG_MAP(CChatOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChatOptions::CChatOptions():
	CPropertySheet(),
	generalOptions( NULL ),
	soundOptions( NULL ),
	statusModeOptions( NULL )
{
	SetTitle( format( IDS_OPTIONS ).c_str() );

	generalOptions    = new CChatGeneralOptions();
	soundOptions      = new CChatSoundOptions();
	statusModeOptions = new CChatStatusModeOptions();
	AddPage( generalOptions );
	AddPage( soundOptions );
	AddPage( statusModeOptions );
}

CChatOptions::~CChatOptions()
{
	if ( generalOptions ) delete generalOptions;
	if ( soundOptions ) delete soundOptions;
	if ( statusModeOptions ) delete statusModeOptions;
}
