#ifndef _RDO_PLUGIN_GAME_5_TILES_H_
#define _RDO_PLUGIN_GAME_5_TILES_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QPushButton>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class Tile: public QPushButton
{Q_OBJECT

public:
	Tile(int number, QWidget* pParent);
	virtual ~Tile();

signals:
	void tileClicked(int number);

private:
	int tileNumber;

private slots:
	void reemitClicked();
};

#endif // _RDO_PLUGIN_GAME_5_TILES_H_
