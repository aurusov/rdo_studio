#ifndef RDOEDITORAPP_H
#define RDOEDITORAPP_H
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#define useLog 0

#if useLog == 1
#define logFileName "c:\\rdo_edit_log.txt"
#define logWrite( A )                                  \
{                                                      \
	CFileStatus status;                                \
	if( !CFile::GetStatus( logFileName, status ) ) {   \
		CFile f;                                       \
		f.Open( logFileName, CFile::modeCreate );      \
		f.Close();                                     \
	}                                                  \
	CFile f;                                           \
	if ( f.Open( logFileName, CFile::modeWrite ) ) {   \
		f.SeekToEnd();                                 \
		CString _s = "app: ";                          \
		_s += A;                                       \
		_s += "\r\n";                                  \
		f.Write( _s, _s.GetLength() );                 \
		f.Close();                                     \
	}                                                  \
}
#endif

// ----------------------------------------------------------------------------
// ---------- RDOEditorApp
// ----------------------------------------------------------------------------
class RDOEditorApp: public CWinApp
{
protected:
	CFont font;

	virtual int ExitInstance();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	RDOEditorApp();
	virtual ~RDOEditorApp();

	virtual BOOL InitInstance();

	CFont& getFont();
	static void appendMenu( CMenu* from, const int from_index, CMenu* to );
	static void eraseMenu( CMenu* from, const int from_index = -1 );

	static void setCaption( const CString& str );

	static CString getFullFileName();
	static CString extractFilePath( CString str );
	static bool fileExists( CString str );
	static CString getFullHelpFileName( CString str = "RAO-editor.chm" );

	void setupFileAssociation();

	HMODULE dllHandle;
};

// ----------------------------------------------------------------------------
// ---------- RDOFileAssociationDlg
// ----------------------------------------------------------------------------
class RDOFileAssociationDlg: public CDialog
{
protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	virtual void OnCancel();

public:
	RDOFileAssociationDlg( CWnd* pParentWnd = NULL );
	virtual ~RDOFileAssociationDlg();

	int checkInFuture;
};

// ----------------------------------------------------------------------------
extern RDOEditorApp rdoEditorApp;

#endif // RDOEDITORAPP_H
