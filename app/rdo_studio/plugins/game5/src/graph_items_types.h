/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/graph_edge.h
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief
  \indent    4T
*/

#ifndef _RDO_PLUGIN_GAME_5_GRAPH_ITEMS_TYPES_H
#define _RDO_PLUGIN_GAME_5_GRAPH_ITEMS_TYPES_H

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QGraphicsItem>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

namespace rdo {
namespace plugin {
namespace game5 {

enum TypeID
{
	GRAPH_NODE = QGraphicsItem::UserType + 1,
	GRAPH_EDGE = QGraphicsItem::UserType + 2,
};

}}} // namespace rdo::plugin::game5

#endif // _RDO_PLUGIN_GAME_5_GRAPH_ITEMS_TYPES_H
