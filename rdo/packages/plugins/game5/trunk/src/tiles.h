/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/tiles.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_TILES_H_
#define _RDO_PLUGIN_GAME_5_TILES_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QPushButton>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class Tile: public QPushButton
{
Q_OBJECT
public:
	Tile(int number, QWidget* pParent);
	
signals:
	void tileClicked(int number);

private slots:
	void reemitClicked();

private:
	int tileNumber;
};

#endif // _RDO_PLUGIN_GAME_5_TILES_H_