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

enum class GraphItemType
{
    NODE = QGraphicsItem::UserType + 1,
    EDGE = QGraphicsItem::UserType + 2,
};

}}} // namespace rdo::plugin::game5
