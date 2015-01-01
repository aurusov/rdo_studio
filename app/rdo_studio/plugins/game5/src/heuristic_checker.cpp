// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <boost/filesystem/path.hpp>
#include <QMessageBox>
#include <memory>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/heuristic_checker.h"
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "app/rdo_studio/src/model/model.h"
#include "app/rdo_studio/src/application.h"
// --------------------------------------------------------------------------------

namespace
{
    const int DPS_CO_INDEX = 4;
}

HeuristicChecker::HeuristicChecker(): r_solution_file("RightSolutions.rs")
{ }

bool HeuristicChecker::openRightSolutionFile()
{
    if (!r_solution_file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(g_pApp->getMainWnd(),
                                        "RAO-Studio",
                                        "Невозможно открыть файл эталонных решений",
                                        QMessageBox::Ok,
                                        QMessageBox::Ok);
        return EXIT_FAILURE;
    }
    // Get the header
    std::unique_ptr<char[]> raw_data(new char[sizeof(quint32)]);
    r_solution_file.read(raw_data.get(), sizeof(quint32));
    bufSize = *reinterpret_cast<quint32*>(raw_data.get());
    rightSolution = 0;
    return EXIT_SUCCESS;
}

HeuristicChecker::~HeuristicChecker()
{
    if (r_solution_file.isOpen())
    {
	    r_solution_file.close();
    }
}

QString HeuristicChecker::getSolutionCost() const
{
    rdo::gui::model::Model* pModel = g_pApp->getMainWndUI()->getModel();
    boost::filesystem::path traceFilePath(pModel->getFullName().toStdString());
    traceFilePath.replace_extension(".trc");
    QFile trcFile(QString(traceFilePath.string().c_str()));
    QString trcString;
    if (trcFile.open(QIODevice::ReadOnly))
    {
        trcString = QString(trcFile.readAll());
    }
    trcFile.close();
    const int begin = trcString.indexOf(QRegExp("SES|SEN"));
    return trcString.mid(begin).section(' ', DPS_CO_INDEX, DPS_CO_INDEX, QString::SectionSkipEmpty);
}

QString HeuristicChecker::modelRSS(const quint32* set) const
{
    std::stringstream RSStabTextStream;
    RSStabTextStream << "$Resources" << std::endl;
    for (quint32 i = 1; i < bufSize + 1; i++)
    {
        if (set[i - 1] != 0)
        {
            RSStabTextStream << "\tФишка" << set[i - 1]  <<" = Фишка(" << set[i - 1] << ", " << i  << ");" << std::endl;
        }
	else
        {
	    RSStabTextStream << "\tДырка = Дырка_t(" << i << ");" << std::endl;
        }
    }
    RSStabTextStream << "$End" << std::endl;
    return QString::fromStdString(RSStabTextStream.str());
}

void HeuristicChecker::getNextRightSolution()
{
    rdo::gui::model::Model* pModel = g_pApp->getMainWndUI()->getModel();
    // Set that we don't want to get a graph
    if (!r_solution_file.atEnd())
    {
        // Get case and set it
        std::unique_ptr<char[]> raw_data(new char[bufSize*sizeof(quint32)]);
        r_solution_file.read(raw_data.get(), bufSize*sizeof(quint32));
        quint32* set_data = reinterpret_cast<quint32*>(raw_data.get());
        for (int i = 0; i < pModel->getTab()->tabBar()->count(); i++)
        {
	    if (i == (int)rdo::FileType::RSS)
	    {
	        pModel->getTab()->getItemEdit(i)->clearAll();
	    }
        }
        pModel->getTab()->getItemEdit(rdo::FileType::RSS)->appendText(modelRSS(set_data));
        // Get the right solution
        raw_data.reset(new char[sizeof(quint32)]);
        r_solution_file.read(raw_data.get(), sizeof(quint32));
        rightSolution = *reinterpret_cast<quint32*>(raw_data.get());
        pModel->runModel();
    }
    else
    {
        QMessageBox::warning(g_pApp->getMainWnd(),
                                        "RAO-Studio",
                                        "Эвристика допустима!",
                                        QMessageBox::Ok,
                                        QMessageBox::Ok);
        r_solution_file.seek(sizeof(quint32));
        rightSolution = 0;
    }
}

void HeuristicChecker::compareSolutions()
{
    if (!r_solution_file.isOpen())
    {
	if (openRightSolutionFile() == EXIT_FAILURE)
	{
	    return ;
	}
    }
    if(getSolutionCost().toInt() != rightSolution)
    {
        QMessageBox::warning(g_pApp->getMainWnd(),
                                        "RAO-Studio",
                                        "Эвристика не допустима!",
                                        QMessageBox::Ok,
                                        QMessageBox::Ok);
        r_solution_file.seek(sizeof(quint32));
        rightSolution = 0;
    }
    else
    {
        getNextRightSolution();
    }
}