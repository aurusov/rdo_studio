/******************************************************************************//**
 * @copyright (c) RDO-Team, 2007
 * @file      rdocalc.h
 * @authors   Барс Александр, Урусов Андрей, Лущан Дмитрий
 * @date      16.05.2007
 * @brief     Вычислитель
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_CALC_H_
#define _LIB_RUNTIME_CALC_H_

// *********************************************************************** INCLUDES
#include <list>
// *********************************************************************** SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/smart_ptr/intrusive_ptr.h"
#include "rdo_common/smart_ptr/interface_ptr.h"
#include "rdo_lib/rdo_runtime/rdo_value.h"
#include "rdo_lib/rdo_runtime/rdo_object.h"
#include "rdo_lib/rdo_runtime/rdo_random_distribution.h"
#include "rdo_lib/rdo_runtime/rdo_memory.h"
#include "rdo_lib/rdo_runtime/rdo_res_type.h"
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

/******************************************************************************//**
 * @class   RDOCalc
 * @brief   "Вычислитель" системы
 *********************************************************************************/
OBJECT(RDOCalc) IS INSTANCE_OF(RDOSrcInfo)
{
DECLARE_FACTORY(RDOCalc)

public:
	REF(RDOValue) calcValue(CREF(LPRDORuntime) pRuntime);

	virtual rbool compare(CREF(LPRDOCalc) pCalc) const;

protected:
	RDOCalc();
	virtual ~RDOCalc();

	RDOValue m_value;

	virtual REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime) = 0;
};

/******************************************************************************//**
 * @def     DECALRE_ICalc
 * @brief   Декларация интерфейса ICalc
 *********************************************************************************/
#define DECALRE_ICalc \
private:              \
	REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);

/******************************************************************************//**
 * @def     CALC_SUB
 * @brief   Калки-потомки
 *********************************************************************************/
#define CALC_SUB(TYPE, PARENT) \
PREDECLARE_POINTER(TYPE);      \
class TYPE: public PARENT

/******************************************************************************//**
 * @def     CALC
 * @brief   Калки-потомки от RDOCalc
 *********************************************************************************/
#define CALC(TYPE) CALC_SUB(TYPE, RDOCalc)

/******************************************************************************//**
 * @class   RDOCalcConst
 * @brief   Константы
 *********************************************************************************/
CALC(RDOCalcConst)
{
DECLARE_FACTORY(RDOCalcConst)
public:
	CREF(RDOValue) getValue() const;

private:
	RDOCalcConst(CREF(RDOValue) value);
	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcGetResParam
 * @brief   Параметры постоянного ресурса
 *********************************************************************************/
CALC(RDOCalcGetResParam)
{
DECLARE_FACTORY(RDOCalcGetResParam)
protected:
	RDOCalcGetResParam(int _resNumb, int _parNumb);

	int m_resID;
	int m_paramID;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcGetTempResParamFRM
 * @brief   Параметры временного ресурса для FRM
 *********************************************************************************/
CALC_SUB(RDOCalcGetTempResParamFRM, RDOCalcGetResParam)
	AND IMPLEMENTATION_OF(INotify)
{
DECLARE_FACTORY(RDOCalcGetTempResParamFRM)
private:
	RDOCalcGetTempResParamFRM(int _resNumb, int _parNumb);
	virtual REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime);

	virtual void notify(ruint message, PTR(void) param);
};

/******************************************************************************//**
 * @class   RDOCalcGetUnknowResParam
 * @brief   Параметры несуществующего ресурса
 *********************************************************************************/
CALC(RDOCalcGetUnknowResParam)
{
DECLARE_FACTORY(RDOCalcGetUnknowResParam)
private:
	RDOCalcGetUnknowResParam(CREF(tstring) resName, CREF(tstring) parName);

	tstring m_resName;
	tstring m_parName;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcGetGroupResParam
 * @brief   Получение группы параметров ресурсов
 * @todo    Что это?
 *********************************************************************************/
CALC(RDOCalcGetGroupResParam)
{
DECLARE_FACTORY(RDOCalcGetGroupResParam)
private:
	RDOCalcGetGroupResParam(int parNumb);

	int m_parNumb;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOSetResourceParamCalc
 * @brief   Сохранение калки для ресурса паттерна
 *********************************************************************************/
CALC(RDOSetResourceParamCalc)
{
DECLARE_FACTORY(RDOSetResourceParamCalc)
private:
	RDOSetResourceParamCalc(int resNumb, int parNumb, CREF(LPRDOCalc) pCalc);

	int        m_resNumb;
	int        m_parNumb;
	LPRDOCalc  m_pCalc;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOSetPatternParamCalc
 * @brief   Сохранение калки для параметра паттерна
 *********************************************************************************/
CALC(RDOSetPatternParamCalc)
{
DECLARE_FACTORY(RDOSetPatternParamCalc)
private:
	RDOSetPatternParamCalc(int parNumb, RDOValue val);

	int      m_parNumb;
	RDOValue m_val;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcPatParam
 * @brief   Параметр паттерна
 *********************************************************************************/
CALC(RDOCalcPatParam)
{
DECLARE_FACTORY(RDOCalcPatParam)
private:
	RDOCalcPatParam(int numberOfParam);

	int m_numberOfParam;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcGetTimeNow
 * @brief   Получение текущего модельного времени
 *********************************************************************************/
CALC(RDOCalcGetTimeNow)
{
DECLARE_FACTORY(RDOCalcGetTimeNow)
DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcGetSeconds
 * @brief   Получение реального времени работы модели
 *********************************************************************************/
CALC(RDOCalcGetSeconds)
{
DECLARE_FACTORY(RDOCalcGetSeconds)
DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcGetTermNow
 * @brief   Получение значения терминального счетчика
 *********************************************************************************/
CALC(RDOCalcGetTermNow)
{
DECLARE_FACTORY(RDOCalcGetTermNow)
DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOFunCalc
 * @brief   Функция
 *********************************************************************************/
CALC(RDOFunCalc)
{
public:
	virtual void addRetCalc(CREF(LPRDOCalc) pCalc);

protected:
	RDOFunCalc();
};

/******************************************************************************//**
 * @class   RDOFuncTableCalc
 * @brief   Функция типа таблица
 *********************************************************************************/
CALC_SUB(RDOFuncTableCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFuncTableCalc)
public:
	void addResultCalc(CREF(LPRDOCalcConst) res);

private:
	RDOFuncTableCalc(CREF(LPRDOCalc) pArgCalc);

	std::vector<LPRDOCalcConst> m_results;
	LPRDOCalc                   m_pArgCalc;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOFunListCalc
 * @brief   Функция типа список
 *********************************************************************************/
CALC_SUB(RDOFunListCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFunListCalc)
public:
	void addCase(CREF(LPRDOCalc) pCaseCalc, CREF(LPRDOCalcConst) pResultCalc);

private:
	RDOFunListCalc(CREF(LPRDOCalcConst) pDefaultValue);

	std::vector<LPRDOCalc>       m_cases;
	std::vector<LPRDOCalcConst>  m_results;
	LPRDOCalcConst               m_pDefaultValue;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOFunAlgorithmicCalc
 * @brief   Алгоритмическая функция
 *********************************************************************************/
CALC_SUB(RDOFunAlgorithmicCalc, RDOFunCalc)
{
DECLARE_FACTORY(RDOFunAlgorithmicCalc)
public:
	void addCalcIf(CREF(LPRDOCalc) pCond, CREF(LPRDOCalc) pAct);

protected:
	RDOFunAlgorithmicCalc();

	std::vector<LPRDOCalc> m_conditions;
	std::vector<LPRDOCalc> m_actions;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOFunAlgorithmicDiapCalc
 * @brief   Алгоритмическая функция с проверкой на диапазон возвращаемого значения
 *********************************************************************************/
CALC_SUB(RDOFunAlgorithmicDiapCalc, RDOFunAlgorithmicCalc)
{
DECLARE_FACTORY(RDOFunAlgorithmicDiapCalc)
private:
	RDOFunAlgorithmicDiapCalc(CREF(RDOValue) min_value, CREF(RDOValue) max_value);

	RDOValue m_min_value;
	RDOValue m_max_value;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOFunCalcGroup
 * @brief   Базовая калка для групповых калок
 *********************************************************************************/
CALC_SUB(RDOFunCalcGroup, RDOFunCalc)
{
protected:
	int        m_nResType;
	LPRDOCalc  m_pCondition;

	RDOFunCalcGroup(int nResType, CREF(LPRDOCalc) pCondition);
};

/******************************************************************************//**
 * @def     DEFINE_CALC_GROUP
 * @brief   Групповые калки
 *********************************************************************************/
#define DEFINE_CALC_GROUP(CalcName) \
CALC_SUB(RDOFunCalc##CalcName, RDOFunCalcGroup) \
{ \
DECLARE_FACTORY(RDOFunCalc##CalcName) \
private: \
	RDOFunCalc##CalcName(int nResType, CREF(LPRDOCalc) pCondition) \
		: RDOFunCalcGroup(nResType, pCondition) \
	{} \
 	virtual REF(RDOValue) doCalc(CREF(LPRDORuntime) pRuntime); \
};

/******************************************************************************//**
 * @class   RDOFunCalcExist
 * @brief   Логический предикат существования
 *********************************************************************************/
DEFINE_CALC_GROUP(Exist);

/******************************************************************************//**
 * @class   RDOFunCalcNotExist
 * @brief   Логический предикат отрицания существования
 *********************************************************************************/
DEFINE_CALC_GROUP(NotExist);

/******************************************************************************//**
 * @class   RDOFunCalcForAll
 * @brief   Логический предикат общности
 *********************************************************************************/
DEFINE_CALC_GROUP(ForAll);

/******************************************************************************//**
 * @class   RDOFunCalcNotForAll
 * @brief   Логический предикат отрицания общности
 *********************************************************************************/
DEFINE_CALC_GROUP(NotForAll);

class RDOResource;

/******************************************************************************//**
 * @class   RDOFunCalcSelect
 * @brief   Базовая групповая функция
 *********************************************************************************/
CALC_SUB(RDOFunCalcSelect, RDOFunCalcGroup)
{
DECLARE_FACTORY(RDOFunCalcSelect)
public:
	mutable std::list<LPRDOResource> res_list;
	void prepare(CREF(LPRDORuntime) pRuntime);

private:
	RDOFunCalcSelect(int nResType, CREF(LPRDOCalc) pCondition);

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOFunCalcSelectBase
 * @brief   Базовая калка для операторов Select
 *********************************************************************************/
CALC_SUB(RDOFunCalcSelectBase, RDOFunCalc)
{
protected:
	RDOFunCalcSelectBase(CREF(LPRDOFunCalcSelect) pSelect, CREF(LPRDOCalc) pCondition);

	LPRDOFunCalcSelect m_pSelect;
	LPRDOCalc          m_pCondition;
};

/******************************************************************************//**
 * @def     DEFINE_CALC_SELECT_GROUP
 * @brief   Select-группа
 *********************************************************************************/
#define DEFINE_CALC_SELECT_GROUP(CalcName) \
CALC_SUB(RDOFunCalcSelect##CalcName, RDOFunCalcSelectBase) \
{ \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName) \
private: \
	RDOFunCalcSelect##CalcName(CREF(LPRDOFunCalcSelect) pSelect, CREF(LPRDOCalc) pCondition) \
		: RDOFunCalcSelectBase(pSelect, pCondition) \
	{} \
	DECALRE_ICalc; \
};

/******************************************************************************//**
 * @def     DEFINE_CALC_SELECT_METHOD
 * @brief   Select-метод
 *********************************************************************************/
#define DEFINE_CALC_SELECT_METHOD(CalcName) \
CALC_SUB(RDOFunCalcSelect##CalcName, RDOFunCalcSelectBase) \
{ \
DECLARE_FACTORY(RDOFunCalcSelect##CalcName) \
private: \
	RDOFunCalcSelect##CalcName(CREF(LPRDOFunCalcSelect) pSelect) \
		: RDOFunCalcSelectBase(pSelect, NULL) \
	{} \
	DECALRE_ICalc; \
};

/******************************************************************************//**
 * @class   RDOFunCalcSelectExist
 * @brief   Квантор существования
 *********************************************************************************/
DEFINE_CALC_SELECT_GROUP(Exist);

/******************************************************************************//**
 * @class   RDOFunCalcSelectNotExist
 * @brief   Обратный квантор существования
 *********************************************************************************/
DEFINE_CALC_SELECT_GROUP (NotExist );

/******************************************************************************//**
 * @class   RDOFunCalcSelectForAll
 * @brief   Проверка условия для каждого элемента множества
 *********************************************************************************/
DEFINE_CALC_SELECT_GROUP (ForAll   );

/******************************************************************************//**
 * @class   RDOFunCalcSelectNotForAll
 * @brief   Обратная проверка условия для каждого элемента множества
 *********************************************************************************/
DEFINE_CALC_SELECT_GROUP (NotForAll);

/******************************************************************************//**
 * @class   RDOFunCalcSelectEmpty
 * @brief   Проверка множества на пустоту
 *********************************************************************************/
DEFINE_CALC_SELECT_METHOD(Empty    );

/******************************************************************************//**
 * @class   RDOFunCalcSelectSize
 * @brief   Размер множества
 *********************************************************************************/
DEFINE_CALC_SELECT_METHOD(Size     );

/******************************************************************************//**
 * @class   RDOCalcFuncParam
 * @brief   Параметр функции
 *********************************************************************************/
CALC(RDOCalcFuncParam)
{
DECLARE_FACTORY(RDOCalcFuncParam)
private:
	RDOCalcFuncParam(int param_number, CREF(RDOSrcInfo) _src_info);

	int m_param_number;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcGetConst
 * @brief   Получение контанты
 *********************************************************************************/
CALC(RDOCalcGetConst)
{
DECLARE_FACTORY(RDOCalcGetConst)
private:
	RDOCalcGetConst(int number);

	int m_number;
	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcSetConst
 * @brief   Установка константы
 *********************************************************************************/
CALC(RDOCalcSetConst)
{
DECLARE_FACTORY(RDOCalcSetConst)
private:
	RDOCalcSetConst(int number, CREF(LPRDOCalc) pCalc);

	int        m_number;
	LPRDOCalc  m_pCalc;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcFunctionCall
 * @brief   Вызов функции
 *********************************************************************************/
CALC(RDOCalcFunctionCall)
{
DECLARE_FACTORY(RDOCalcFunctionCall)
public:
	void addParameter(CREF(LPRDOCalc) pCalc);
	void setFunctionCalc(CREF(LPRDOFunCalc) pFunction);

private:
	RDOCalcFunctionCall(CREF(LPRDOFunCalc) pFunction);

	std::vector<LPRDOCalc>  m_parameters;
	LPRDOFunCalc            m_pFunction;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcCreateNumberedResource
 * @brief   Рудимент для rdo_converter
 *********************************************************************************/
CALC(RDOCalcCreateNumberedResource)
{
DECLARE_FACTORY(RDOCalcCreateNumberedResource)
public:
	ruint getNumber() const {NEVER_REACH_HERE;}
	virtual PTR(RDOResource) createResource(CREF(LPRDORuntime) pRuntime) const {NEVER_REACH_HERE; return 0;}

protected:
	RDOCalcCreateNumberedResource(int _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent);

	int                    m_pType;
	rbool                  traceFlag;
	std::vector<RDOValue>  paramsCalcs;
	ruint                  number;
	rbool                  isPermanent;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcCreateProcessResource
 * @brief   Рудимент для rdo_converter
 *********************************************************************************/
CALC(RDOCalcCreateProcessResource)
{
DECLARE_FACTORY(RDOCalcCreateProcessResource)
public:
	ruint getNumber() const {NEVER_REACH_HERE;}
	virtual PTR(RDOResource) createResource(CREF(LPRDORuntime) pRuntime) const {NEVER_REACH_HERE; return 0;}

protected:
	RDOCalcCreateProcessResource(rsint _type, rbool _traceFlag, CREF(std::vector<RDOValue>) _paramsCalcs, int _number, rbool _isPermanent);

	rsint                  m_pType;
	rbool                  traceFlag;
	std::vector<RDOValue>  paramsCalcs;
	ruint                  number;
	rbool                  isPermanent;

	DECALRE_ICalc;
};

/******************************************************************************//**
 * @class   RDOCalcCreateResource
 * @brief   Создание нового ресурса
 *********************************************************************************/
CALC(RDOCalcCreateResource)
{
DECLARE_FACTORY(RDOCalcCreateResource)
private:
	//! relResID == 0 для ресурсов, создаваемых при инициализации модели
	RDOCalcCreateResource(CREF(LPIResourceType) pType, CREF(std::vector<RDOValue>) rParamsCalcs, rbool traceFlag, rbool permanentFlag, ruint relResID = 0);

	LPIResourceType        m_pResType;
	std::vector<RDOValue>  m_paramsCalcs;
	rbool                  m_traceFlag;
	rbool                  m_permanentFlag;
	ruint                  m_relResID;

	DECALRE_ICalc;
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "rdo_lib/rdo_runtime/rdocalc.inl"

#endif // _LIB_RUNTIME_CALC_H_
