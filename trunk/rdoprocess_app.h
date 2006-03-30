#ifndef RDO_PROCESS_APP_H
#define RDO_PROCESS_APP_H

#include <fstream>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "rdoprocess_project.h"
#include "rdoprocess_messages.h"
#include "ctrl/rdolink.h"
#include "resource.h"

#include "rdoprocess_creation_RDO_files_MJ.h" // MJ 30.03.06
/*! Класс приложения. При старте программы автоматически создается один экземпляр.
К нему можно обратится по имени rpapp. Базовый класс CWinApp используется библиотекой MFC для инициализации
программы. Производит инициализацию реестра и создает класс главного окна.
RPApp содержит объект project.
*/

// ----------------------------------------------------------------------------
// ---------- RPApp
// ----------------------------------------------------------------------------
class RPMainFrame;

class RPApp: public CWinApp
{
friend rp::msg::~msg();

protected:
	RPMainFrame* mainFrame;
	HMENU m_hMDIMenu;
	HACCEL m_hMDIAccel;

	rp::msg*   _msg;
	RPProject* _project;

public:
	RPApp();

	rp::msg& msg()       { return *_msg;     }
	RPProject& project() { return *_project; }
	std::map< UINT, HCURSOR > cursors;
	std::ofstream             log;

	//MJ start Для создания класса файлов РДО и ссылки на него
	RPCreationRDOFilesMJ* RDOfiles;

	// MJ stop
	
	
	
	void sendMessage( RPObject* from, UINT message, WPARAM wParam = 0, LPARAM lParam = 0 ) {
		if ( _msg ) _msg->sendMessage( from, message, wParam, lParam );
	}

	//{{AFX_VIRTUAL(RPApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RPApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// ----------------------------------------------------------------------------
// ---------- RPAboutDlg
// ----------------------------------------------------------------------------
class RPAboutDlg: public CDialog
{
public:
	RPAboutDlg();
	virtual ~RPAboutDlg();

protected:
	//{{AFX_DATA(RPAboutDlg)
	enum { IDD = IDD_ABOUT };
	RDOLink	m_web;
	RDOLink	m_email;
	CString	m_caption;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(RPAboutDlg)
	protected:
	virtual void DoDataExchange( CDataExchange* pDX );
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(RPAboutDlg)
	afx_msg void OnAboutEmail();
	afx_msg void OnAboutWeb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern RPApp rpapp;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDO_PROCESS_APP_H
