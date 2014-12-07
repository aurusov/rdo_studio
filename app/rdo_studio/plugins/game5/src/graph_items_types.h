#pragma once

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
