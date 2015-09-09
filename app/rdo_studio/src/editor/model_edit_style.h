#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/parser_edit_style.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace editor {
// --------------------------------------------------------------------------------
// -------------------- ModelAutoComplete
// --------------------------------------------------------------------------------
class ModelAutoComplete
{
public:
    ModelAutoComplete();
    virtual ~ModelAutoComplete();

    ModelAutoComplete& operator =( const ModelAutoComplete& autoComplete );
    bool operator ==( const ModelAutoComplete& autoComplete ) const;
    bool operator !=( const ModelAutoComplete& autoComplete ) const;

    void load(QSettings& settings);
    void save(QSettings& settings) const;

    bool useAutoComplete;
    bool showFullList;
};

QSettings& operator<< (QSettings& settings, const ModelAutoComplete& auto_complete);
QSettings& operator>> (QSettings& settings,       ModelAutoComplete& auto_complete);

// --------------------------------------------------------------------------------
// -------------------- ModelMargin
// --------------------------------------------------------------------------------
class ModelMargin
{
public:
    ModelMargin();
    virtual ~ModelMargin();

    ModelMargin& operator =( const ModelMargin& margin );
    bool operator ==( const ModelMargin& margin ) const;
    bool operator !=( const ModelMargin& margin ) const;

    void load(QSettings& settings);
    void save(QSettings& settings) const;

    bool fold;
    bool bookmark;
    bool lineNumber;
};

QSettings& operator<< (QSettings& settings, const ModelMargin& margin);
QSettings& operator>> (QSettings& settings,       ModelMargin& margin);

// --------------------------------------------------------------------------------
// -------------------- ModelStyle
// --------------------------------------------------------------------------------
class ModelStyle: public ParserStyle
{
protected:

public:
    enum class Fold
    {
        NONE = 0,
        PLUS,
        PLUS_CONNECTED,
        ARROW,
        ARROW_CONNECTED,
        BOX_CONNECTED,
        CIRCLE_CONNECTED
    };

    ModelStyle();
    ~ModelStyle();

    ModelStyle& operator =( const ModelStyle& style );
    bool operator ==( const ModelStyle& style ) const;
    bool operator !=( const ModelStyle& style ) const;

    ModelAutoComplete autoComplete;
    ModelMargin       margin;

    QColor foldFgColor;
    QColor foldBgColor;
    QColor errorBgColor;
    Fold   foldStyle;
    bool   commentFold;

    static ModelStyle getDefaultStyle();
    static ModelStyle getCppStyle();
    static ModelStyle getPascalStyle();
    static ModelStyle getHtmlStyle();
    static ModelStyle getClassicStyle();
    static ModelStyle getTwilightStyle();
    static ModelStyle getOceanStyle();

};

DECLARE_POINTER(ModelStyle)

QSettings& operator<< (QSettings& settings, const ModelStyle& style);
QSettings& operator>> (QSettings& settings,       ModelStyle& style);

}}} // namespace rdo::gui::editor
