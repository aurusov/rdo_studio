#ifndef RDOSTUDIOSTATUSBAR_H
#define RDOSTUDIOSTATUSBAR_H

#if _MSC_VER > 1000
#pragma once
#endif

// --------------------------------------------------------------------------------
// -------------------- RDOStudioStatusBar
// --------------------------------------------------------------------------------
class RDOStudioStatusBar: public CStatusBar
{
DECLARE_DYNAMIC(RDOStudioStatusBar)

private:
	CProgressCtrl progress;

	UINT indicator;

	bool visible;

	void repositionProgressCtrl( const bool redraw = true );

public:
	RDOStudioStatusBar();
	virtual ~RDOStudioStatusBar();

	void setProgressIndicator( const UINT ind ) { indicator = ind; };
	UINT getProgressIndicator() const { return indicator; };

	void setProgressVisible( const bool _visible );
	bool getProgressVisible() { return visible; };

	void setRange( const int lower = 0, const int upper = 100 ) { progress.SetRange32( lower, upper ); };
	void getRange( int& lower, int& upper ) const { const_cast<CProgressCtrl&>(progress).GetRange( lower, upper ); };
	int setPos( const int pos ) { return progress.SetPos( pos ); };
	int getPos() const { return const_cast<CProgressCtrl&>(progress).GetPos(); };
	int offsetPos( const int offset = 1 ) { return progress.OffsetPos( offset ); };
	int setStep( const int step = 1 ) { return progress.SetStep( step ); };
	int stepIt() { return progress.StepIt(); };

protected:
	//{{AFX_VIRTUAL(RDOStudioStatusBar)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(RDOStudioStatusBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // RDOSTUDIOSTATUSBAR_H
