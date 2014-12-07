// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_node_info.h"
// --------------------------------------------------------------------------------

GraphNodeInfo::GraphNodeInfo(int nodeID, int parentNodeId, int pathCost, int restPathCost,
                             const QString& moveDirection, int moveCost, int relevantTile,
                             int graphLevel, int tileMoveFrom, int tileMoveTo,
                             const std::vector<unsigned int>& boardState)
    : m_nodeID(nodeID)
    , m_parentNodeId(parentNodeId)
    , m_pathCost(pathCost)
    , m_restPathCost(restPathCost)
    , m_moveDirection(moveDirection)
    , m_moveCost(moveCost)
    , m_relevantTile(relevantTile)
    , m_graphLevel(graphLevel)
    , m_tileMoveFrom(tileMoveFrom)
    , m_tileMoveTo(tileMoveTo)
    , m_relatedToSolutionState(false)
    , m_boardState(boardState)
{}

GraphNodeInfo::GraphNodeInfo(const GraphNodeInfo& other)
    : m_nodeID(other.m_nodeID)
    , m_parentNodeId(other.m_parentNodeId)
    , m_pathCost(other.m_pathCost)
    , m_restPathCost(other.m_restPathCost)
    , m_moveDirection(other.m_moveDirection)
    , m_moveCost(other.m_moveCost)
    , m_relevantTile(other.m_relevantTile)
    , m_graphLevel(other.m_graphLevel)
    , m_tileMoveFrom(other.m_tileMoveFrom)
    , m_tileMoveTo(other.m_tileMoveTo)
    , m_relatedToSolutionState(other.m_relatedToSolutionState)
    , m_boardState(other.m_boardState)
{}

GraphNodeInfo::GraphNodeInfo()
{}

GraphNodeInfo::~GraphNodeInfo()
{}
