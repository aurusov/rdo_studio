#pragma once

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
