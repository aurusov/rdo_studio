/*
 * copyright: (c) RDO-Team, 2014
 * filename : rdointerface.cpp
 * author   : Урусов Андрей
 * date     : 03.05.2014
 * bref     :
 * indent   : 4T
 */

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/interface/rdointerface.h"
// --------------------------------------------------------------------------------

namespace rdo {

UnknownPointer::UnknownPointer()
	: m_pInterface(NULL)
	, m_pUnknown  (NULL)
{}

UnknownPointer::UnknownPointer(IUnknown& unknown)
	: m_pInterface(NULL    )
	, m_pUnknown  (&unknown)
{
	if (m_pUnknown)
		m_pUnknown->AddRef();
}

UnknownPointer::UnknownPointer(LPIGetUnknown pGetUnknown)
	: m_pInterface(NULL)
{
	m_pUnknown = pGetUnknown ? pGetUnknown->GetUnknown() : NULL;
	if (m_pUnknown)
		m_pUnknown->AddRef();
}

UnknownPointer::UnknownPointer(void* pInterface, LPIUnknown pUnknown)
	: m_pInterface(pInterface)
	, m_pUnknown  (pUnknown  )
{
	if (m_pUnknown)
		m_pUnknown->AddRef();
}

UnknownPointer::UnknownPointer(const UnknownPointer& pointer)
	: m_pInterface(pointer.m_pInterface)
	, m_pUnknown  (pointer.m_pUnknown  )
{
	if (m_pUnknown)
		m_pUnknown->AddRef();
}

UnknownPointer::~UnknownPointer()
{
	if (m_pUnknown)
		m_pUnknown->Release();
}

bool UnknownPointer::operator==(const UnknownPointer& pointer) const
{
	return m_pUnknown == pointer.m_pUnknown;
}

UnknownPointer& UnknownPointer::operator= (const UnknownPointer& pointer)
{
	if (m_pUnknown)
		m_pUnknown->Release();

	m_pInterface = pointer.m_pInterface;
	m_pUnknown   = pointer.m_pUnknown;
	if (m_pUnknown)
		m_pUnknown->AddRef();

	return *this;
}

UnknownPointer::operator bool() const
{
	return m_pUnknown != NULL;
}

UnknownPointer UnknownPointer::query_cast(std::size_t id)
{
	return m_pUnknown ? m_pUnknown->QueryInterface(id) : UnknownPointer();
}

} // namespace rdo
