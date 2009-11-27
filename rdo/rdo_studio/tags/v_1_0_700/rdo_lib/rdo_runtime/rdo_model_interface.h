/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdo_model_interface.h
 * author   : Урусов Андрей
 * date     : 28.07.2009
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDO_MODEL_INTERFACE_H_
#define _RDO_MODEL_INTERFACE_H_

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include <rdointerface.h>
// ===============================================================================

class IModelStructure
{
public:
	virtual void writeModelStructure(REF(std::ostream) stream) const = 0;
};
#define DECLARE_IModelStructure \
	virtual void writeModelStructure(REF(std::ostream) stream) const;

class IName
{
public:
	virtual CREF(tstring) name() const = 0;
};
#define DECLARE_IName \
	virtual CREF(tstring) name() const;

#endif //! _RDO_MODEL_INTERFACE_H_
