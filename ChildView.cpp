#include "stdafx.h"
#include <io.h>
#include <string>
#include "mult.h"
#include "ChildView.h"

typedef std::vector<std::wstring> NameList;
NameList m_vectData;

void listDir(const wchar_t * sdir)
{
	std::wstring str, strDir;
    struct _wfinddata_t c_file;
    long hFile;

    // Find first file in current directory
    str = sdir + (std::wstring) L"\\*";
    if( (hFile = _wfindfirst( str.c_str(), &c_file )) == -1L ){
        fprintf(stderr, "Error opening %s!\n", str.c_str());
        return;
    }

    do{
        //skip if find . and ..
        if ((wcscmp(c_file.name, L".") == 0 ||  wcscmp(c_file.name, L"..") == 0)) {
            continue;
        }

        str = L"";

        if (c_file.attrib & _A_SUBDIR){
			strDir = sdir + (std::wstring) L"\\" + (std::wstring) c_file.name;
            m_vectData.push_back(str);
            listDir(strDir.c_str());
            continue;
        }
        //join given path and file name
        str += (std::wstring) sdir + (std::wstring) L"\\" + (std::wstring) c_file.name;
        m_vectData.push_back(str);
    }while(_wfindnext( hFile, &c_file ) == 0);
   _findclose( hFile );
   return;
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT ()
	ON_WM_TIMER ()
END_MESSAGE_MAP()

CChildView::CChildView()
{}

CChildView::~CChildView()
{}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	listDir(_T("mult"));

	NameList::const_iterator it = m_vectData.begin();
	while (it != m_vectData.end())
	{
		m_imageList.push_back(Gdiplus::Image::FromFile(it->c_str(), TRUE));
		++it;
	}

	m_imageCurrent = m_imageList.begin();

	SetTimer(1, 500, 0);

	return 0;
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC _dc(this);
	Gdiplus::Graphics dc(_dc);
	dc.DrawImage(*m_imageCurrent, Gdiplus::Point(250, 50));
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	CWnd::OnTimer(nIDEvent);

	++m_imageCurrent;
	if (m_imageCurrent == m_imageList.end())
	{
		m_imageCurrent = m_imageList.begin();
	}

	Invalidate();
}
