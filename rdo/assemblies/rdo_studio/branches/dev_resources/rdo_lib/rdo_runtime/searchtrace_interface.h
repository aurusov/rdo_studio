/*
 * copyright: (c) RDO-Team, 2009
 * filename : searchtrace_interface.h
 * author   : Урусов Андрей
 * date     : 30.07.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _SEARCHTRACE_INTERFACE_H_
#define _SEARCHTRACE_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <rdointerface.h>
// ===============================================================================

class IDPTSearchTraceStatistics
{
public:
	enum Type
	{
		ST_TIMES,
		ST_COST,
		ST_MEMORY,
		ST_NODES,
		ST_NODES_EXPENDED,
		ST_NODES_FULL,
		ST_NODES_IN_GRAPH
	};
	virtual ruint getCalcCnt        () const = 0;
	virtual ruint getCalcResFoundCnt() const = 0;
	virtual void  getStatsDOUBLE    (Type type, REF(double) min, REF(double) max, REF(double) med) const = 0;
	virtual void  getStatsRUINT     (Type type, REF(ruint)  min, REF(ruint)  max, REF(double) med) const = 0;
};
#define DECLARE_IDPTSearchTraceStatistics                                                                 \
	virtual ruint getCalcCnt        () const;                                                             \
	virtual ruint getCalcResFoundCnt() const;                                                             \
	virtual void  getStatsDOUBLE    (Type type, REF(double) min, REF(double) max, REF(double) med) const; \
	virtual void  getStatsRUINT     (Type type, REF(ruint)  min, REF(ruint)  max, REF(double) med) const;

#endif //! _SEARCHTRACE_INTERFACE_H_
