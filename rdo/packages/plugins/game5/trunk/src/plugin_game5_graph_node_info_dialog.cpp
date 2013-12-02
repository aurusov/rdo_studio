/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/plugin_game5_graph_node_info_dialog.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/plugin_game5_graph_node_info_dialog.h"
// --------------------------------------------------------------------------------

PluginGame5GraphNodeInfoDialog::PluginGame5GraphNodeInfoDialog(QWidget* pParent)
	: QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
	, m_pNode(NULL)
{
	setupUi(this);
	gameBoard->init(true);

	buttonNext->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
	buttonPrev->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
	connect(buttonNext, &QPushButton::clicked, this, &PluginGame5GraphNodeInfoDialog::nextNode);
	connect(buttonPrev, &QPushButton::clicked, this, &PluginGame5GraphNodeInfoDialog::prevNode);

	if (pParent)
	{
		move(pParent->frameGeometry().center() - frameGeometry().center());
	}
}

PluginGame5GraphNodeInfoDialog::~PluginGame5GraphNodeInfoDialog()
{
}

void PluginGame5GraphNodeInfoDialog::updateDlg(GraphNode* node)
{
	m_pNode = node;

	labelPathCostOut    ->setText(QString::number(node->getPathCost()));
	labelRestPathCostOut->setText(QString::number(node->getRestPathCost()));
	labelRelevantTileOut->setText(QString::number(node->getRelevantTile()));
	QString moveText;
	if (node->getRelevantTile())
	{
		QString moveDirectionText;
		switch (node->getMoveDirection())
		{
			case 1: moveDirectionText = "Вправо"; break;
			case 2: moveDirectionText = "Влево" ; break;
			case 3: moveDirectionText = "Вверх" ; break;
			case 4: moveDirectionText = "Вниз"  ; break;
		}
		moveText = moveDirectionText + " (c " + QString::number(node->getTileMoveFrom())
		         + " на " + QString::number(node->getTileMoveTo()) + ")";
	}
	else
	{
		moveText = "Начало поиска";
	}
	labelSolutionOut->setText(node->isRelatedToSolution() ? "Да" : "Нет");
	labelMoveDirectionOut->setText(moveText);
	labelMoveCostOut     ->setText(QString::number(node->getMoveCost()));
	labelNodeNumOut      ->setText(QString::number(node->getGraphNode()));
	if (GraphNode* parentNode = node->getParentGraphNode())
	{
		labelParentNodeNumOut->setText(QString::number(parentNode->getGraphNode()));
	}
	else
	{
		labelParentNodeNumOut->setText(QString::number(0));
	}

	buttonNext->setEnabled(!node->getChildrenList().empty());
	buttonPrev->setEnabled(node->getParentGraphNode());

	gameBoard->setTilesPositon(node->getBoardState());
}

void PluginGame5GraphNodeInfoDialog::nextNode()
{
	BOOST_FOREACH(GraphNode* node, m_pNode->getChildrenList())
	{
		if (node->isRelatedToSolution())
		{
			emit updateCheckedNode(node);
			return;
		}
	}
	emit updateCheckedNode(*m_pNode->getChildrenList().begin());
}

void PluginGame5GraphNodeInfoDialog::prevNode()
{
	emit updateCheckedNode(m_pNode->getParentGraphNode());
}