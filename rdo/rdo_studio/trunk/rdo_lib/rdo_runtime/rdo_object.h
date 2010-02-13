/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdoruntime_object.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDORUNTIME_OBJECT_H_
#define _RDORUNTIME_OBJECT_H_

// ====================================================================== INCLUDES
#include <vector>
#include <algorithm>
// ====================================================================== SYNOPSIS
#include "rdo_common/rdocommon.h"
#include "rdo_common/rdotypes.h"
// ===============================================================================

OPEN_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDORuntimeObject
// ----------------------------------------------------------------------------
class RDORuntime;
class RDORuntimeParent;

class RDORuntimeObject
{
friend class RDORuntime;
public:
	virtual ~RDORuntimeObject();
	PTR(RDORuntimeParent) getParent() const                         { return m_parent;  }
	void                  reparent (PTR(RDORuntimeParent) parent);
	void                  detach   ();

//#ifndef _DEBUG
//	PTR(void) operator new   (size_t   sz);
//	void      operator delete(PTR(void) v);
//#endif

protected:
	RDORuntimeObject(PTR(RDORuntimeParent) parent );

	PTR(RDORuntimeParent) m_parent;

private:
	size_t        m_object_size; // Размер текущего объекта
	static size_t s_memory_size; // Сумма размеров всех объектов
};

// ----------------------------------------------------------------------------
// ---------- RDORuntimeContainer
// ----------------------------------------------------------------------------
class RDORuntimeContainer
{
protected:
	RDORuntimeContainer(PTR(RDORuntime) runtime)
		: m_runtime(runtime)
	{}

	PTR(RDORuntime) getRuntime() const                  { return m_runtime;    }
	void            setRuntime(PTR(RDORuntime) runtime) { m_runtime = runtime; }

private:
	PTR(RDORuntime) m_runtime;
};

// ----------------------------------------------------------------------------
// ---------- RDORuntimeParent
// ----------------------------------------------------------------------------
class RDORuntimeParent: public RDORuntimeObject
{
public:
	RDORuntimeParent(PTR(RDORuntimeParent) parent);
	virtual ~RDORuntimeParent();

	void insertObject(PTR(RDORuntimeObject) object)
	{
		if (object)
		{
//			TRACE( "insert object: %d\n", object );
			if (object == this)
			{
//				TRACE( "insert parent himself %d !!!!!!!!!!!!!!!!!!!\n", this );
			}
			else
			{
				m_childList.push_back(object);
			}
		}
		else
		{
//			TRACE( "insert object NULL !!!!!!!!!!!!!!!\n" );
		}
	}
	void removeObject(PTR(RDORuntimeObject) object)
	{
		ChildList::reverse_iterator it = std::find(m_childList.rbegin(), m_childList.rend(), object);
		if (it != m_childList.rend())
		{
//			TRACE( "remove object: %d\n", object );
			// Команда it.base() приводит реверсивный итератор к нормальному,
			// но перед этим необходимо сделать инкремент
			it++;
			m_childList.erase(it.base());
		}
		else
		{
//			TRACE( "remove object: %d faild !!!!!!!!!!!!!!!!!!!!\n", object );
		}
	}
	void deleteObjects()
	{
		ChildList::reverse_iterator it = m_childList.rbegin();
		while (it != m_childList.rend())
		{
			delete *it;
			it = m_childList.rbegin();
		}
		m_childList.clear();
	}

protected:
	typedef std::vector<PTR(RDORuntimeObject)> ChildList;
	ChildList m_childList;
};

// ----------------------------------------------------------------------------
// ---------- RDOSrcInfo
// ----------------------------------------------------------------------------
class RDOSrcInfo
{
public:
	struct Position
	{
		enum { UNDEFINE_POS  = ~0 };
		enum { UNDEFINE_LINE = ~0 };

		ruint m_first_line;
		ruint m_first_pos;
		ruint m_last_line;
		ruint m_last_pos;

		Position::Position()
			: m_first_line(UNDEFINE_LINE)
			, m_first_pos (UNDEFINE_POS )
			, m_last_line (UNDEFINE_LINE)
			, m_last_pos  (UNDEFINE_POS )
		{}
		Position::Position(ruint first_line, ruint first_pos, ruint last_line, ruint last_pos)
			: m_first_line(first_line)
			, m_first_pos (first_pos )
			, m_last_line (last_line )
			, m_last_pos  (last_pos  )
		{}
		rbool empty() const
		{
			return m_first_line == UNDEFINE_LINE && m_first_pos == UNDEFINE_POS && m_last_line == UNDEFINE_LINE && m_last_pos == UNDEFINE_POS;
		}
		rbool point() const
		{
			return m_first_line == m_last_line && m_first_pos == m_last_pos;
		}
	};

	RDOSrcInfo()
		: m_text_data(_T("")              )
		, m_file_type(rdoModelObjects::TRC)
	{}
	void setSrcInfo(CREF(RDOSrcInfo) info)
	{
		setSrcPos     (info.m_position );
		setSrcText    (info.m_text_data);
		setSrcFileType(info.m_file_type);
	}
	void setSrcInfo(CREF(RDOSrcInfo) begin, CREF(tstring) delim, CREF(RDOSrcInfo) end)
	{
		setSrcPos     (begin.src_pos().m_first_line, begin.src_pos().m_first_pos, end.src_pos().m_last_line, end.src_pos().m_last_pos);
		setSrcText    (begin.src_text() + delim + end.src_text());
		setSrcFileType(begin.src_filetype());
	}
	void setSrcPos(CREF(Position) position)
	{
		m_position.m_first_line = position.m_first_line;
		m_position.m_first_pos  = position.m_first_pos;
		m_position.m_last_line  = position.m_last_line;
		m_position.m_last_pos   = position.m_last_pos;
	}
	void setSrcPos(CREF(Position) position_begin, CREF(Position) position_end)
	{
		m_position.m_first_line = position_begin.m_first_line;
		m_position.m_first_pos  = position_begin.m_first_pos;
		m_position.m_last_line  = position_end.m_last_line;
		m_position.m_last_pos   = position_end.m_last_pos;
	}
	void setSrcPos(ruint first_line, ruint first_pos, ruint last_line, ruint last_pos)
	{
		m_position.m_first_line = first_line;
		m_position.m_first_pos  = first_pos;
		m_position.m_last_line  = last_line;
		m_position.m_last_pos   = last_pos;
	}
	virtual void setSrcText(CREF(tstring) value)
	{
		m_text_data = value;
	}
	void setSrcFileType(rdoModelObjects::RDOFileType value)
	{
		m_file_type = value;
	}
	CREF(RDOSrcInfo)             src_info    () const { return *this;       }
	CREF(Position)               src_pos     () const { return m_position;  }
	CREF(tstring)                src_text    () const { return m_text_data; }
	rdoModelObjects::RDOFileType src_filetype() const { return m_file_type; }
	rbool                        src_empty   () const
	{
		return m_position.empty() && m_text_data.empty() && m_file_type == rdoModelObjects::TRC;
	}

private:
	Position                     m_position;
	tstring                      m_text_data;
	rdoModelObjects::RDOFileType m_file_type;
};

CLOSE_RDO_RUNTIME_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- INotify
// ----------------------------------------------------------------------------
class INotify
{
public:
	virtual void notify(ruint message, PTR(void) param) = 0;
};
#define DECLARE_INotify \
	virtual void notify(ruint message, PTR(void) param);

#endif //! _RDORUNTIME_OBJECT_H_
