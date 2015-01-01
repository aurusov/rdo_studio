#pragma once

// ----------------------------------------------------------------------- INCLUDES
#include <QString>
#include <QFile>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/board.h"
// --------------------------------------------------------------------------------

class HeuristicChecker
{

public:
    HeuristicChecker();
    void compareSolutions();
    ~HeuristicChecker();

private:
    QString modelRSS(const quint32* set) const;
    QString getSolutionCost() const;
    void getNextRightSolution();
    bool openRightSolutionFile();

    QFile r_solution_file;
    quint32 bufSize;
    qint32 rightSolution;
};