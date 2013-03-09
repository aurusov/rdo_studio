/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_object.cpp
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      30.01.2007
  \brief     Определение базового класса для всех объектов RDORuntime
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_object.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/// @todo удалить комментарии

// --------------------------------------------------------------------------------
// -------------------- RDORuntimeObject
// --------------------------------------------------------------------------------
//size_t RDORuntimeObject::s_memory_size = 0;
//
//RDORuntimeObject::RDORuntimeObject(PTR(RDORuntimeParent) parent)
//	: m_parent(parent)
//{
//	if (m_parent)
//	{
//		m_parent->insertObject(this);
//	}
//	else
//	{
////		TRACE( "========= no parent for %d\n", this );
//	}
//}
//
//RDORuntimeObject::~RDORuntimeObject()
//{
////	TRACE( "destroy %d\n", this );
//	if (m_parent)
//	{
//		m_parent->removeObject(this);
//	}
//}
//
//void RDORuntimeObject::detach()
//{
//	if (m_parent)
//	{
//		m_parent->removeObject(this);
//		m_parent = NULL;
//	}
//}
//
//void RDORuntimeObject::reparent(PTR(RDORuntimeParent) parent)
//{
//	if (m_parent != parent)
//	{
//		if (m_parent)
//		{
//			m_parent->removeObject(this);
//		}
//		m_parent = parent;
//		if (m_parent)
//		{
//			m_parent->insertObject(this);
//		}
//	}
//}

//#ifndef _DEBUG
//PTR(void) RDORuntimeObject::operator new(size_t sz)
//{
//	s_memory_size += sz;
//	PTR(RDORuntimeObject) obj = static_cast<PTR(RDORuntimeObject)>(::operator new(sz));
//	obj->m_object_size = sz;
////	obj->pRuntime->memory_insert(sz);
//	return obj;
//}
//
//void RDORuntimeObject::operator delete(PTR(void) v)
//{
//	s_memory_size -= static_cast<PTR(RDORuntimeObject)>(v)->m_object_size;
//	::operator delete(v);
//}
//#endif

// --------------------------------------------------------------------------------
// -------------------- RDORuntimeParent
// --------------------------------------------------------------------------------
//RDORuntimeParent::RDORuntimeParent(PTR(RDORuntimeParent) parent)
//	: RDORuntimeObject(parent)
//{}
//
//RDORuntimeParent::~RDORuntimeParent()
//{
//	deleteObjects();
//}

CLOSE_RDO_RUNTIME_NAMESPACE
