// stdafx.cpp : source file that includes just the standard includes
//	rdo_studio.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

void appendMenu( CMenu* from, const int from_index, CMenu* to )
{
	CString s;
	from->GetMenuString( from_index, s, MF_BYPOSITION );

	CMenu* m_from = from->GetSubMenu( from_index );
	int item_count = 0;
	if ( m_from ) item_count = m_from->GetMenuItemCount();

	if ( item_count ) {

		CMenu* m_to = new CMenu();
		m_to->CreateMenu();
		to->AppendMenu( MF_STRING | MF_POPUP, (int)m_to->m_hMenu, s );

		for ( int i = 0; i < item_count; i++ ) {
			appendMenu( m_from, i, m_to );
		}

	} else {
		UINT itemID = from->GetMenuItemID( from_index );
		if ( itemID ) {
			to->AppendMenu( MF_STRING, itemID, s );
		} else {
			to->AppendMenu( MF_SEPARATOR );
		}
	}
}

void eraseMenu( CMenu* from, const int from_index )
{
	CMenu* m_from;
	if ( from_index != -1 )	{
		m_from = from->GetSubMenu( from_index );
	} else {
		m_from = from;
	}
	int item_count = 0;
	if ( m_from ) item_count = m_from->GetMenuItemCount();

	if ( item_count ) {
		for ( int i = item_count-1; i >= 0; i-- ) {
			eraseMenu( m_from, i );
		}
		if ( from_index != -1 ) delete m_from;
	}

	if ( from_index != -1 ) from->DeleteMenu( from_index, MF_BYPOSITION );
}

int roundDouble( const double val )
{
	if ( val >= 0 )
		return ( val - (int)val < 0.5 ) ? (int)val : (int)val + 1;
	else
		return ( val - (int)val > - 0.5 ) ? (int)val : (int)val - 1;
}
