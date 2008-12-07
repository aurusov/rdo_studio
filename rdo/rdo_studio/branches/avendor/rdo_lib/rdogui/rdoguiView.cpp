// rdoguiView.cpp : implementation of the CRdoguiView class
//

#include "stdafx.h"
#include "rdogui.h"

#include "rdoguiDoc.h"
#include "rdoguiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRdoguiView

IMPLEMENT_DYNCREATE(CRdoguiView, CView)

BEGIN_MESSAGE_MAP(CRdoguiView, CView)
	//{{AFX_MSG_MAP(CRdoguiView)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRdoguiView construction/destruction

CRdoguiView::CRdoguiView()
	: dc(NULL), rr(0, 0, 0, 0), bmp(NULL)
{
	// TODO: add construction code here
}

CRdoguiView::~CRdoguiView()
{
	delete bmp;
	delete dc;
}

BOOL CRdoguiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRdoguiView drawing

void CRdoguiView::OnDraw(CDC* pDC)
{
	CRdoguiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	if(!dc)
	{
		dc = new CDC();
		dc->CreateCompatibleDC(pDC);
	}

	CRect newRect;
	GetClientRect(newRect);
	if(!rr.EqualRect(newRect) || !bmp)
	{
		*rr = newRect;
		delete bmp;
		bmp = new CBitmap();
		bmp->CreateCompatibleBitmap(pDC, newRect.Width(), newRect.Height());
		dc->SelectObject(bmp);
	}

	CBrush brush(RGB(255, 255, 255));
	CBrush* pOldBrush = dc->SelectObject(&brush);
	CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	CPen* pOldPen = dc->SelectObject(&penBlack);
	dc->Rectangle(rr);
	dc->SelectObject(pOldBrush);
	dc->SelectObject(pOldPen);


	CSingleLock lock(&pDoc->frameUsed);
	lock.Lock();
	if(pDoc->frame)
	{

		////// Background ///////////
		if(!pDoc->frame->hasBackPicture)
		{
			CBrush brush(RGB(pDoc->frame->r, pDoc->frame->g, pDoc->frame->b));
			CBrush* pOldBrush = dc->SelectObject(&brush);
			CPen penBlack;
			penBlack.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
			CPen* pOldPen = dc->SelectObject(&penBlack);
			CRect rect(0, 0, pDoc->frame->width, pDoc->frame->height);
			dc->Rectangle(rect);
			dc->SelectObject(pOldBrush);
			dc->SelectObject(pOldPen);
		}

		int size = pDoc->frame->elements.size();
		for(int i = 0; i < size; i++)
		{
			RDOFrameElement *currElement = pDoc->frame->elements.at(i);
			switch(currElement->type)
			{
			case RDOFrameElement::text_type:
				{																		
					RDOTextElement *textEl = (RDOTextElement *)currElement;
					if(!textEl->background.isTransparent)
					{
						dc->SetBkMode(OPAQUE);
						dc->SetBkColor(RGB(textEl->background.r, textEl->background.g, textEl->background.b));
					}
					else
						dc->SetBkMode(TRANSPARENT);

					if(!textEl->foreground.isTransparent)
						dc->SetTextColor(RGB(textEl->foreground.r, textEl->foreground.g, textEl->foreground.b));

					UINT nFormat = DT_SINGLELINE;
					switch(textEl->align)
					{
					case RDOTextElement::left:
						nFormat |= DT_LEFT; break;
					case RDOTextElement::right:
						nFormat |= DT_RIGHT; break;
					case RDOTextElement::center:
						nFormat |= DT_CENTER; break;
					}

					dc->DrawText(textEl->strText.c_str(), textEl->strText.length(),
						CRect(textEl->x, textEl->y, textEl->x + textEl->w, textEl->y + textEl->h),
						nFormat);
				}
				break;

			case RDOFrameElement::rect_type:
				{																		
					RDORectElement *rectEl = (RDORectElement *)currElement;
					CBrush brush(RGB(rectEl->background.r, rectEl->background.g, rectEl->background.b));
					if(!rectEl->background.isTransparent)
					{
						dc->SelectObject(brush);
					}
					else
						dc->SelectStockObject(NULL_BRUSH);

					CPen pen(PS_SOLID, 0, RGB(rectEl->foreground.r, rectEl->foreground.g, rectEl->foreground.b));
					if(!rectEl->foreground.isTransparent)
						dc->SelectObject(pen);

					dc->Rectangle(rectEl->x, rectEl->y, rectEl->x + rectEl->w, rectEl->y + rectEl->h);
				}
				break;

			case RDOFrameElement::line_type:
				{																		
					RDOLineElement *lineEl = (RDOLineElement *)currElement;
					CPen pen(PS_SOLID, 0, RGB(lineEl->foreground.r, lineEl->foreground.g, lineEl->foreground.b));
					if(!lineEl->foreground.isTransparent)
						dc->SelectObject(pen);

					dc->MoveTo(lineEl->x, lineEl->y);
					dc->LineTo(lineEl->w, lineEl->h);
				}
				break;

			case RDOFrameElement::s_bmp_type:
				{																		
					RDOSBmpElement *sbmpEl = (RDOSBmpElement *)currElement;
					CBrush brush(RGB(196, 196, 0));
					dc->SelectObject(brush);
					CPen pen(PS_SOLID, 3, RGB(0, 196, 196));
					dc->SelectObject(pen);
					dc->Rectangle(sbmpEl->x, sbmpEl->y, sbmpEl->x + sbmpEl->w, sbmpEl->y + sbmpEl->h);
					dc->MoveTo(sbmpEl->x, sbmpEl->y);
					dc->LineTo(sbmpEl->x + sbmpEl->w, sbmpEl->y + sbmpEl->h);
					dc->MoveTo(sbmpEl->x + sbmpEl->w, sbmpEl->y);
					dc->LineTo(sbmpEl->x, sbmpEl->y + sbmpEl->h);
				}
				break;

			case RDOFrameElement::active_type:
				{																		
					RDOActiveElement *activeEl = (RDOActiveElement *)currElement;
					CBrush brush(RGB(196, 0, 196));
					dc->SelectObject(brush);
					CPen pen(PS_SOLID, 2, RGB(196, 196, 0));
					dc->SelectObject(pen);
					dc->Rectangle(activeEl->x, activeEl->y, activeEl->x + activeEl->w, activeEl->y + activeEl->h);
					dc->MoveTo(activeEl->x, activeEl->y);
					dc->LineTo(activeEl->x + activeEl->w, activeEl->y + activeEl->h);
					dc->MoveTo(activeEl->x + activeEl->w, activeEl->y);
					dc->LineTo(activeEl->x, activeEl->y + activeEl->h);
				}
				break;
			}
		}
	}

	pDC->BitBlt(0, 0, rr.Width(), rr.Height(), dc, 0, 0, SRCCOPY);
	lock.Unlock();
}

/////////////////////////////////////////////////////////////////////////////
// CRdoguiView printing

BOOL CRdoguiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRdoguiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRdoguiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRdoguiView diagnostics

#ifdef _DEBUG
void CRdoguiView::AssertValid() const
{
//	CView::AssertValid();
}

void CRdoguiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRdoguiDoc* CRdoguiView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRdoguiDoc)));
	return (CRdoguiDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CRdoguiView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
//	return CView::OnEraseBkgnd(pDC);
	return 1;
}

void CRdoguiView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CRdoguiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CSingleLock lock(&pDoc->frameUsed);
	lock.Lock();
	pDoc->scanCodes.push_back(nChar);
	lock.Unlock();
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRdoguiView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRdoguiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CSingleLock lock(&pDoc->frameUsed);
	lock.Lock();

	if(pDoc->frame)
	{
		int size = pDoc->frame->elements.size();
		for(int i = 0; i < size; i++)
		{
			RDOFrameElement *currElement = pDoc->frame->elements.at(i);
			if(currElement->type == RDOFrameElement::active_type)
			{																		
				RDOActiveElement *activeEl = (RDOActiveElement *)currElement;
				CRect rect(activeEl->x, activeEl->y, activeEl->x + activeEl->w, activeEl->y + activeEl->h);
				if(rect.PtInRect(point))
					pDoc->areasActivated.push_back(activeEl->operName);
			}
		}
	}

	lock.Unlock();

	CView::OnLButtonDown(nFlags, point);
}
