#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/edit.h"
#include "app/rdo_studio/src/editor/model_edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {

class Parser: public Edit
{
public:
    Parser(QWidget* pParent);
    virtual ~Parser();

    void setEditorStyle(ParserStyle* pStyle);

    void replaceCurrent(const QString& str, int changePosValue = -1) const;

protected:
    std::string kw0;
    std::string kw1;
    std::string kw2;
    std::string kw3;
    std::string getAllKW() const;

    static std::string convertToLexer(const std::string& kw);

private:
    typedef  Edit  super;
};

}}} // namespace rdo::gui::editor
