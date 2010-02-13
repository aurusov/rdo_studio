/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_object.h
 * author   : Александ Барс, Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOPARSER_OBJECT_H_
#define _RDOPARSER_OBJECT_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/namespace.h"
#include "rdo_lib/rdo_parser/rdogramma.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
// ===============================================================================

OPEN_RDO_PARSER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- RDODeletable
// ----------------------------------------------------------------------------
class RDOParser;

class RDODeletable
{
public:
	RDODeletable(PTR(RDOParser) parser);
	virtual ~RDODeletable();

	PTR(RDOParser) parser() const { return m_parser; }

	void noAutoDelete();

//#ifndef _DEBUG
//	PTR(void) operator new   (size_t   sz);
//	void      operator delete(PTR(void) v);
//#endif

protected:
	PTR(RDOParser) m_parser;

private:
	size_t m_object_size;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserObject
// ----------------------------------------------------------------------------
class RDOParser;

class RDOParserObject: public RDODeletable
{
public:
	CPTR(RDOParserObject) parent  () const                         { return m_parent; }
	void                  reparent(CPTR(RDOParserObject) parent);
	virtual ~RDOParserObject();

protected:
	RDOParserObject(PTR(RDOParser)        parser);
	RDOParserObject(CPTR(RDOParserObject) parent);

private:
	CPTR(RDOParserObject) m_parent;
};

// ----------------------------------------------------------------------------
// ---------- RDOParserSrcInfo
// ----------------------------------------------------------------------------
class RDOParserSrcInfo: public rdoRuntime::RDOSrcInfo
{
public:
	RDOParserSrcInfo();
	RDOParserSrcInfo(CREF(YYLTYPE) pos);
	RDOParserSrcInfo(CREF(rdoRuntime::RDOSrcInfo) info);
	RDOParserSrcInfo(CREF(rdoRuntime::RDOSrcInfo::Position) pos);
	RDOParserSrcInfo(CREF(tstring) text);
	RDOParserSrcInfo(CREF(YYLTYPE) pos, CREF(tstring) text);
	RDOParserSrcInfo(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end, rbool first_align = false);
	RDOParserSrcInfo(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end, CREF(tstring) text);

	virtual void setSrcInfo(CREF(RDOParserSrcInfo) info)
	{
		setSrcPos     (info.src_pos()     );
		setSrcText    (info.src_text()    );
		setSrcFileType(info.src_filetype());
	}
	void setSrcInfo(CREF(RDOParserSrcInfo) begin, CREF(tstring) delim, CREF(RDOParserSrcInfo) end)
	{
		RDOParserSrcInfo src_info;
		src_info.setSrcPos     (begin.src_pos().m_first_line, begin.src_pos().m_first_pos, end.src_pos().m_last_line, end.src_pos().m_last_pos);
		src_info.setSrcText    (begin.src_text() + delim + end.src_text());
		src_info.setSrcFileType(begin.src_filetype());
		setSrcInfo(src_info);
	}
	virtual void setSrcPos(CREF(RDOSrcInfo::Position) pos)
	{
		RDOSrcInfo::setSrcPos(pos);
	}
	void setSrcPos(CREF(YYLTYPE) pos)
	{
		setSrcPos(RDOSrcInfo::Position(pos.first_line, pos.first_column, pos.last_line, pos.last_column));
	}
	void setSrcPos(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end)
	{
		setSrcPos(RDOSrcInfo::Position(pos_begin.first_line, pos_begin.first_column, pos_end.last_line, pos_end.last_column));
	}
	void setSrcPos(ruint first_line, ruint first_pos, ruint last_line, ruint last_pos)
	{
		setSrcPos(RDOSrcInfo::Position(first_line, first_pos, last_line, last_pos));
	}
	YYLTYPE getPosAsYY() const
	{
		YYLTYPE pos1;
		rdoRuntime::RDOSrcInfo::Position pos2 = src_pos();
		pos1.first_line   = pos2.m_first_line;
		pos1.first_column = pos2.m_first_pos;
		pos1.last_line    = pos2.m_last_line;
		pos1.last_column  = pos2.m_last_pos;
		return pos1;
	}
	static ruint getPosByLength(ruint pos, CREF(tstring) text)
	{
		return pos + text.length();
	}

private:
	void init();
};

template <class T> class compareName
{
public:
	compareName(CREF(tstring) name)
		: m_name(name)
	{}
	rbool operator() (CPTR(T) obj)
	{
		return obj->name() == m_name;
	}

private:
	CREF(tstring) m_name;
};

template <class T> class compareNameRef
{
public:
	compareNameRef(CREF(tstring) name)
		: m_name(name)
	{}
	rbool operator() (CREF(T) obj)
	{
		return obj.name() == m_name;
	}

private:
	CREF(tstring) m_name;
};

CLOSE_RDO_PARSER_NAMESPACE

#endif //! _RDOPARSER_OBJECT_H_
