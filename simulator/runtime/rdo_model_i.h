/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_model_i.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      28.07.2009
  \brief     Интерфейсы IModelStructure и IName
  \indent    4T
*/

#ifndef _LIB_RUNTIME_MODEL_I_H_
#define _LIB_RUNTIME_MODEL_I_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/interface/rdointerface.h"
// --------------------------------------------------------------------------------

/*!
  \interface IModelStructure
  \brief     Интерфейс структуры модели, используется для записи стукруры в файл трассировки
*/
class IModelStructure
{
public:
	/*!
	  \brief      Записать структу модели в поток
	  \param[out] stream выходной поток
	*/
	virtual void writeModelStructure(std::ostream& stream) const = 0;
};

#define DECLARE_IModelStructure \
	virtual void writeModelStructure(std::ostream& stream) const;

/*!
  \interface IName
  \brief     Интерфейс имени объекта
*/
class IName
{
public:
	/*!
	  \brief  Получить имя объекта
	  \return имя объекта
	*/
	virtual const std::string& name() const = 0;
};

#define DECLARE_IName \
	virtual const std::string& name() const;

#endif // _LIB_RUNTIME_MODEL_I_H_
