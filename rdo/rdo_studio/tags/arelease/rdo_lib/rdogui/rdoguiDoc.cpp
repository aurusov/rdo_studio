// rdoguiDoc.cpp : implementation of the CRdoguiDoc class
//

#include "stdafx.h"
#include "rdogui.h"

#include "rdoguiDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRdoguiDoc

IMPLEMENT_DYNCREATE(CRdoguiDoc, CDocument)

BEGIN_MESSAGE_MAP(CRdoguiDoc, CDocument)
	//{{AFX_MSG_MAP(CRdoguiDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRdoguiDoc construction/destruction

CRdoguiDoc::CRdoguiDoc()
{
	// TODO: add one-time construction code here
	model = NULL;
	frame = NULL;
	th = NULL;
}

CRdoguiDoc::~CRdoguiDoc()
{
	delete model;
	delete frame;
}

BOOL CRdoguiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


void frameCallBack(RDOConfig *config, void *param)
{
	if(config->showAnimation == RDO_SM_Animation)
	{
		CRdoguiDoc *doc = (CRdoguiDoc *)param;
		CSingleLock lock(&doc->frameUsed);
		lock.Lock();
		delete doc->frame;
		doc->frame = config->frame;

		config->keysPressed.insert(config->keysPressed.end(), doc->scanCodes.begin(), doc->scanCodes.end());
		doc->scanCodes.clear();

		config->activeAreasMouseClicked.insert(config->activeAreasMouseClicked.end(), doc->areasActivated.begin(), doc->areasActivated.end());
		doc->areasActivated.clear();

		lock.Unlock();
		doc->UpdateAllViews(NULL);
		Sleep(config->realTimeDelay);
	}
}


UINT MyControllingFunction( LPVOID pParam )
{
	CRdoguiDoc *doc = (CRdoguiDoc *)pParam;

	doc->model->executeModel(NULL, frameCallBack, pParam);
	MessageBox(NULL, doc->model->getConsole().c_str(), "End",
		MB_OK | MB_ICONINFORMATION);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CRdoguiDoc serialization

void CRdoguiDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		model = new RdoModel();
		bool res = model->parseModel(string((LPCSTR)ar.GetFile()->GetFilePath()));
		MessageBox(NULL, model->getConsole().c_str(), res?"Success":"Error", 
			MB_OK | res?MB_ICONINFORMATION:MB_ICONSTOP);
		if(res)
		{
			th = AfxBeginThread(MyControllingFunction, (LPVOID)this);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRdoguiDoc diagnostics

#ifdef _DEBUG
void CRdoguiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRdoguiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRdoguiDoc commands

void CRdoguiDoc::DeleteContents() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(th)
		th->SuspendThread();

	th = NULL;
	delete model;
	model = NULL;
	delete frame;
	frame = NULL;

	CDocument::DeleteContents();
}
