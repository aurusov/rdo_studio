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
#include "rdo_lib/rdo_converter/namespace.h"
#include "rdo_lib/rdo_converter/rdobison.h"
#include "rdo_lib/rdo_converter/rdogramma.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_common/rdomacros.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

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
	explicit RDOParserSrcInfo(CREF(tstring) text);
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
		setSrcPos(Position(pos.m_first_line, pos.m_first_pos, pos.m_last_line, pos.m_last_pos));
	}
	void setSrcPos(CREF(YYLTYPE) pos_begin, CREF(YYLTYPE) pos_end)
	{
		setSrcPos(Position(pos_begin.m_first_line, pos_begin.m_first_pos, pos_end.m_last_line, pos_end.m_last_pos));
	}
	void setSrcPos(ruint first_line, ruint first_pos, ruint last_line, ruint last_pos)
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
		pos1.m_first_seek = Position::UNDEFINE_POS;
		pos1.m_last_seek  = Position::UNDEFINE_POS;
		return pos1;
	}
	static ruint getPosByLength(ruint pos, CREF(tstring) text)
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
	compareName(CREF(tstring) name)
		: m_name(name)
	{}
	rbool operator() (CPTR(T) obj)
	{
		return obj->name() == m_name;
	}
	rbool operator() (CREF(rdo::intrusive_ptr<T>) pObj)
	{
		return pObj->name() == m_name;
	}

private:
	CREF(tstring) m_name;
};

template <class T>
class compareNameRef
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

CLOSE_RDO_CONVERTER_NAMESPACE

#endif //! _RDOPARSER_OBJECT_H_
