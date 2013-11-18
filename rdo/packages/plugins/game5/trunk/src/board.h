/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/board.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_BOARD_H_
#define _RDO_PLUGIN_GAME_5_BOARD_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QFrame>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/tiles.h"
// --------------------------------------------------------------------------------

class Board: public QFrame
{
Q_OBJECT
public:
	Board(QWidget * pParent);
	~Board() {};

	const int m_tileSize;
	const int m_tilesCountX;
	const int m_tilesCountY;
	const int m_spacer;
	const int m_boardSpacer;
	const int m_boardSizeX;
	const int m_boardSizeY;
	const int m_topLeftX;
	const int m_topLeftY;
	bool      m_solvabilityCheck;
	const std::vector<unsigned int>& getTilesPos() const;

private:
	std::vector<unsigned int> tilesPosition;
	std::vector<Tile *> tiles;

	void   moveTile (int tileNumber, unsigned int position);
	QPoint tilePoint(int place);
	int    sizeCalc (int count);
	bool   freePlaceIsNearby(int place);
	bool   lineupIsSolvable();

public slots:
	void setTilesPositon(const QString& string);

private slots:
	void clickOnTile(int number);
	void buildRightLineup();
	void buildRandomLineup(bool solvabilityCheck);
};

#endif // _RDO_PLUGIN_GAME_5_BOARD_H_
