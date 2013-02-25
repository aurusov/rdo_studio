/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofile.h
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      10.05.2009
  \brief     Работа с файлами
  \indent    4T
*/

#ifndef _UTILS_RDOFILE_H_
#define _UTILS_RDOFILE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
// --------------------------------------------------------------------------------

OPEN_RDO_NAMESPACE

//! Скорее пространство имён для статических методов, чем класс
class File
{
public:
	static rbool create(CREF(tstring) name);
	static rbool create(CREF(tstring) name, CREF(tstring) content);

	//! Проверяет существование файла
	//! \param name - имя файла
	//! \result \b true, если существует
	static rbool   exist          (CREF(tstring) name);

	//! Проверяет признак только чтение
	//! \param name - имя файла
	//! \result \b true, если у файла установлен атрибут только чтение
	static rbool   read_only      (CREF(tstring) name);

	//! Удаляет файл
	//! \param name - имя файла
	//! \result \b true, если файл удалён удачно
	static rbool   unlink         (CREF(tstring) name);

	//! Разбивает полное имя файла на его составляющие
	//! \param[in]  name     - полное имя файла
	//! \param[out] fileDir  - путь
	//! \param[out] fileName - имя
	//! \param[out] fileExt  - расширение
	//! \result \b true, если получилось извлечь данные
	static rbool   splitpath      (CREF(tstring) name, REF(tstring) fileDir, REF(tstring) fileName, REF(tstring) fileExt);

	//! Возвращает полное имя временного файла
	//! \result полное имя временного файла
	static tstring getTempFileName();

	//! Получение пути файла по его имени
	//! \param fileName - полное имя файла
	//! \result Путь к файлу, заканчивающийся '/' или '\' в зависимости от платформы
	static tstring extractFilePath(CREF(tstring) fileName);

	//! Удаляет пустоты (пробелы, табуляцию, переводы строк) в конце файла
	//! \param name - имя файла
	//! \result \b true, если всё прошло успешно
	static rbool   trimLeft       (CREF(tstring) name);
};

CLOSE_RDO_NAMESPACE

#include "utils/rdofile.inl"

#endif // _UTILS_RDOFILE_H_
