/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_activity_interface.h
 * author   : Урусов Андрей
 * date     : 01.08.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_ACTIVITY_INTERFACE_H_
#define _RDO_ACTIVITY_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <rdointerface.h>
// ===============================================================================

class IActivity
{
public:
	virtual void addParamCalc  (PTR(rdoRuntime::RDOCalc) calc)  = 0;
	virtual int  getResByRelRes(ruint rel_res_id) const         = 0;
	virtual void setRelRes     (ruint rel_res_id, ruint res_id) = 0;

};
#define DECLARE_IActivity \
	virtual void addParamCalc  (PTR(rdoRuntime::RDOCalc) calc);  \
	virtual int  getResByRelRes(ruint rel_res_id) const;         \
	virtual void setRelRes     (ruint rel_res_id, ruint res_id);

#endif //! _RDO_PRIORITY_INTERFACE_H_
