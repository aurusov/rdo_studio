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
#include "utils/src/common/warning_disable.h"
#include <boost/range/algorithm/find.hpp>
#include <ctime>
#include <cstdlib>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/board.h"
// --------------------------------------------------------------------------------

namespace
{
	const std::size_t holeIndex = 0;
} // anonymous namespace

Board::Board(QWidget* pParent)
	: QFrame        (pParent)
	, m_tileSize    (75)
	, m_tilesCountX (3)
	, m_tilesCountY (2)
	, m_spacer      (1)
	, m_boardSpacer (3)
	, m_boardSizeX  (sizeCalc(m_tilesCountX) - m_spacer + 2 * m_boardSpacer)
	, m_boardSizeY  (sizeCalc(m_tilesCountY) - m_spacer + 2 * m_boardSpacer)
	, m_topLeftX    (m_boardSpacer)
	, m_topLeftY    (m_boardSpacer)
{
	setFixedSize(m_boardSizeX,m_boardSizeY);

	setStyleSheet("\
		background-color: pink; \
		border-style: outset; \
		border-width: 2px; \
		border-radius: 13px; \
		border-color: black; \
		align: center; \
	");
}

Board::~Board()
{
}

void Board::init(bool disabledMode)
{
	tilesPosition.resize(m_tilesCountX * m_tilesCountY);
	tiles        .resize(m_tilesCountX * m_tilesCountY);

	for (int index = 1; index < m_tilesCountX * m_tilesCountY; index++)
	{
		tiles[index] = new Tile(index, this);
		tiles[index]->setFixedSize(m_tileSize,m_tileSize);
		tiles[index]->setDisabled(disabledMode);
		connect(tiles[index], &Tile::tileClicked, this, &Board::clickOnTile);
	}
	buildCorrectOrder();
}

void Board::clickOnTile(int number)
{
	if (freePlaceIsNearby(tilesPosition[number]))
	{
		moveTile(number, tilesPosition[holeIndex]);
	}
}

QPoint Board::tilePoint(int place) const
{
	const int column = place % m_tilesCountX;
	const int row    = place / m_tilesCountX;
	const int tilePlaceX = m_topLeftX + sizeCalc(column);
	const int tilePlaceY = m_topLeftY + sizeCalc(row   );
	return QPoint(tilePlaceX,tilePlaceY);
}

int Board::sizeCalc(int count) const
{
	return count * m_tileSize + count * m_spacer;
}

bool Board::freePlaceIsNearby(int place) const
{
	const int tileCol      = place % m_tilesCountX;
	const int tileRow      = place / m_tilesCountX;
	const int freePlaceCol = tilesPosition[holeIndex] % m_tilesCountX;
	const int freePlaceRow = tilesPosition[holeIndex] / m_tilesCountX;

	return ((tileCol == freePlaceCol + 1) && (tileRow == freePlaceRow)) ||
	       ((tileCol == freePlaceCol - 1) && (tileRow == freePlaceRow)) ||
	       ((tileRow == freePlaceRow + 1) && (tileCol == freePlaceCol)) ||
	       ((tileRow == freePlaceRow - 1) && (tileCol == freePlaceCol)) ;
}

void Board::buildCorrectOrder()
{
	for (int index = 1; index < m_tilesCountX * m_tilesCountY; index++)
	{
		std::size_t position = index - 1;
		tilesPosition[index] = position;
		tiles[index]->move(tilePoint(position));
	}
	tilesPosition[holeIndex] = m_tilesCountX * m_tilesCountY - 1;
}

void Board::buildRandomOrder(bool solvabilityCheck)
{
	std::srand(unsigned(std::time(0)));
	do
	{
		std::random_shuffle(tilesPosition.begin(), tilesPosition.end());
		for (int index = 1; index < m_tilesCountX * m_tilesCountY; index++)
		{
			tiles[index]->move(tilePoint(tilesPosition[index]));
		}
	} while (solvabilityCheck && !lineupIsSolvable());
}

bool Board::lineupIsSolvable() const
{
	int freePlaceRow = tilesPosition[holeIndex] / m_tilesCountX + 1;
	int sum = 0;
	for (int i = 1; i < m_tilesCountX * m_tilesCountY; i++)
	{
		if (tilesPosition[i] > 0)
		{
			for (int j = i + 1; j < m_tilesCountX * m_tilesCountY; j++)
			{
				if (tilesPosition[j] > 0 && tilesPosition[i] > tilesPosition[j])
					sum++;
			}
		}
	}
	return m_tilesCountX % 2 != 0
		? sum % 2 == 0
		: (m_tilesCountY % 2 != 0
				? (sum + freePlaceRow) % 2 != 0
				: (sum + freePlaceRow) % 2 == 0
		); // См. Перельман. Живая математика.
}

const int& Board::getTilePosition(const int& index) const
{
	return tilesPosition[index] + 1;
}

const int&Board::getQuantityOfTiles() const
{
	return m_tilesCountX * m_tilesCountY - 1;
}

QString Board::getBoardState() const
{
	std::vector<unsigned int> transpVector;
	transpVector.resize(tilesPosition.size());
	for (unsigned int i = 0; i < tilesPosition.size(); i++)
	{
		transpVector[tilesPosition[i]] = i;
	}

	QString boardStateStr;
	for (unsigned int i = 0; i < transpVector.size(); i++)
	{
		boardStateStr += QString::number(transpVector[i]) + " ";
	}

	return boardStateStr;
}

void Board::setTilesPositon(const QString& string)
{
	for (unsigned int positionIndex = 0; positionIndex < tilesPosition.size(); positionIndex++)
	{
		unsigned int tileIndex = string.section(' ', positionIndex, positionIndex, QString::SectionSkipEmpty).toInt();
		moveTile(tileIndex, positionIndex);
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
