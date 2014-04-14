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
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
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
	FileMessage(CREF(std::string) text, rdoModelObjects::RDOFileType file, std::size_t line, std::size_t pos, Type type = MT_ERROR);

	std::string getText() const;
	rdoModelObjects::RDOFileType getFileType() const;
	std::size_t getLineNumber() const;
	std::size_t getPosInLine() const;
	Type getType () const;

	void setText(CREF(std::string) text);

private:
	std::string m_text; //!< Текст сообщения
	rdoModelObjects::RDOFileType m_file; //!< Файл с ошибкой
	std::size_t m_line; //!< Номер строки с ошибкой
	std::size_t m_pos; //!< Позиция ошибки в строке
	Type m_type; //!<  Тип сообщения
};

}}} // namespace rdo::simulation::report

#endif // _SIMULATOR_REPORT_FILE_MESSAGE_H_
