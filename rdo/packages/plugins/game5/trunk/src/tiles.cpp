/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/tiles.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

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
	setObjectName("button" + QString::number(number));
	setStyleSheet("\
	               background-color: red; \
	               border-style: outset; \
	               border-width: 2px; \
	               border-radius: 10px; \
	               border-color: black; \
	               font: bold 28px; \
	               padding: 6px;"
	);

	connect(this, SIGNAL(clicked()), this, SLOT(reemitClicked()));
}

void Tile::reemitClicked()
{
	emit tileClicked(tileNumber);
}

