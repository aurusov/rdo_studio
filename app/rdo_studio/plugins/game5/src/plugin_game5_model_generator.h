#pragma once

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/board.h"
// --------------------------------------------------------------------------------

class PluginGame5ModelGenerator
{
public:
    static QString modelRTP(const Board& gameBoard);
    static QString modelRSS(const Board& gameBoard);
    static QString modelPAT();
    static QString modelDPT(const Board& gameBoard, const std::string& evaluateBy = "0", bool compareTops = true,
                            const std::string& activityValueLeft = "before 1", const std::string& activityValueRight = "before 1",
                            const std::string& activityValueDown = "before 1", const std::string& activityValueUp = "before 1");
    static QString modelFUN(const Board& gameBoard);
};
