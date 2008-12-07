#ifndef RDOSPLITTERWND_H
#define RDOSPLITTERWND_H
#pragma once

// ----------------------------------------------------------------------------
// ---------- RDOSplitterWnd
// ----------------------------------------------------------------------------
class RDOSplitterWnd: public CSplitterWnd
{
DECLARE_DYNCREATE( RDOSplitterWnd )
protected:
    int hiddenCol;
    int hiddenRow;
	

	afx_msg void OnSetFocus( CWnd *pOldWnd );
	DECLARE_MESSAGE_MAP()
public:
    RDOSplitterWnd();
    virtual ~RDOSplitterWnd();

    void showColumn();
    void hideColumn(int col);
    void showRow();
    void hideRow(int row);
    
};

#endif //RDOSPLITTERWND_H