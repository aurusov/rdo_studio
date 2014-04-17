/*!
  \copyright (c) RDO-Team, 2011
  \file      simulator/compiler/parser/rdo_object.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

#ifndef _RDOPARSER_OBJECT_H_
#define _RDOPARSER_OBJECT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdomacros.h"
#include "utils/src/smart_ptr/intrusive_ptr/intrusive_ptr.h"
#include "simulator/compiler/parser/namespace.h"
#include "simulator/compiler/parser/grammar/rdobison.h"
#include "simulator/runtime/rdo_object.h"
// --------------------------------------------------------------------------------

OPEN_RDO_PARSER_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOParserSrcInfo
// --------------------------------------------------------------------------------
class RDOParserSrcInfo: public rdo::runtime::RDOSrcInfo
{
public:
	RDOParserSrcInfo();
	RDOParserSrcInfo(const YYLTYPE& pos);
	RDOParserSrcInfo(const rdo::runtime::RDOSrcInfo& info);
	RDOParserSrcInfo(const rdo::runtime::RDOSrcInfo::Position& pos);
	explicit RDOParserSrcInfo(const std::string& text);
	RDOParserSrcInfo(const YYLTYPE& pos, const std::string& text);
	RDOParserSrcInfo(const YYLTYPE& pos_begin, const YYLTYPE& pos_end, bool first_align = false);
	RDOParserSrcInfo(const YYLTYPE& pos_begin, const YYLTYPE& pos_end, const std::string& text);
	virtual ~RDOParserSrcInfo();

	virtual void setSrcInfo(const RDOParserSrcInfo& info)
	{
		setSrcPos     (info.src_pos()     );
		setSrcText    (info.src_text()    );
		setSrcFileType(info.src_filetype());
	}
	void setSrcInfo(const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end)
	{
		RDOParserSrcInfo src_info;
		src_info.setSrcPos     (begin.src_pos().m_first_line, begin.src_pos().m_first_pos, end.src_pos().m_last_line, end.src_pos().m_last_pos);
		src_info.setSrcText    (begin.src_text() + delim + end.src_text());
		src_info.setSrcFileType(begin.src_filetype());
		setSrcInfo(src_info);
	}
	virtual void setSrcPos(const RDOSrcInfo::Position& pos)
	{
		RDOSrcInfo::setSrcPos(pos);
	}
	void setSrcPos(const YYLTYPE& pos)
	{
		setSrcPos(Position(pos.m_first_line, pos.m_first_pos, pos.m_last_line, pos.m_last_pos));
	}
	void setSrcPos(const YYLTYPE& pos_begin, const YYLTYPE& pos_end)
	{
		setSrcPos(Position(pos_begin.m_first_line, pos_begin.m_first_pos, pos_end.m_last_line, pos_end.m_last_pos));
	}
	void setSrcPos(std::size_t first_line, std::size_t first_pos, std::size_t last_line, std::size_t last_pos)
	{
		setSrcPos(Position(first_line, first_pos, last_line, last_pos));
	}
	YYLTYPE getPosAsYY() const
	{
		YYLTYPE  pos1;
		Position pos2 = src_pos();
		pos1.m_first_line = pos2.m_first_line;
		pos1.m_first_pos  = pos2.m_first_pos;
		pos1.m_last_line  = pos2.m_last_line;
		pos1.m_last_pos   = pos2.m_last_pos;
		pos1.m_first_seek = std::size_t(Position::UNDEFINE_POS);
		pos1.m_last_seek  = std::size_t(Position::UNDEFINE_POS);
		return pos1;
	}
	static std::size_t getPosByLength(std::size_t pos, const std::string& text)
	{
		return pos + text.length();
	}

private:
	void init();
};

template <class T>
class compareName
{
public:
	compareName(const compareName<T>& obj)
		: m_name(obj.m_name)
	{}
	compareName(const std::string& name)
		: m_name(name)
	{}
	compareName<T>& operator= (const compareName<T>& obj)
	{
		m_name = obj.m_name;
		return *this;
	}
	bool operator()(const T* pObj)
	{
		return pObj->name() == m_name;
	}
	bool operator() (const rdo::intrusive_ptr<T>& pObj)
	{
		return pObj->name() == m_name;
	}

private:
	const std::string& m_name;
};

template <class T>
class compareNameRef
{
public:
	compareNameRef(const compareNameRef<T>& obj)
		: m_name(obj.m_name)
	{}
	compareNameRef(const std::string& name)
		: m_name(name)
	{}
	compareNameRef<T>& operator= (const compareNameRef<T>& obj)
	{
		m_name = obj.m_name;
		return *this;
	}
	bool operator() (const T& obj)
	{
		return obj.name() == m_name;
	}

private:
	const std::string& m_name;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif // _RDOPARSER_OBJECT_H_
