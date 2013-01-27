/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/frame/options_ctrl.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      18.04.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_FRAME_OPTIONS_CTRL_H_
#define _RDO_STUDIO_FRAME_OPTIONS_CTRL_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioFrameOptionsCtrl
// --------------------------------------------------------------------------------
class RDOStudioFrameStyle;

class RDOStudioFrameOptionsCtrl: public CWnd
{
private:
	CBitmap bmp;
	CFont   font;
	RDOStudioFrameStyle* style;

public:
	RDOStudioFrameOptionsCtrl();
	virtual ~RDOStudioFrameOptionsCtrl();

	void setStyle( RDOStudioFrameStyle* _style );

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_FRAME_OPTIONS_CTRL_H_
