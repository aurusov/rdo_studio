/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdotraceroperation.h
  \author    Захаров Павел
  \date      11.03.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_TRACER_RDOTRACEROPERATION_H_
#define _RDO_STUDIO_MFC_TRACER_RDOTRACEROPERATION_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio_mfc/rdo_tracer/rdotracerserie.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOTracerOperationBase
// --------------------------------------------------------------------------------
class RDOTracerPattern;

class RDOTracerOperationBase: public RDOTracerSerie
{
protected:
	RDOTracerPattern* pattern;
	tstring Name;
	void incOperationsCount( RDOTracerTimeNow* const time, const int eventIndex );
public:
	RDOTracerOperationBase( RDOTracerPattern* const pat );
	virtual ~RDOTracerOperationBase();

	RDOTracerPattern* getPattern() const { return pattern; };
	//void start( RDOTracerTimeNow* const time, const int eventIndex );
	//void accomplish( RDOTracerTimeNow* const time, const int eventIndex );
	void setName( CREF(tstring) name ) { Name = name; if ( title.empty() ) title = Name; };
	const tstring getName() const { return Name; };
	virtual void getCaptions( std::vector<tstring> &captions, const int val_count ) const;
	virtual void monitorTime( RDOTracerTimeNow* const time, const int eventIndex );
};

// --------------------------------------------------------------------------------
// -------------------- RDOTracerOperation
// --------------------------------------------------------------------------------
class RDOTracerOperation: public RDOTracerOperationBase
{
public:
	RDOTracerOperation( RDOTracerPattern* const pat );
	virtual ~RDOTracerOperation();

	void start( RDOTracerTimeNow* const time, const int eventIndex );
	void accomplish( RDOTracerTimeNow* const time, const int eventIndex );
};

// --------------------------------------------------------------------------------
// -------------------- RDOTracerEvent
// --------------------------------------------------------------------------------
class RDOTracerEvent: public RDOTracerOperationBase
{
public:
	RDOTracerEvent( RDOTracerPattern* const pat );
	virtual ~RDOTracerEvent();
	
	void occurs( RDOTracerTimeNow* const time, const int eventIndex );
	virtual void monitorTime( RDOTracerTimeNow* const time, const int eventIndex );
};

#endif // _RDO_STUDIO_MFC_TRACER_RDOTRACEROPERATION_H_
