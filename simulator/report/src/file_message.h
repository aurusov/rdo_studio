#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <string>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/model_objects.h"
// --------------------------------------------------------------------------------

namespace rdo {
namespace simulation {
namespace report {

class FileMessage
{
public:
    enum Type
    {
        MT_ERROR = 0,
        MT_WARNING
    };

    FileMessage(const std::string& text, rdo::FileType file, std::size_t line, std::size_t pos, Type type = MT_ERROR);

    std::string getText() const;
    rdo::FileType getFileType() const;
    std::size_t getLineNumber() const;
    std::size_t getPosInLine() const;
    Type getType () const;

    void setText(const std::string& text);

private:
    std::string m_text;
    rdo::FileType m_file;
    std::size_t m_line;
    std::size_t m_pos;
    Type m_type;
};

}}} // namespace rdo::simulation::report
