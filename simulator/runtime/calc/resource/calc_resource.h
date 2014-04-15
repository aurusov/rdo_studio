/*!
  \copyright (c) RDO-Team, 2011
  \file      calc_resource.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      19.12.2011
  \brief     Калки для ресурсов
  \indent    4T
*/

#ifndef _LIB_RUNTIME_CALC_RESOURCE_H_
#define _LIB_RUNTIME_CALC_RESOURCE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/calc/calc_base.h"
#include "simulator/runtime/set_operation_type.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Вспомогательный класс для вытаскивания ресурса по ID и оборачивания его в RDOValue
class RDOCalcGetResourceHelper
{
public:
	static bool getResource(CREF(LPRDORuntime) pRuntime, std::size_t resourceID, RDOValue& result);
};

//! Получение ресурса по ID
PREDECLARE_POINTER(RDOCalcGetResourceByID);
class RDOCalcGetResourceByID: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetResourceByID)
private:
	RDOCalcGetResourceByID(CREF(std::size_t) resourceID);

	std::size_t m_resourceID;

	DECLARE_ICalc;
};

//! Получение параметр ресурса по калку ресурса и ID параметра
PREDECLARE_POINTER(RDOCalcGetResourceParam);
class RDOCalcGetResourceParam: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetResourceParam)
private:
	RDOCalcGetResourceParam(CREF(LPRDOCalc) pResource, std::size_t paramID);

	LPRDOCalc m_pResource;
	int m_paramID;

	DECLARE_ICalc;
};

//! Параметры несуществующего ресурса
PREDECLARE_POINTER(RDOCalcGetUnknowResParam);
class RDOCalcGetUnknowResParam: public RDOCalc
{
DECLARE_FACTORY(RDOCalcGetUnknowResParam)
private:
	RDOCalcGetUnknowResParam(CREF(std::string) resName, CREF(std::string) parName);

	std::string m_resName;
	std::string m_parName;

	DECLARE_ICalc;
};

//! Установка значения параметра ресурса перед моделированием из SMR
PREDECLARE_POINTER(RDOSetResourceParamCalc);
class RDOSetResourceParamCalc: public RDOCalc
{
DECLARE_FACTORY(RDOSetResourceParamCalc)
private:
	RDOSetResourceParamCalc(std::size_t resourceID, std::size_t paramID, CREF(LPRDOCalc) pCalc);

	std::size_t m_resourceID;
	std::size_t m_paramID;
	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

PREDECLARE_POINTER(RDOCalcSetResourceTrace);
class RDOCalcSetResourceTrace: public RDOCalc
{
DECLARE_FACTORY(RDOCalcSetResourceTrace)
private:
	RDOCalcSetResourceTrace(const LPRDOCalc& getResource, bool traceValue);

	LPRDOCalc m_getResource;
	bool  m_traceValue;

	DECLARE_ICalc;
};

template <SetOperationType::Type setOperationType>
class RDOSetResourceParam: public RDOCalc
{
public:
	RDOSetResourceParam(const LPRDOCalc& getResource, const std::size_t paramID, const LPRDOCalc& pCalc = NULL);
	virtual ~RDOSetResourceParam();

protected:
	LPRDOCalc m_getResource;
	std::size_t m_paramID;
	LPRDOCalc m_pCalc;

	DECLARE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/calc/resource/calc_resource.inl"

#endif // _LIB_RUNTIME_CALC_RESOURCE_H_
