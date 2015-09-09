#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QString>
#include <vector>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class GraphNodeInfo
{
public:
    GraphNodeInfo(int nodeID, int parentNodeId, int pathCost, int restPathCost,
                  const QString& moveDirection, int moveCost, int relevantTile,
                  int graphLevel, int tileMoveFrom, int tileMoveTo,
                  const std::vector<unsigned int>& boardState);
    GraphNodeInfo(const GraphNodeInfo& other);
    GraphNodeInfo();
    virtual ~GraphNodeInfo();

    int     m_nodeID;
    int     m_parentNodeId;
    int     m_pathCost;
    int     m_restPathCost;
    QString m_moveDirection;
    int     m_moveCost;
    int     m_relevantTile;
    int     m_graphLevel;
    int     m_tileMoveFrom;
    int     m_tileMoveTo;
    bool    m_relatedToSolutionState;
    std::vector<unsigned int> m_boardState;
};
