/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/runtime/rdo_object.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      30.01.2007
  \brief     Определение базового класса для всех объектов RDORuntime
  \indent    4T
*/

#ifndef _LIB_RUNTIME_OBJECT_H_
#define _LIB_RUNTIME_OBJECT_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
#include <algorithm>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
#include "utils/rdotypes.h"
#include "simulator/runtime/namespace.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

/*!
  \class     RDORuntimeObject
  \brief     Базовый класс для объектов Runtime
*/
class RDORuntimeObject
{
public:
	RDORuntimeObject();
	virtual ~RDORuntimeObject();
};

/// @todo удалить комментарии

//class RDORuntimeObject
//{
//friend class RDORuntime;
//public:
//	virtual ~RDORuntimeObject();
//	PTR(RDORuntimeParent) getParent() const                         { return m_parent;  }
//	void                  reparent (PTR(RDORuntimeParent) parent);
//	void                  detach   ();
//
////#ifndef _DEBUG
////	PTR(void) operator new   (size_t   sz);
////	void      operator delete(PTR(void) v);
////#endif
//
//protected:
//	RDORuntimeObject(PTR(RDORuntimeParent) parent );
//
//	PTR(RDORuntimeParent) m_parent;
//
//private:
//	size_t        m_object_size; // Размер текущего объекта
//	static size_t s_memory_size; // Сумма размеров всех объектов
//};

// --------------------------------------------------------------------------------
// -------------------- RDORuntimeParent
// --------------------------------------------------------------------------------
//class RDORuntimeParent: public RDORuntimeObject
//{
//public:
//	RDORuntimeParent(PTR(RDORuntimeParent) parent);
//	virtual ~RDORuntimeParent();
//
//	void insertObject(PTR(RDORuntimeObject) object)
//	{
//		if (object)
//		{
////			TRACE( "insert object: %d\n", object );
//			if (object == this)
//			{
////				TRACE( "insert parent himself %d !!!!!!!!!!!!!!!!!!!\n", this );
//			}
//			else
//			{
//				m_childList.push_back(object);
//			}
//		}
//		else
//		{
////			TRACE( "insert object NULL !!!!!!!!!!!!!!!\n" );
//		}
//	}
//	void removeObject(PTR(RDORuntimeObject) object)
//	{
//		ChildList::reverse_iterator it = std::find(m_childList.rbegin(), m_childList.rend(), object);
//		if (it != m_childList.rend())
//		{
////			TRACE( "remove object: %d\n", object );
//			// Команда it.base() приводит реверсивный итератор к нормальному,
//			// но перед этим необходимо сделать инкремент
//			it++;
//			m_childList.erase(it.base());
//		}
//		else
//		{
////			TRACE( "remove object: %d faild !!!!!!!!!!!!!!!!!!!!\n", object );
//		}
//	}
//	void deleteObjects()
//	{
//		ChildList::reverse_iterator it = m_childList.rbegin();
//		while (it != m_childList.rend())
//		{
//			delete *it;
//			it = m_childList.rbegin();
//		}
//		m_childList.clear();
//	}
//
//protected:
//	typedef std::vector<PTR(RDORuntimeObject)> ChildList;
//	ChildList m_childList;
//};

/*!
  \class     RDOSrcInfo
  \brief     Исходная информация
*/
class RDOSrcInfo
{
public:
	/*!
	  \struct    Position
	  \brief     Позиция в тексте
	*/
	struct Position
	{
		static const ruint UNDEFINE_POS  = ruint(~0);
		static const ruint UNDEFINE_LINE = ruint(~0);

		ruint m_first_line;
		ruint m_first_pos;
		ruint m_last_line;
		ruint m_last_pos;

		Position();
		Position(ruint first_line, ruint first_pos, ruint last_line, ruint last_pos);
		rbool empty() const;
		rbool point() const;
	};

	RDOSrcInfo();
	virtual ~RDOSrcInfo();

	void setSrcInfo(CREF(RDOSrcInfo) info);
	void setSrcInfo(CREF(RDOSrcInfo) begin, CREF(tstring) delim, CREF(RDOSrcInfo) end);

	void setSrcPos(CREF(Position) position);
	void setSrcPos(CREF(Position) position_begin, CREF(Position) position_end);
	void setSrcPos(ruint first_line, ruint first_pos, ruint last_line, ruint last_pos);

	virtual void setSrcText(CREF(tstring) value);
	void setSrcFileType(rdoModelObjects::RDOFileType value);

	CREF(RDOSrcInfo)             src_info    () const;
	CREF(Position)               src_pos     () const;
	CREF(tstring)                src_text    () const;
	rdoModelObjects::RDOFileType src_filetype() const;
	rbool                        src_empty   () const;

private:
	Position                     m_position;
	tstring                      m_text_data;
	rdoModelObjects::RDOFileType m_file_type;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_object.inl"

#endif // _LIB_RUNTIME_OBJECT_H_
