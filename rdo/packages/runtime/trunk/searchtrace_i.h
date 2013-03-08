/*!
  \copyright (c) RDO-Team, 2011
  \file      searchtrace_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      30.07.2009
  \brief     Интерфейс IDPTSearchTraceStatistics
  \indent    4T
*/

#ifndef _LIB_RUNTIME_SEARCH_TRACE_I_H_
#define _LIB_RUNTIME_SEARCH_TRACE_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
// --------------------------------------------------------------------------------

/*!
  \interface IDPTSearchTraceStatistics
  \brief     Интерфейс IDPTSearchTraceStatistics
*/
class IDPTSearchTraceStatistics
{
public:
	/*!
	  \enum      Type
	  \brief     Тип статистики точки принятия решений DPTSearch
	*/
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

#endif // _LIB_RUNTIME_SEARCH_TRACE_I_H_
