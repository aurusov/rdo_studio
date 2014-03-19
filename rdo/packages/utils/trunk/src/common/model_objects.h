/*!
  \copyright (c) RDO-Team, 2011
  \file      model_objects.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.06.2009
  \brief     Типы объектов модели
  \indent    4T
*/

#ifndef _UTILS_MODEL_OBJECTS_H_
#define _UTILS_MODEL_OBJECTS_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdotypes.h"
// --------------------------------------------------------------------------------

namespace rdoModelObjects {

//! \details Типы файлов
enum RDOFileType
{
	RTP = 0, //!< Файл типов ресурсов
	RSS,     //!< Файл ресурсов
	EVN,     //!< Файл событий
	PAT,     //!< Файл паттернов
	DPT,     //!< Файл точек принятия решений
	PRC,     //!< Файл процессов
	FRM,     //!< Файл анимации
	FUN,     //!< Файл функций
	SMR,     //!< Файл прогона
	PMD,     //!< Файл описания показателей
	PMV,     //!< Файл со значениями показателей
	TRC,     //!< Файл трассировки
	RDOX,    //!< Файл проекта
	PRCX     //!< Файл GUI-процессов, XML
};

tstring getFileTypeString(RDOFileType type);

} // namespace rdoModelObjects

#endif // _UTILS_MODEL_OBJECTS_H_
