#ifndef _RDO_PLUGIN_GAME_5_TILE_ORDER_DIALOG_H_
#define _RDO_PLUGIN_GAME_5_TILE_ORDER_DIALOG_H_

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include "ui_plugin_game5_tiles_order_dialog.h"
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class TilesOrderDialog
	: public QDialog
	, public Ui_RAOgame5TilesOrderDialog
{
Q_OBJECT
public:
	TilesOrderDialog(QWidget* parent, const std::vector<unsigned int>& state);
	virtual ~TilesOrderDialog();

signals:
	void tilesOrderCommited(const std::vector<unsigned int>& state);

private:
	QString validatorRegExpPattern(int value) const;

private slots:
	void onOkClick();
};

#endif // _RDO_PLUGIN_GAME_5_TILE_ORDER_DIALOG_H_
