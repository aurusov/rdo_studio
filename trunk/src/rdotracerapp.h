#ifndef RDOTRACERAPP_H
#define RDOTRACERAPP_H

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "rdotracermainfrm.h"

// ----------------------------------------------------------------------------
// ---------- RDOTracerApp
// ----------------------------------------------------------------------------
class RDOTracerApp : public CWinApp
{
protected:
	CFont font;

public:
	RDOTracerApp();
	virtual ~RDOTracerApp();

	//{{AFX_VIRTUAL(RDOTracerApp)
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOTracerApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	CFont& getFont();
	
	static void appendMenu( CMenu* from, const int from_index, CMenu* to );
	static void eraseMenu( CMenu* from, const int from_index = -1 );

	RDOTracerMainFrame* mainFrame;
	static std::string getFullFileName();
	static std::string extractFilePath( const std::string& fileName );
	static bool isFileExists( const std::string& fileName );
	static std::string getFullHelpFileName( std::string str = "RAO-tracer.chm" );
	
	void showMFCException( const UINT errorTypeID, CException& e ) const;
};

// ----------------------------------------------------------------------------
extern RDOTracerApp rdoTracerApp;

//{{AFX_INSERT_LOCATION}}

#endif // RDOTRACERAPP_H