/******************************************************************************//**
 * @copyright (c) RDO-Team, 2009
 * @file      rdo_model_i.h
 * @authors   Урусов Андрей
 * @date      28.07.2009
 * @brief     unknown
 * @indent    4T
 *********************************************************************************/

#ifndef _LIB_RUNTIME_MODEL_I_H_
#define _LIB_RUNTIME_MODEL_I_H_

// **************************************************************************** PCH
// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
#include "rdo_common/rdointerface.h"
// ********************************************************************************

/******************************************************************************//**
 * @interface IModelStructure
 * @brief     unknown
 *********************************************************************************/
class IModelStructure
{
public:
	/**
	 * @fn     virtual void writeModelStructure(REF(std::ostream) stream) const
	 * @brief  Запись структуры модели
	 * @param  stream выходной поток
	 */
	virtual void writeModelStructure(REF(std::ostream) stream) const = 0;
};

/******************************************************************************//**
 * @def     DECLARE_IModelStructure
 * @brief   Макрос для декларации метода структуры модели IModelStructure
 * @details Декларирует метод \a writeModelStructure()
 *********************************************************************************/
#define DECLARE_IModelStructure \
	virtual void writeModelStructure(REF(std::ostream) stream) const;

/******************************************************************************//**
 * @interface IName
 * @brief     unknown
 *********************************************************************************/
class IName
{
public:
	/**
	 * @fn     virtual CREF(tstring) name() const
	 * @brief  Запрос имени
	 * @return строка имени
	 */
	virtual CREF(tstring) name() const = 0;
};

/******************************************************************************//**
 * @def     DECLARE_IName
 * @brief   Макрос для декларации метода интерфейса IName
 * @details Декларирует метод \a name()
 *********************************************************************************/
#define DECLARE_IName \
	virtual CREF(tstring) name() const;

#endif // _LIB_RUNTIME_MODEL_I_H_
