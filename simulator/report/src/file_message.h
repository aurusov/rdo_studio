/*!
  \copyright (c) RDO-Team, 2012
  \file      file_message.h
  \authors   Пройдаков Евгений (lord.tiran@gmail.com)
  \date      1.10.2012
  \brief     Файловое сообщение
  \indent    4T
*/

#ifndef _SIMULATOR_REPORT_FILE_MESSAGE_H_
#define _SIMULATOR_REPORT_FILE_MESSAGE_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdotypes.h"
#include "utils/src/common/rdomacros.h"
#include "utils/src/common/model_objects.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

//! Файловое сообщение
class FileMessage
{
public:
	enum Type
	{
		MT_ERROR = 0,
		MT_WARNING
	};

	//! Конструктор сообщения
	//! \param message - текст сообщение
	//! \param file    - файл, в котором найдена ошибка
	//! \param line    - номер строки с ошибкой
	//! \param pos     - позиция ошибки в строке
	//! \param type    - тип сообщения. Значение по умолчанию \b MT_ERROR
	FileMessage(CREF(tstring) text, rdoModelObjects::RDOFileType file, ruint line, ruint pos, Type type = MT_ERROR);

	tstring getText    () const;
	rdoModelObjects::RDOFileType getFileType() const;
	ruint getLineNumber() const;
	ruint getPosInLine () const;
	Type  getType      () const;

	void setText(CREF(tstring) text);

private:
	tstring                      m_text; //!< Текст сообщения
	rdoModelObjects::RDOFileType m_file; //!< Файл с ошибкой
	ruint                        m_line; //!< Номер строки с ошибкой
	ruint                        m_pos;  //!< Позиция ошибки в строке
	Type                         m_type; //!<  Тип сообщения
};

}}} // namespace rdo::simulation::report

#endif // _SIMULATOR_REPORT_FILE_MESSAGE_H_
