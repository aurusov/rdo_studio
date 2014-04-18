/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_type_param_suchas.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      09.01.2011
  \brief     
  \indent    4T
*/

#ifndef _CONVERTOR_RDO_TYPE_PARAM_SUCHAS_H_
#define _CONVERTOR_RDO_TYPE_PARAM_SUCHAS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdo_type_param.h"
#include "converter/smr2rdox/param.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOTypeParamSuchAs
// --------------------------------------------------------------------------------
class RDOTypeParamSuchAs: public RDOTypeParam
{
DECLARE_FACTORY(RDOTypeParamSuchAs);
public:
	const LPRDOParam& getParam() const;

private:
	RDOTypeParamSuchAs(const LPRDOParam& pParam);
	virtual ~RDOTypeParamSuchAs();

	LPRDOParam m_pParam;
};
DECLARE_POINTER(RDOTypeParamSuchAs);

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE

#endif // _CONVERTOR_RDO_TYPE_PARAM_SUCHAS_H_
