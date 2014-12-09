#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
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
    FileMessage(const std::string& text, rdo::model::FileType file, std::size_t line, std::size_t pos, Type type = MT_ERROR);

    std::string getText() const;
    rdo::model::FileType getFileType() const;
    std::size_t getLineNumber() const;
    std::size_t getPosInLine() const;
    Type getType () const;

    void setText(const std::string& text);

private:
    std::string m_text; // Текст сообщения
    rdo::model::FileType m_file; // Файл с ошибкой
    std::size_t m_line; // Номер строки с ошибкой
    std::size_t m_pos; // Позиция ошибки в строке
    Type m_type; //  Тип сообщения
};

}}} // namespace rdo::simulation::report
