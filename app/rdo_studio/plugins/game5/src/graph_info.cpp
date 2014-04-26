/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/graph_info.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QFormLayout>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/graph_info.h"
// --------------------------------------------------------------------------------

GraphInfo::GraphInfo(QWidget* parent)
    : QGroupBox("Информация о графе", parent)
{
	QFormLayout* graphInfoLayout = new QFormLayout(this);

	m_solutionCostLabel       = new QLabel("Стоимость решения:", this);
	m_solutionCostValue       = new QLabel(this);
	m_numberOfOpenNodesLabel  = new QLabel("Количество раскрытых вершин:", this);
	m_numberOfOpenNodesValue  = new QLabel(this);
	m_totalNumberOfNodesLabel = new QLabel("Количество вершин в графе:", this);
	m_totalNumberOfNodesValue = new QLabel(this);

	graphInfoLayout->setWidget(0, QFormLayout::LabelRole, m_solutionCostLabel);
	graphInfoLayout->setWidget(0, QFormLayout::FieldRole, m_solutionCostValue);
	graphInfoLayout->setWidget(1, QFormLayout::LabelRole, m_numberOfOpenNodesLabel);
	graphInfoLayout->setWidget(1, QFormLayout::FieldRole, m_numberOfOpenNodesValue);
	graphInfoLayout->setWidget(2, QFormLayout::LabelRole, m_totalNumberOfNodesLabel);
	graphInfoLayout->setWidget(2, QFormLayout::FieldRole, m_totalNumberOfNodesValue);

	setStyleSheet("background-color: rgba(255, 255, 255, 40);");
}

GraphInfo::~GraphInfo()
{}

void GraphInfo::update(int solutionCost, int numOfOpenNodes, int totalNumOfNodes)
{
	m_solutionCostValue->setText(QString::number(solutionCost));
	m_numberOfOpenNodesValue->setText(QString::number(numOfOpenNodes));
	m_totalNumberOfNodesValue->setText(QString::number(totalNumOfNodes));
}
