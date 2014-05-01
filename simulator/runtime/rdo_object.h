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
#include "utils/src/common/rdocommon.h"
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
//	RDORuntimeParent* getParent() const { return m_parent;  }
//	void reparent(RDORuntimeParent* parent);
//	void detach();
//
////#ifndef _DEBUG
////	void* operator new(size_t sz);
////	void operator delete(void* v);
////#endif
//
//protected:
//	RDORuntimeObject(RDORuntimeParent* parent );
//
//	RDORuntimeParent* m_parent;
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
//	RDORuntimeParent(RDORuntimeParent* parent);
//	virtual ~RDORuntimeParent();
//
//	void insertObject(RDORuntimeObject* object)
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
//	void removeObject(RDORuntimeObject* object)
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
//	typedef std::vector<RDORuntimeObject*> ChildList;
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
		static const std::size_t UNDEFINE_POS  = std::size_t(~0);
		static const std::size_t UNDEFINE_LINE = std::size_t(~0);

		std::size_t m_first_line;
		std::size_t m_first_pos;
		std::size_t m_last_line;
		std::size_t m_last_pos;

		Position();
		Position(std::size_t first_line, std::size_t first_pos, std::size_t last_line, std::size_t last_pos);
		bool empty() const;
		bool point() const;
	};

	RDOSrcInfo();
	virtual ~RDOSrcInfo();

	void setSrcInfo(const RDOSrcInfo& info);
	void setSrcInfo(const RDOSrcInfo& begin, const std::string& delim, const RDOSrcInfo& end);

	void setSrcPos(const Position& position);
	void setSrcPos(const Position& position_begin, const Position& position_end);
	void setSrcPos(std::size_t first_line, std::size_t first_pos, std::size_t last_line, std::size_t last_pos);

	virtual void setSrcText(const std::string& value);
	void setSrcFileType(rdoModelObjects::RDOFileType value);

	const RDOSrcInfo& src_info() const;
	const Position& src_pos() const;
	const std::string& src_text() const;
	rdoModelObjects::RDOFileType src_filetype() const;
	bool src_empty() const;

private:
	Position m_position;
	std::string m_text_data;
	rdoModelObjects::RDOFileType m_file_type;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_object-inl.h"

#endif // _LIB_RUNTIME_OBJECT_H_
