/*!
  \copyright (c) RDO-Team, 2007-2012
  \file      app/rdo_studio_mfc/src/model/new.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      12.01.2007
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio_mfc/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <shlobj.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/src/model/new.h"
// --------------------------------------------------------------------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModelNew
// --------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(RDOStudioModelNew, CDialog)
	ON_BN_CLICKED (IDC_MODEL_PATH_BUTTON, OnModelPathButton)
	ON_EN_CHANGE  (IDC_MODEL_NAME,        OnChangeModelName)
	ON_BN_CLICKED (IDC_MODEL_EMPTY,       OnModelEmpty     )
	ON_BN_CLICKED (IDC_MODEL_TEMPLATE,    OnModelEmpty     )
	ON_BN_CLICKED (IDC_MODEL_BARBER1,     OnModelEmpty     )
	ON_BN_CLICKED (IDC_MODEL_BARBER2,     OnModelEmpty     )
	ON_BN_CLICKED (IDC_COMMENT,           OnModelEmpty     )
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

RDOStudioModelNew::RDOStudioModelNew()
	: CDialog   (IDD                  )
	, m_colorRed(RGB(0xFF, 0x00, 0x00))
	, m_needRed (false                )
{
	m_modelName     = _T("mymodel");
	m_modelPath     = getMyDocFolder();
	m_info          = _T("");
	m_comment       = FALSE;
	m_modelTemplate = 1;
}

RDOStudioModelNew::~RDOStudioModelNew()
{}

void RDOStudioModelNew::DoDataExchange(PTR(CDataExchange) pDX) 
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK,            m_ok           );
	DDX_Text   (pDX, IDC_MODEL_NAME,  m_modelName    );
	DDX_Text   (pDX, IDC_MODEL_PATH,  m_modelPath    );
	DDX_Text   (pDX, IDC_INFO,        m_info         );
	DDX_Check  (pDX, IDC_COMMENT,     m_comment      );
	DDX_Radio  (pDX, IDC_MODEL_EMPTY, m_modelTemplate);
}

void RDOStudioModelNew::OnOK() 
{
	CDialog::OnOK();
}

BOOL RDOStudioModelNew::OnNotify(WPARAM wParam, LPARAM lParam, PTR(LRESULT) pResult) 
{
	return CDialog::OnNotify(wParam, lParam, pResult);
}

CString RDOStudioModelNew::getMyDocFolder() const
{
	LPITEMIDLIST pidl = NULL;
	if (::SHGetSpecialFolderLocation(m_hWnd, CSIDL_PERSONAL, &pidl) != NOERROR)
	{
		return _T("");
	}
	TCHAR szFolderDroppedIn[MAX_PATH];
	return ::SHGetPathFromIDList(pidl, szFolderDroppedIn) ? szFolderDroppedIn : _T("");
}

void RDOStudioModelNew::updateInfo()
{
	if (!m_modelPath.IsEmpty())
	{
		if (m_modelPath[m_modelPath.GetLength()-1] != _T('\\') && m_modelPath[m_modelPath.GetLength()-1] != _T('/'))
		{
			m_modelPath += _T('\\');
		}
	}
	if (m_modelName.IsEmpty())
	{
		m_info   = _T("Необходимо указать имя модели");
		m_needRed = true;
	}
	else
	{
		CString fullpath = m_modelPath + m_modelName;
		CFileFind finder;
		if (finder.FindFile(fullpath))
		{
			finder.FindNextFile();
			if (finder.IsDirectory())
			{
				m_info = rdo::format(_T("Такая директория уже существует: '%s\\'"), (LPCTSTR)fullpath).c_str();
			}
			else if (finder.IsReadOnly() || finder.IsCompressed() || finder.IsSystem() || finder.IsHidden() || finder.IsTemporary() || finder.IsNormal() || finder.IsArchived())
			{
				m_info = rdo::format(_T("Такой файл уже существует: '%s'"), (LPCTSTR)fullpath).c_str();
			}
			m_needRed = true;
		}
		else
		{
			m_info   = rdo::format(_T("Будет создана директория: '%s\\'"), (LPCTSTR)fullpath).c_str();
			m_needRed = false;
		}
		finder.Close();
	}
	m_ok.EnableWindow(!m_needRed);
	GetDlgItem(IDC_INFO)->SetFont(m_needRed ? &m_fontRed : &m_fontNormal);
	UpdateData(false);
}

static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	TCHAR szPath[_MAX_PATH];
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		if (lpData)
			SendMessage(hWnd,BFFM_SETSELECTION,TRUE,lpData);
		break;
	case BFFM_SELCHANGED:
		SHGetPathFromIDList(LPITEMIDLIST(lParam),szPath);
		SendMessage(hWnd, BFFM_SETSTATUSTEXT, NULL, LPARAM(szPath));
		break;
	}
	return 0;
}

BOOL GetFolder(LPCTSTR szTitle, LPTSTR szPath, LPCTSTR szRoot, HWND hWndOwner)
{
	if (szPath == NULL)
		return false;

	rbool result = false;

	LPMALLOC pMalloc;
	if (::SHGetMalloc(&pMalloc) == NOERROR)
	{
		BROWSEINFO bi;
		::ZeroMemory(&bi,sizeof bi);
		bi.ulFlags   = BIF_RETURNONLYFSDIRS;

		// дескриптор окна-владельца диалога
		bi.hwndOwner = hWndOwner;

		// добавление заголовка к диалогу
		bi.lpszTitle = szTitle;

		// отображение текущего каталога
		bi.lpfn      = BrowseCallbackProc;
		bi.ulFlags  |= BIF_STATUSTEXT;

		// установка каталога по умолчанию
		bi.lParam    = LPARAM(szPath);

		// установка корневого каталога
		if (szRoot != NULL)
		{
			PTR(IShellFolder) pDF;
			if (SHGetDesktopFolder(&pDF) == NOERROR)
			{
				LPITEMIDLIST pIdl = NULL;
				ULONG        chEaten;
				ULONG        dwAttributes;

				USES_CONVERSION;
				LPOLESTR oleStr = T2OLE(szRoot);

				pDF->ParseDisplayName(NULL, NULL, oleStr, &chEaten, &pIdl, &dwAttributes);
				pDF->Release();

				bi.pidlRoot = pIdl;
			}
		}

		LPITEMIDLIST pidl = ::SHBrowseForFolder(&bi);
		if (pidl != NULL)
		{
			if (::SHGetPathFromIDList(pidl,szPath))
				result = true;
			pMalloc->Free(pidl);
		}
		if (bi.pidlRoot != NULL)
		{
			pMalloc->Free((PTR(void))bi.pidlRoot);
		}
		pMalloc->Release();
	}
	return result;
}

void RDOStudioModelNew::OnModelPathButton()
{
	TCHAR szTitle[] = _T("Выбор директории для модели");
	TCHAR szPath[MAX_PATH];
	if (GetFolder(szTitle, szPath, NULL, m_hWnd))
	{
		m_modelPath = szPath;
		updateInfo();
	}
}

void RDOStudioModelNew::OnChangeModelName()
{
	static_cast<PTR(CEdit)>(GetDlgItem(IDC_MODEL_NAME))->GetWindowText(m_modelName);
	updateInfo();
}

BOOL RDOStudioModelNew::OnInitDialog()
{
	BOOL res = CDialog::OnInitDialog();
	PTR(CFont) pFont = GetFont();
	if (pFont)
	{
		LOGFONT lf;
		pFont->GetLogFont(&lf);
		m_fontNormal.CreateFontIndirect(&lf);
		lf.lfWeight = FW_BOLD;
		m_fontRed.CreateFontIndirect(&lf);
	}
	m_colorNormal = ::GetSysColor(COLOR_WINDOWTEXT);
	updateInfo();
	return res;
}

void RDOStudioModelNew::OnModelEmpty()
{
	UpdateData();
}

HBRUSH RDOStudioModelNew::OnCtlColor(PTR(CDC) pDC, PTR(CWnd) pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_INFO)
	{
		pDC->SetTextColor(m_needRed ? m_colorRed : m_colorNormal);
	}
	return hbr;
}

tstring RDOStudioModelNew::getModelName() const
{
	return tstring(m_modelName);
}

tstring RDOStudioModelNew::getModelPath() const
{
	return tstring(m_modelPath);
}

ruint RDOStudioModelNew::getModelTemplate() const
{
	return m_modelTemplate * 2 + m_comment;
}
