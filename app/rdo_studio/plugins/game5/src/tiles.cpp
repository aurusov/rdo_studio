// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/tiles.h"
// --------------------------------------------------------------------------------

Tile::Tile(int number, QWidget* pParent)
    : QPushButton(pParent)
    , tileNumber (number)
{
    setText(QString::number(number));
    setStyleSheet("\
                   background-color: red; \
                   border-style: outset; \
                   border-width: 2px; \
                   border-radius: 10px; \
                   border-color: black; \
                   font: bold 28px; \
                   color: black; \
                   padding: 6px;");

    connect(this, &QPushButton::clicked, this, &Tile::reemitClicked);
}

Tile::~Tile()
{}

void Tile::reemitClicked()
{
    emit tileClicked(tileNumber);
}

