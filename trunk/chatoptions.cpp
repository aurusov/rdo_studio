#include "stdafx.h"
#include "chatoptions.h"
#include "chatapp.h"
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
	userName = chatApp.getUserName();

	CChatMainFrame* mainFrame = (CChatMainFrame*)AfxGetMainWnd();

	useTray            = mainFrame->getUseTray() ? 1 : 0;
	notifyAboutMessage = mainFrame->getNotifyAboutMessage() ? 1 : 0;

	closeButtonAction    = mainFrame->getCloseButtonAction();
	minimizeButtonAction = mainFrame->getMinimizeButtonAction();

	usePopup      = mainFrame->popupMessage.getUsePopup() ? 1 : 0;
	popupPosition = mainFrame->popupMessage.getPosition();
	popupDelay    = mainFrame->popupMessage.getTimerDelay();
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
	int itemHeight = ((CComboBox*)GetDlgItem( IDC_POPUPPOSITION_COMBO ))->GetItemHeight( -1 );
	fgColorCB.setItemHeight( itemHeight );
	bgColorCB.setItemHeight( itemHeight );
	fgColorCB.insertBaseColors();
	bgColorCB.insertBaseColors();

	CChatMainFrame* mainFrame = (CChatMainFrame*)AfxGetMainWnd();
	fgColorCB.insertColor( mainFrame->popupMessage.getTextColor() );
	bgColorCB.insertColor( mainFrame->popupMessage.getBackgroundColor() );

	OnUseTrayCBChanged();
	OnUsePopupCBChanged();

	return true;
}

void CChatGeneralOptions::OnOK()
{
	chatApp.setUserName( userName );

	CChatMainFrame* mainFrame = (CChatMainFrame*)AfxGetMainWnd();

	mainFrame->setUseTray( useTray ? true : false );
	mainFrame->setNotifyAboutMessage( notifyAboutMessage ? true : false );
	mainFrame->setCloseButtonAction( (CChatCloseButtonAction)closeButtonAction );
	mainFrame->setMinimizeButtonAction( (CChatMinimizeButtonAction)minimizeButtonAction );

	mainFrame->popupMessage.setUsePopup( usePopup ? true : false );
	mainFrame->popupMessage.setPosition( (CChatPopupPosition)popupPosition );
	mainFrame->popupMessage.setTimerDelay( popupDelay );
	mainFrame->popupMessage.setTextColor( fgColorCB.getCurrentColor() );
	mainFrame->popupMessage.setBackgroundColor( bgColorCB.getCurrentColor() );

	CPropertyPage::OnOK();
}

void CChatGeneralOptions::OnUseTrayCBChanged()
{
	bool use = ((CButton*)GetDlgItem( IDC_TRAYUSE_CHECK ))->GetCheck() ? true : false;
	if ( !use ) {
		if ( ((CButton*)GetDlgItem( IDC_CLOSEBUTTON_TRAY_RADIO ))->GetCheck() ) {
			((CButton*)GetDlgItem( IDC_CLOSEBUTTON_TRAY_RADIO ))->SetCheck( false );
			((CButton*)GetDlgItem( IDC_CLOSEBUTTON_CLOSE_RADIO ))->SetCheck( true );
		}
		if ( ((CButton*)GetDlgItem( IDC_MINIMIZEBUTTON_TRAY_RADIO ))->GetCheck() ) {
			((CButton*)GetDlgItem( IDC_MINIMIZEBUTTON_TRAY_RADIO ))->SetCheck( false );
			((CButton*)GetDlgItem( IDC_MINIMIZEBUTTON_MINIMIZE_RADIO ))->SetCheck( true );
		}
	}
	GetDlgItem( IDC_TRAYNOTIFY_CHECK )->EnableWindow( use );
	GetDlgItem( IDC_CLOSEBUTTON_TRAY_RADIO )->EnableWindow( use );
	GetDlgItem( IDC_MINIMIZEBUTTON_TRAY_RADIO )->EnableWindow( use );
	OnUpdateModify();
}

void CChatGeneralOptions::OnUsePopupCBChanged()
{
	bool use = ((CButton*)GetDlgItem( IDC_POPUPUSE_CHECK ))->GetCheck() ? true : false;
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

	CChatMainFrame* mainFrame = (CChatMainFrame*)AfxGetMainWnd();
	
	SetModified( userName != chatApp.getUserName() || useTray != ( mainFrame->getUseTray() ? 1 : 0 ) || notifyAboutMessage != ( mainFrame->getNotifyAboutMessage() ? 1 : 0 ) || closeButtonAction != mainFrame->getCloseButtonAction() || minimizeButtonAction != mainFrame->getMinimizeButtonAction() || usePopup != ( mainFrame->popupMessage.getUsePopup() ? 1 : 0 ) || popupPosition != mainFrame->popupMessage.getPosition() || popupDelay != mainFrame->popupMessage.getTimerDelay() || fgColorCB.getCurrentColor() != mainFrame->popupMessage.getTextColor() || bgColorCB.getCurrentColor() != mainFrame->popupMessage.getBackgroundColor() );
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
	while ( POSITION pos = sounds.GetTailPosition() ) {
		delete sounds.GetAt( pos );
		sounds.RemoveTail();
	}
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

	listBox.setImageList( &imageList );
	for ( int i = 0; i < chatApp.sounds.GetCount(); i++ ) {
		CChatSound* snd = new CChatSound( *chatApp.sounds.GetAt( chatApp.sounds.FindIndex( i ) ) );
		sounds.AddTail( snd );
		listBox.addString( chatApp.sounds.getName( snd->type ), snd->useSound ? 0 : -1 );
	}
	listBox.SetCurSel( 0 );
	OnListBoxSelectChange();

	return true;
}

void CChatSoundOptions::OnOK()
{
	chatApp.sounds.setUseSound( useSound ? true : false );

	for ( int i = 0; i < sounds.GetCount(); i++ ) {
		CChatSound* snd1 = chatApp.sounds.GetAt( chatApp.sounds.FindIndex( i ) );
		CChatSound* snd2 = sounds.GetAt( sounds.FindIndex( i ) );
		*snd1 = *snd2;
	}

	CPropertyPage::OnOK();
}

void CChatSoundOptions::OnListBoxSelectChange()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatSound* snd = sounds.GetAt( sounds.FindIndex( current ) );
		if ( snd ) {
			((CButton*)GetDlgItem( IDC_SOUNDNONE_RADIO ))->SetCheck( false );
			((CButton*)GetDlgItem( IDC_SOUNDDEFAULT_RADIO ))->SetCheck( false );
			((CButton*)GetDlgItem( IDC_SOUNDEXTERNAL_RADIO ))->SetCheck( false );
			if ( !snd->useSound ) {
				((CButton*)GetDlgItem( IDC_SOUNDNONE_RADIO ))->SetCheck( true );
			} else {
				if ( snd->useDefault ) {
					((CButton*)GetDlgItem( IDC_SOUNDDEFAULT_RADIO ))->SetCheck( true );
				} else {
					((CButton*)GetDlgItem( IDC_SOUNDEXTERNAL_RADIO ))->SetCheck( true );
				}
			}
			GetDlgItem( IDC_SOUNDEXTERNAL_EDIT )->SetWindowText( snd->file );
		}
	}
	OnUpdateModify();
}

void CChatSoundOptions::OnListBoxDoubleClick()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatSound* snd = sounds.GetAt( sounds.FindIndex( current ) );
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
		CChatSound* snd = sounds.GetAt( sounds.FindIndex( current ) );
		if ( snd ) {
			snd->useSound   = ((CButton*)GetDlgItem( IDC_SOUNDNONE_RADIO ))->GetCheck() ? false : true;
			snd->useDefault = ((CButton*)GetDlgItem( IDC_SOUNDDEFAULT_RADIO ))->GetCheck() ? true : false;
			listBox.setItemImage( current, snd->useSound ? 0 : -1 );
		}
	}

	OnUpdateModify();
}

void CChatSoundOptions::OnEditChanged()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatSound* snd = sounds.GetAt( sounds.FindIndex( current ) );
		if ( snd ) {
			GetDlgItem( IDC_SOUNDEXTERNAL_EDIT )->GetWindowText( snd->file );
			OnUpdateModify();
		}
	}
}

void CChatSoundOptions::OnButtonClicked()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatSound* snd = sounds.GetAt( sounds.FindIndex( current ) );
		if ( snd ) {
			CString filter = "WAV Files (*.wav)|*.wav|All Files (*.*)|*.*||";
			CFileDialog dlg( true, "wav", snd->file, OFN_HIDEREADONLY, filter );
			if ( dlg.DoModal() == IDOK ) {
				snd->file = dlg.GetPathName();
				GetDlgItem( IDC_SOUNDEXTERNAL_EDIT )->SetWindowText( snd->file );
			}
		}
	}
}

void CChatSoundOptions::OnUpdateModify()
{
	UpdateData();

	bool use = ((CButton*)GetDlgItem( IDC_SOUNDUSE_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_SOUND_LIST )->EnableWindow( use );
	bool selected = use && listBox.GetCurSel() != LB_ERR;
	GetDlgItem( IDC_SOUNDNONE_RADIO )->EnableWindow( selected );
	GetDlgItem( IDC_SOUNDDEFAULT_RADIO )->EnableWindow( selected );
	GetDlgItem( IDC_SOUNDEXTERNAL_RADIO )->EnableWindow( selected );
	GetDlgItem( IDC_SOUNDEXTERNAL_EDIT )->EnableWindow( selected );
	GetDlgItem( IDC_SOUNDEXTERMAL_BUTTON )->EnableWindow( selected );
	if ( selected ) {
		bool externalFlag = ((CButton*)GetDlgItem( IDC_SOUNDEXTERNAL_RADIO ))->GetCheck() ? true : false;
		GetDlgItem( IDC_SOUNDEXTERNAL_EDIT )->EnableWindow( externalFlag );
		GetDlgItem( IDC_SOUNDEXTERMAL_BUTTON )->EnableWindow( externalFlag );
	}

	bool flag = false;
	for ( int i = 0; i < sounds.GetCount(); i++ ) {
		CChatSound* snd1 = chatApp.sounds.GetAt( chatApp.sounds.FindIndex( i ) );
		CChatSound* snd2 = sounds.GetAt( sounds.FindIndex( i ) );
		if ( *snd1 != *snd2 ) {
			flag = true;
			break;
		}
	}

	SetModified( useSound != ( chatApp.sounds.getUseSound() ? 1 : 0 ) || flag );
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
	CChatMainFrame* mainFrame = (CChatMainFrame*)AfxGetMainWnd();

	useAutoAway            = mainFrame->getUseAutoAway() ? 1 : 0;
	autoAwayMinuts         = mainFrame->getAutoAwayMinuts();
	useAutoNotAvailible    = mainFrame->getUseAutoNotAvailible() ? 1 : 0;
	autoNotAvailibleMinuts = mainFrame->getAutoNotAvailibleMinuts();

	imageList.Create( 16, 16, ILC_COLORDDB | ILC_MASK, 1, 3 );
	CBitmap bmp;
	bmp.LoadBitmap( IDB_STATUSMODE );
	imageList.Add( &bmp, RGB( 255, 0, 255 ) );
}

CChatStatusModeOptions::~CChatStatusModeOptions()
{
	while ( POSITION pos = statusModes.GetTailPosition() ) {
		delete statusModes.GetAt( pos );
		statusModes.RemoveTail();
	}
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

	listBox.setImageList( &imageList );
	for ( int i = 0; i < chatApp.statusModes.GetCount(); i++ ) {
		CChatStatusMode* statusMode = new CChatStatusMode( *chatApp.statusModes.GetAt( chatApp.statusModes.FindIndex( i ) ) );
		statusModes.AddTail( statusMode );
		listBox.addString( chatApp.statusModes.getDefaultName( statusMode->type ), statusMode->type );
	}

	listBox.SetCurSel( 0 );

	OnListBoxSelectChange();
	OnAutoAwayChanged();

	return true;
}

void CChatStatusModeOptions::OnOK()
{
	for ( int i = 0; i < statusModes.GetCount(); i++ ) {
		CChatStatusMode* statusMode1 = chatApp.statusModes.GetAt( chatApp.statusModes.FindIndex( i ) );
		CChatStatusMode* statusMode2 = statusModes.GetAt( statusModes.FindIndex( i ) );
		*statusMode1 = *statusMode2;
	}

	CChatMainFrame* mainFrame = (CChatMainFrame*)AfxGetMainWnd();

	mainFrame->setUseAutoAway( useAutoAway ? true : false );
	mainFrame->setAutoAwayMinuts( autoAwayMinuts );
	mainFrame->setUseAutoNotAvailible( useAutoNotAvailible ? true : false );
	mainFrame->setAutoNotAvailibleMinuts( autoNotAvailibleMinuts );

	CPropertyPage::OnOK();
}

void CChatStatusModeOptions::OnListBoxSelectChange()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatStatusMode* statusMode = statusModes.GetAt( statusModes.FindIndex( current ) );
		if ( statusMode ) {
			GetDlgItem( IDC_STATUSMODENAME_EDIT )->SetWindowText( statusMode->name );
			GetDlgItem( IDC_STATUSMODEINFO_EDIT )->SetWindowText( statusMode->info );
		}
	}
	OnUpdateModify();
}

void CChatStatusModeOptions::OnNameChanged()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatStatusMode* statusMode = statusModes.GetAt( statusModes.FindIndex( current ) );
		if ( statusMode ) {
			GetDlgItem( IDC_STATUSMODENAME_EDIT )->GetWindowText( statusMode->name );
			OnUpdateModify();
		}
	}
}

void CChatStatusModeOptions::OnInfoChanged()
{
	int current = listBox.GetCurSel();
	if ( current != LB_ERR ) {
		CChatStatusMode* statusMode = statusModes.GetAt( statusModes.FindIndex( current ) );
		if ( statusMode ) {
			GetDlgItem( IDC_STATUSMODEINFO_EDIT )->GetWindowText( statusMode->info );
			OnUpdateModify();
		}
	}
}

void CChatStatusModeOptions::OnAutoAwayChanged()
{
	bool use = ((CButton*)GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_AWAY_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_AWAY_EDIT )->EnableWindow( use );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_AWAY_SPIN )->EnableWindow( use );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_AWAY_STATIC )->EnableWindow( use );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_CHECK )->EnableWindow( use );
	bool useNA = use && ((CButton*)GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_EDIT )->EnableWindow( useNA );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_SPIN )->EnableWindow( useNA );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_STATIC )->EnableWindow( useNA );
	OnUpdateModify();
}

void CChatStatusModeOptions::OnAutoNotAvailibaleChanged()
{
	bool use = ((CButton*)GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_CHECK ))->GetCheck() ? true : false;
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_EDIT )->EnableWindow( use );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_SPIN )->EnableWindow( use );
	GetDlgItem( IDC_AUTOMATICALLYSTATUSMODE_NOTAVAILIBLE_STATIC )->EnableWindow( use );
	OnUpdateModify();
}

void CChatStatusModeOptions::OnUpdateModify()
{
	UpdateData();

	bool flag = false;
	for ( int i = 0; i < statusModes.GetCount(); i++ ) {
		CChatStatusMode* statusMode1 = chatApp.statusModes.GetAt( chatApp.statusModes.FindIndex( i ) );
		CChatStatusMode* statusMode2 = statusModes.GetAt( statusModes.FindIndex( i ) );
		if ( *statusMode1 != *statusMode2 ) {
			flag = true;
			break;
		}
	}

	CChatMainFrame* mainFrame = (CChatMainFrame*)AfxGetMainWnd();

	SetModified( flag || useAutoAway != ( mainFrame->getUseAutoAway() ? 1 : 0 ) || autoAwayMinuts != mainFrame->getAutoAwayMinuts() || useAutoNotAvailible != ( mainFrame->getUseAutoNotAvailible() ? 1 : 0 ) || autoNotAvailibleMinuts != mainFrame->getAutoNotAvailibleMinuts() );
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
	CString s;
	s.LoadString( ID_OPTIONS );
	SetTitle( s );

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
