#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QFrame>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/tiles.h"
// --------------------------------------------------------------------------------

class Board: public QFrame
{
Q_OBJECT
public:
    Board(QWidget* pParent = NULL);
    ~Board();

    void setTilesDisabled(bool value);

    const int m_tileSize;
    const int m_tilesCountX;
    const int m_tilesCountY;
    const int m_spacer;
    const int m_boardSpacer;
    const int m_boardSizeX;
    const int m_boardSizeY;
    const int m_topLeftX;
    const int m_topLeftY;

    int getTilePosition(int index) const;
    int getHolePosition() const;
    int getQuantityOfTiles() const;
    std::vector<unsigned int> getBoardState() const;

    class PlacedTile: public Tile
    {
    public:
        PlacedTile(int number, QWidget* pParent);
        ~PlacedTile();

        unsigned int getPosition() const;
        void setPosition(unsigned int value);

    private:
        unsigned int position;
    };

public slots:
    void setTilesPositon(const std::vector<unsigned int>& newState);
    void buildCorrectOrder();
    void buildRandomOrder(bool solvabilityCheck);

private:
    std::vector<PlacedTile*> tiles;

    void   swapTiles (int first, int second);
    QPoint tilePoint(int place) const;
    int    sizeCalc (int count) const;
    bool   freePlaceIsNearby(int place) const;
    bool   orderIsSolvable(const std::vector<unsigned int>& tilesPosition) const;
    void   moveTile(PlacedTile* tile, unsigned int place);

private slots:
    void clickOnTile(int number);
};
