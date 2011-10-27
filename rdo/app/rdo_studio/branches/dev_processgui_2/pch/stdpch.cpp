// stdafx.cpp : source file that includes just the standard includes
//	rdo_studio.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "app/rdo_studio_mfc/pch/stdpch.h"

void appendMenu(PTR(CMenu) pFrom, const int from_index, PTR(CMenu) pTo)
{
	CString s;
	pFrom->GetMenuString(from_index, s, MF_BYPOSITION);

	PTR(CMenu) pSubFrom = pFrom->GetSubMenu(from_index);
	int item_count = 0;
	if (pSubFrom) item_count = pSubFrom->GetMenuItemCount();

	if (item_count)
	{

		PTR(CMenu) pMenu = new CMenu();
		pMenu->CreateMenu();
		pTo->AppendMenu(MF_STRING | MF_POPUP, (int)pMenu->m_hMenu, s);

		for (int i = 0; i < item_count; i++)
		{
			appendMenu(pSubFrom, i, pMenu);
		}

	}
	else
	{
		UINT itemID = pFrom->GetMenuItemID(from_index);
		if (itemID)
		{
			pTo->AppendMenu(MF_STRING, itemID, s);
		}
		else
		{
			pTo->AppendMenu(MF_SEPARATOR);
		}
	}
}

void eraseMenu(PTR(CMenu) pFrom, const int from_index)
{
	PTR(CMenu) pSubFrom = (from_index != -1) ? pFrom->GetSubMenu(from_index) : pFrom;
	int item_count = 0;
	if (pSubFrom)
	{
		item_count = pSubFrom->GetMenuItemCount();
	}

	if (item_count)
	{
		for (int i = item_count-1; i >= 0; i--)
		{
			eraseMenu(pSubFrom, i);
		}
		if (from_index != -1)
		{
			delete pSubFrom;
		}
	}

	if (from_index != -1)
	{
		pFrom->DeleteMenu(from_index, MF_BYPOSITION);
	}
}

int roundDouble(const double val)
{
	if (val >= 0)
	{
		return (val - (int)val < 0.5) ? (int)val : (int)val + 1;
	}
	else
	{
		return (val - (int)val > - 0.5) ? (int)val : (int)val - 1;
	}
}
