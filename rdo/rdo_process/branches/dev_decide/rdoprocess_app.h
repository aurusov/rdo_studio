#ifndef RDO_PROCESS_APP_H
#define RDO_PROCESS_APP_H

#include <fstream>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include <rdoprocess_project.h>
#include "rdoprocess_method_manager.h"
#include "rdoprocess_mainfrm.h"
#include "ctrl/rdolink.h"
#include "resource.h"

/*! Класс приложения. При старте программы автоматически создается один экземпляр.
К нему можно обратится по имени rpapp. Базовый класс CWinApp используется библиотекой MFC для инициализации
программы. Производит инициализацию реестра и создает класс главного окна.
*/

// ----------------------------------------------------------------------------
// ---------- RPApp
// ----------------------------------------------------------------------------
class RPMainFrame;

class RPApp: public CWinApp
{
friend class RPProjectMFC;
protected:
	RPMainFrame*    mainFrame;
	HMENU           m_hMDIMenu;
	HACCEL          m_hMDIAccel;
	RPMethodManager methods;
	std::ofstream   log;

public:
	RPApp();

	RPProjectBar& getProjectBar() const             { return mainFrame->projectBar; }
	const RPMethodManager& getMethodManager() const { return methods;               }

	//{{AFX_VIRTUAL(RPApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RPApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern RPApp rpapp;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // RDO_PROCESS_APP_H
