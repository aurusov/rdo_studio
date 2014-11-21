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
	const std::size_t HOLE_NUMBER = 0;
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
	setFixedSize(m_boardSizeX, m_boardSizeY);

	setStyleSheet("\
		background-color: pink; \
		border-style: outset; \
		border-width: 2px; \
		border-radius: 13px; \
		border-color: black; \
		align: center; \
	");

	tiles.resize(m_tilesCountX * m_tilesCountY);

	for (int index = 0; index < m_tilesCountX * m_tilesCountY; index++)
	{
		tiles[index] = new PlacedTile(index, this);
		tiles[index]->setFixedSize(m_tileSize,m_tileSize);
		connect(tiles[index], &Tile::tileClicked, this, &Board::clickOnTile);
	}
	tiles[HOLE_NUMBER]->setVisible(false);
	buildCorrectOrder();
}

Board::~Board()
{}

void Board::setTilesDisabled(bool value)
{
	for (PlacedTile* tile: tiles)
	{
		tile->setDisabled(value);
	}
}

void Board::clickOnTile(int number)
{
	if (freePlaceIsNearby(tiles[number]->getPosition()))
	{
		swapTiles(number, HOLE_NUMBER);
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
	const int freePlaceCol = tiles[HOLE_NUMBER]->getPosition() % m_tilesCountX;
	const int freePlaceRow = tiles[HOLE_NUMBER]->getPosition() / m_tilesCountX;

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
		moveTile(tiles[index], position);
	}
	moveTile(tiles[HOLE_NUMBER], m_tilesCountX * m_tilesCountY - 1);
}

void Board::buildRandomOrder(bool solvabilityCheck)
{
	std::srand(unsigned(std::time(0)));
	std::vector<unsigned int> tilesPosition;
	for (const auto& tile: tiles)
	{
		tilesPosition.push_back(tile->getPosition());
	}
	do
	{
		std::random_shuffle(tilesPosition.begin(), tilesPosition.end());
	} while (solvabilityCheck && !orderIsSolvable(tilesPosition));
	for (int index = 0; index < m_tilesCountX * m_tilesCountY; index++)
	{
		moveTile(tiles[index], tilesPosition[index]);
	}
}

bool Board::orderIsSolvable(const std::vector<unsigned int>& tilesPosition) const
{
	int freePlaceRow = tilesPosition[HOLE_NUMBER] / m_tilesCountX + 1;
	int sum = 0;
	for (int i = 1; i < m_tilesCountX * m_tilesCountY; i++)
	{
		for (int j = i + 1; j < m_tilesCountX * m_tilesCountY; j++)
		{
			if (tilesPosition[i] > tilesPosition[j])
				sum++;
		}
	}
	return m_tilesCountX % 2 != 0
		? sum % 2 == 0
		: (m_tilesCountY % 2 != 0
				? (sum + freePlaceRow) % 2 != 0
				: (sum + freePlaceRow) % 2 == 0
		); // См. Перельман. Живая математика.
}

int Board::getTilePosition(int index) const
{
	return tiles[index]->getPosition() + 1;
}

int Board::getHolePosition() const
{
	return getTilePosition(HOLE_NUMBER);
}

int Board::getQuantityOfTiles() const
{
	return m_tilesCountX * m_tilesCountY - 1;
}

std::vector<unsigned int> Board::getBoardState() const
{
	std::vector<unsigned int> boardState;
	boardState.resize(tiles.size());
	for (unsigned int i = 0; i < tiles.size(); i++)
	{
		boardState[tiles[i]->getPosition()] = i;
	}
	return boardState;
}

void Board::setTilesPositon(const std::vector<unsigned int>& newState)
{
	for (unsigned int positionIndex = 0; positionIndex < newState.size() &&
			positionIndex < getBoardState().size(); positionIndex++)
	{
		unsigned int currentStateIndex = getBoardState()[positionIndex];
		swapTiles(newState[positionIndex], currentStateIndex);
	}
}

void Board::swapTiles(int first, int second)
{
	if (first != second)
	{
		const unsigned int firstPosition = tiles[first]->getPosition();
		moveTile(tiles[first], tiles[second]->getPosition());
		moveTile(tiles[second], firstPosition);
	}
}

void Board::moveTile(PlacedTile* tile, unsigned int place)
{
	tile->move(tilePoint(place));
	tile->setPosition(place);
}

Board::PlacedTile::PlacedTile(int number, QWidget* pParent)
    : Tile(number, pParent)
{}

Board::PlacedTile::~PlacedTile()
{}

unsigned int Board::PlacedTile::getPosition() const
{
	return position;
}

void Board::PlacedTile::setPosition(unsigned int value)
{
	position = value;
}
