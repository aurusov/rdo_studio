/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      status_bar.h
  \author    Захаров Павел
  \date      09.04.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_STATUS_BAR_H_
#define _RDO_STUDIO_MFC_STATUS_BAR_H_

// --------------------------------------------------------------------------------
// -------------------- RDOStudioStatusBar
// --------------------------------------------------------------------------------
class RDOStudioStatusBar: public CStatusBar
{
DECLARE_DYNAMIC(RDOStudioStatusBar)

private:
	CProgressCtrl progress;

	UINT indicator;

	rbool visible;

	void repositionProgressCtrl( const rbool redraw = true );

public:
	RDOStudioStatusBar();
	virtual ~RDOStudioStatusBar();

	void setProgressIndicator( const UINT ind ) { indicator = ind; };
	UINT getProgressIndicator() const { return indicator; };

	void  setProgressVisible( const rbool _visible );
	rbool getProgressVisible() { return visible; };

	void setRange( const int lower = 0, const int upper = 100 ) { progress.SetRange32( lower, upper ); };
	void getRange( int& lower, int& upper ) const { const_cast<CProgressCtrl&>(progress).GetRange( lower, upper ); };
	int setPos( const int pos ) { return progress.SetPos( pos ); };
	int getPos() const { return const_cast<CProgressCtrl&>(progress).GetPos(); };
	int offsetPos( const int offset = 1 ) { return progress.OffsetPos( offset ); };
	int setStep( const int step = 1 ) { return progress.SetStep( step ); };
	int stepIt() { return progress.StepIt(); };

private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

#endif // _RDO_STUDIO_MFC_STATUS_BAR_H_
