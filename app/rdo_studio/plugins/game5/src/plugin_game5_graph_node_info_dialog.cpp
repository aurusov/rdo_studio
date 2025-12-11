// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include "utils/src/common/warning_enable.h"
#include <QStyle>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/plugin_game5_graph_node_info_dialog.h"
// --------------------------------------------------------------------------------

PluginGame5GraphNodeInfoDialog::PluginGame5GraphNodeInfoDialog(QWidget* pParent)
    : QDialog(pParent, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint)
    , m_pNode(NULL)
{
    setupUi(this);
    gameBoard->setTilesDisabled(true);

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
{}

void PluginGame5GraphNodeInfoDialog::updateDlg(GraphNode* node)
{
    m_pNode = node;

    labelPathCostOut    ->setText(QString::number(node->getPathCost()));
    labelRestPathCostOut->setText(QString::number(node->getRestPathCost()));
    labelRelevantTileOut->setText(QString::number(node->getRelevantTile()));
    QString moveText = node->getMoveDirection();
    if (node->getRelevantTile())
    {
        moveText += " (c " + QString::number(node->getTileMoveFrom())
                 +  " на " + QString::number(node->getTileMoveTo()) + ")";
    }
    labelSolutionOut     ->setText(node->isRelatedToSolution() ? "Да" : "Нет");
    labelMoveDirectionOut->setText(moveText);
    labelMoveCostOut     ->setText(QString::number(node->getMoveCost()));
    labelNodeNumOut      ->setText(QString::number(node->getNodeID()));
    if (GraphNode* parentNode = node->getParentGraphNode())
    {
        labelParentNodeNumOut->setText(QString::number(parentNode->getNodeID()));
    }
    else
    {
        labelParentNodeNumOut->setText(QString::number(0));
    }

    buttonNext->setEnabled(!node->getChildrenList().empty());
    buttonPrev->setEnabled(node->getParentGraphNode() != NULL);

    gameBoard->setTilesPositon(node->getBoardState());

    if (size().width() < sizeHint().width())
    {
        setFixedSize(sizeHint());
    }
}

void PluginGame5GraphNodeInfoDialog::nextNode()
{
    for (GraphNode* node: m_pNode->getChildrenList())
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
