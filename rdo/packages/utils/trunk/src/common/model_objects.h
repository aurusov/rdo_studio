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

//! \namespace rdoModelObjects
//! \brief     Пространство имён объектов модели
#define OPEN_RDO_MODEL_OBJECTS_NAMESPACE	namespace rdoModelObjects {
#define CLOSE_RDO_MODEL_OBJECTS_NAMESPACE	};

OPEN_RDO_MODEL_OBJECTS_NAMESPACE

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

	//! \details Типы объектов парсера, представляет собой битовую маску
	enum RDOParseType
	{
		obNONE = 0x0000, //!< Тип отсутствует (маски нет)
		obPRE  = 0x0001, //!< Предварительная компиляция
		obRTP  = 0x0002, //!< Компиляция типов ресурсов
		obRSS  = 0x0004, //!< Компиляция ресурсов
		obFUN  = 0x0008, //!< Компиляция функций
		obEVN  = 0x0010, //!< Компиляция событий
		obPAT  = 0x0020, //!< Компиляция паттернов
		obDPT  = 0x0040, //!< Компиляция точек принятия решений
		obPRC  = 0x0080, //!< Компиляция процессов
		obPMD  = 0x0100, //!< Компиляция показателей
		obFRM  = 0x0200, //!< Компиляция анимации
		obSMR  = 0x0400, //!< Компиляция прогона
		obPOST = 0x0800, //!< Пост-компиляция
		obALL  = 0x0FFF  //!< Все типы (маска полностью выставлена)
	};

	tstring getFileTypeString(RDOFileType type);

CLOSE_RDO_MODEL_OBJECTS_NAMESPACE

#endif // _UTILS_MODEL_OBJECTS_H_
