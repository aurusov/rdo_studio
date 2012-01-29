/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_res_type_i.h
  \author    ����� ������� (dluschan@rk9.bmstu.ru)
  \date      05.06.2011
  \brief     ��������� ��� ����� ��������
  \details   ���� �������� - ������� ��������. ��. ������� "��������� �����"
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RES_TYPE_I_H_
#define _LIB_RUNTIME_RES_TYPE_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/smart_ptr/interface_ptr.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

PREDECLARE_POINTER(RDORuntime);
PREDECLARE_POINTER(RDOResource);
PREDECLARE_POINTER(RDOValue);

/*!
  \interface IResourceType
  \brief     ������������� ��������� ����� createRes()
*/
OBJECT_INTERFACE(IResourceType)
{
DECLARE_FACTORY(IResourceType);
public:
	virtual rdoRuntime::LPRDOResource createRes(CREF(LPRDORuntime) pRuntime, ruint resID, CREF(std::vector<RDOValue>) paramsCalcs, rbool traceFlag, rbool permanentFlag) = 0;

protected:
	IResourceType()          {}
	virtual ~IResourceType() {}
};

#define DECLARE_IResourceType \
	rdoRuntime::LPRDOResource createRes(CREF(LPRDORuntime) pRuntime, ruint resID, CREF(std::vector<RDOValue>) paramsCalcs, rbool traceFlag, rbool permanentFlag);

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _LIB_RUNTIME_RES_TYPE_I_H_
