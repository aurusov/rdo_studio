/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/board.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <QMessageBox>
#include <boost/range/algorithm/find.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/board.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_dialog.h"
// --------------------------------------------------------------------------------

Board::Board(QWidget * pParent)
	: m_tileSize    (75)
	, m_tilesCountX (3)
	, m_tilesCountY (2)
	, m_spacer      (1)
	, m_boardSpacer (3)
	, m_boardSizeX  (sizeCalc(m_tilesCountX) - m_spacer + 2 * m_boardSpacer)
	, m_boardSizeY  (sizeCalc(m_tilesCountY) - m_spacer + 2 * m_boardSpacer)
	, m_topLeftX    (m_boardSpacer)
	, m_topLeftY    (m_boardSpacer)
	, QFrame        (pParent)
{
	setFixedSize(m_boardSizeX,m_boardSizeY);
	tilesPosition.resize(m_tilesCountX * m_tilesCountY);
	tiles        .resize(m_tilesCountX * m_tilesCountY);
	tilesPosition[0] = m_tilesCountX * m_tilesCountY;

	for (int place = 1; place < m_tilesCountX * m_tilesCountY; place++)
	{
		tilesPosition[place] = place;
		tiles[place] = new Tile(place, this);
		tiles[place]->setFixedSize(m_tileSize,m_tileSize);
		tiles[place]->setGeometry(tilePoint(place).x(),tilePoint(place).y(),m_tileSize,m_tileSize);
		connect(tiles[place],SIGNAL(tileClicked(int)),this,SLOT(clickOnTile(int)));
	}
	setStyleSheet("\
		background-color: pink; \
		border-style: outset; \
		border-width: 2px; \
		border-radius: 13px; \
		border-color: black; \
		align: center; \
	");
}

void Board::clickOnTile(int number)
{
	Tile* tile = qobject_cast<Tile*>(sender());
	if (freePlaceIsNearby(tilesPosition[number]))
	{
		tile->move(tilePoint(tilesPosition[0]));
		int currentPlace      = tilesPosition[number];
		tilesPosition[number] = tilesPosition[0];
		tilesPosition[0]      = currentPlace;
	}
}

QPoint Board::tilePoint(int place)
{
	int column = (place -1) % m_tilesCountX;
	int row    = (place -1) / m_tilesCountX;
	int tilePlaceX = m_topLeftX + sizeCalc(column);
	int tilePlaceY = m_topLeftY + sizeCalc(row   );
	return QPoint(tilePlaceX,tilePlaceY);
}

int Board::sizeCalc(int count)
{
	return count * m_tileSize + count * m_spacer;
}

bool Board::freePlaceIsNearby(int place)
{
	int tileCol      = (place -1) % m_tilesCountX;
	int tileRow      = (place -1) / m_tilesCountX;
	int freePlaceCol = (tilesPosition[0] -1) % m_tilesCountX;
	int freePlaceRow = (tilesPosition[0] -1) / m_tilesCountX;

	return tileCol == freePlaceCol + 1 && tileRow == freePlaceRow ||
	       tileCol == freePlaceCol - 1 && tileRow == freePlaceRow ||
	       tileRow == freePlaceRow + 1 && tileCol == freePlaceCol ||
	       tileRow == freePlaceRow - 1 && tileCol == freePlaceCol ;
}

void Board::buildRightLineup()
{
	for (int place = 1; place < m_tilesCountX * m_tilesCountY; place++)
	{
		tilesPosition[place] = place;
		tiles[place]->move(tilePoint(place));
	}
	tilesPosition[0] = m_tilesCountX * m_tilesCountY;
}

void Board::buildRandomLineup(bool solvabilityCheck)
{
	bool needRandomize = true;
	while (needRandomize)
	{
		std::random_shuffle(tilesPosition.begin(),tilesPosition.end());
		int freePlaceRow = (tilesPosition[0] -1) / m_tilesCountX;
		for (int tile = 1; tile < m_tilesCountX * m_tilesCountY; tile++)
		{
			tiles[tile]->move(tilePoint(tilesPosition[tile]));
		}
		if (lineupIsSolvable() || !solvabilityCheck)
		{
			needRandomize = false;
		}
	}
}

bool Board::lineupIsSolvable()
{
	int freePlaceRow = (tilesPosition[0] -1) / m_tilesCountX + 1;
	int sum=0;
	for (int i=1; i < m_tilesCountX * m_tilesCountY ;i++)
	{
		if (tilesPosition[i] > 0)
		{
			for (int j=i+1; j < m_tilesCountX * m_tilesCountY ;j++)
			{
				if (tilesPosition[j] > 0 && tilesPosition[i] > tilesPosition[j])
					sum++;
			}
		}
	}
	return m_tilesCountX % 2
		? !(sum % 2)
		: (m_tilesCountY % 2
				? (sum + freePlaceRow) % 2
				: (sum + freePlaceRow) % 2 == 0
		); // См. Перельман. Живая математика.
}

const std::vector<unsigned int>& Board::getTilesPos() const
{
	return tilesPosition;
}

void Board::setTilesPositon(QString string)
{
	QString::SectionFlag flag = QString::SectionSkipEmpty;
	for (unsigned int i = 0; i < tilesPosition.size(); i++)
	{
		unsigned int tile = string.section(' ', i, i).toInt();
		if (tile < tilesPosition.size())
		{
			moveTile(tile, i + 1);
		}
		else
		{
			QMessageBox::warning(qobject_cast<QWidget *>(sender()), "Игра 5",
			                     "Неверное значение: " + QString::number(tile));
		}
	}
}

void Board::moveTile(int tileNumber, unsigned int position)
{
	if (tileNumber)
	{
		tiles[tileNumber]->move(tilePoint(position));
	}
	unsigned int swapedTilePos = std::distance(
		tilesPosition.begin(),
		boost::range::find(tilesPosition, position)
	);

	if (swapedTilePos)
	{
		tiles[swapedTilePos]->move(tilePoint(tilesPosition[tileNumber]));
	}
	tilesPosition[swapedTilePos] = tilesPosition[tileNumber];
	tilesPosition[tileNumber]    = position;
}
