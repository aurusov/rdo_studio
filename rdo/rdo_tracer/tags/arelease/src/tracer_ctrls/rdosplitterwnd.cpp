#include "StdAfx.h"
#include "rdosplitterwnd.h"

// ----------------------------------------------------------------------------
// ---------- RDOSplitterWnd
// ----------------------------------------------------------------------------
IMPLEMENT_DYNCREATE( RDOSplitterWnd, CSplitterWnd )

BEGIN_MESSAGE_MAP( RDOSplitterWnd, CSplitterWnd )
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

RDOSplitterWnd ::RDOSplitterWnd ():
	CSplitterWnd(),
	hiddenCol( -1 ),
	hiddenRow( -1 )
{
}

RDOSplitterWnd ::~RDOSplitterWnd ()
{
}

void RDOSplitterWnd ::OnSetFocus( CWnd *pOldWnd )
{
	CSplitterWnd::OnSetFocus( pOldWnd );
	//GetPane( 0, 0 )->SetFocus();
	SetActivePane( 0, 0 );
}

void RDOSplitterWnd ::hideRow(int row)
{
    ASSERT_VALID( this );
    ASSERT( m_nRows > 1 );
    ASSERT( row < m_nRows );
    ASSERT( hiddenRow == -1 );
    hiddenRow = row;
    
    int activeRow, activeCol;
// if the row has an active window -- change it
    if( GetActivePane( &activeRow, &activeCol ) )
    {
        if( activeRow == row )
        {
            if( ++activeRow >= m_nRows )
                activeRow = 0;
            SetActivePane( activeRow, activeCol );
        }
    }

// hide all row panes.
    for( int curcol = 0; curcol < m_nCols; ++curcol )
    {
        CWnd* pPaneHide = GetPane( row, curcol );
        ASSERT( pPaneHide );

        pPaneHide->ShowWindow( SW_HIDE );
        pPaneHide->SetDlgCtrlID( AFX_IDW_PANE_FIRST + curcol + (16 * m_nRows) );
        
        for( int currow = row + 1; currow < m_nRows; ++currow )
        {
            CWnd* pPane = GetPane( currow, curcol );
            ASSERT( pPane );

            pPane->SetDlgCtrlID( IdFromRowCol( currow - 1, curcol ));
        }
    }

    m_nRows --;
    m_pRowInfo[m_nRows].nCurSize = m_pRowInfo[row].nCurSize;
    RecalcLayout();
}

void RDOSplitterWnd ::showRow()
{
    ASSERT_VALID( this );
    ASSERT( m_nRows < m_nMaxRows );
    ASSERT( hiddenRow != -1 );

    int row = hiddenRow;
    hiddenRow = -1;

    int cyNew = m_pRowInfo[m_nRows].nCurSize;
    m_nRows ++;
    
    ASSERT( m_nRows == m_nMaxRows );
    
    int currow;

// Show the hidden row
    for( int curcol = 0; curcol < m_nCols; ++curcol )
    {
        CWnd* pPaneShow = GetDlgItem( AFX_IDW_PANE_FIRST + curcol + (16 * m_nRows) );
        ASSERT( pPaneShow != NULL );
        pPaneShow->ShowWindow( SW_SHOWNA );

        for( currow = m_nRows - 2; currow >= row; --currow )
        {
            CWnd* pPane = GetPane( currow, curcol );
            ASSERT( pPane != NULL );
            pPane->SetDlgCtrlID( IdFromRowCol( currow + 1, curcol ));
        }

        pPaneShow->SetDlgCtrlID( IdFromRowCol( row, curcol ) );
    }

// new panes have been created -- recalculate layout
    for( currow = row + 1; currow < m_nRows; currow++ )
        m_pRowInfo[currow].nIdealSize = m_pRowInfo[currow - 1].nCurSize;

    m_pRowInfo[row].nIdealSize = cyNew;
    RecalcLayout();
}

void RDOSplitterWnd ::hideColumn(int col)
{
    ASSERT_VALID( this );
    ASSERT( m_nCols > 1 );
    ASSERT( col < m_nCols );
    ASSERT( hiddenCol == -1 );
    hiddenCol = col;
    
// if the column has an active window -- change it
    int activeRow, activeCol;
    if( GetActivePane( &activeRow, &activeCol ) )
    {
        if( activeCol == col )
        {
            if( ++activeCol >= m_nCols )
                activeCol = 0;
            SetActivePane( activeRow, activeCol );
        }
    }

// hide all column panes
    for( int currow = 0; currow < m_nRows; currow++ )
    {
        CWnd* pPaneHide = GetPane(currow, col);
        ASSERT( pPaneHide );

        pPaneHide->ShowWindow( SW_HIDE );
        pPaneHide->SetDlgCtrlID( AFX_IDW_PANE_FIRST + currow * 16 + m_nCols );
        
        for( int curcol = col + 1; curcol < m_nCols; curcol++ )
        {
            CWnd* pPane = GetPane( currow, curcol );
            ASSERT( pPane );

            pPane->SetDlgCtrlID( IdFromRowCol( currow, curcol - 1 ) );
        }
    }

    m_nCols --;
    m_pColInfo[m_nCols].nCurSize = m_pColInfo[col].nCurSize;
    RecalcLayout();
}

void RDOSplitterWnd ::showColumn()
{
    ASSERT_VALID( this );
    ASSERT( m_nCols < m_nMaxCols );
    ASSERT( hiddenCol != -1 );

    int col = hiddenCol;
    hiddenCol = -1;

    int cxNew = m_pColInfo[m_nCols].nCurSize;
    m_nCols++;
    
    ASSERT( m_nCols == m_nMaxCols );
    
    int curcol;

// Show the hidden column
    for( int currow = 0; currow < m_nRows; ++currow )
    {
        CWnd* pPaneShow = GetDlgItem( AFX_IDW_PANE_FIRST + currow * 16 + m_nCols );
        ASSERT( pPaneShow );
        pPaneShow->ShowWindow( SW_SHOWNA );

        for( curcol = m_nCols - 2; curcol >= col; --curcol )
        {
            CWnd* pPane = GetPane( currow, curcol );
            ASSERT( pPane );
            pPane->SetDlgCtrlID( IdFromRowCol( currow, curcol + 1 ) );
        }

        pPaneShow->SetDlgCtrlID( IdFromRowCol( currow, col ) );
    }

// new panes have been created -- recalculate layout
    for( curcol = col + 1; curcol < m_nCols; curcol++ )
        m_pColInfo[curcol].nIdealSize = m_pColInfo[curcol - 1].nCurSize;

    m_pColInfo[col].nIdealSize = cxNew;
    RecalcLayout();
}