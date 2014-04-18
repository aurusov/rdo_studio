/*!
  \copyright (c) RDO-Team, 2013
  \file      app/rdo_studio/plugins/game5/src/plugin_game5.cpp
  \author    Чернов Алексей (ChernovAlexeyOlegovich@gmail.com)
  \date      22.09.2013
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <QMessageBox>
#include <QBitmap>
#include <QDir>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/plugin_game5.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/tracer/tracer.h"
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "utils/src/common/model_objects.h"
// --------------------------------------------------------------------------------

namespace
{
	const QString PLUGIN_GUID           = "{5315750C-964B-4ed2-96FE-21FC6226942B}";
	const QString PLUGIN_ACTION_NAME    = "action" + PLUGIN_GUID;
	const QString PLUGIN_MENU_NAME      = "createdMenu";
	const QString PLUGIN_MENU_TEXT      = "Плагины";
	const QString PLUGIN_TOOLBAR_NAME   = "toolbar" + PLUGIN_GUID;
	const QString RDO_MENUBAR_NAME      = "menubar";
	const size_t  RDOFileType_ENUM_SIZE = 13;
} // end anonymous namespace

QString PluginGame5::getPluginName() const
{
	return "game5_debug";
}

QString PluginGame5::getAuthor() const
{
	return "Chernov";
}

QString PluginGame5::getVersion() const
{
	return "0.2.1";
}

QUuid PluginGame5::getGUID()  const
{
	QUuid pluginGUID(PLUGIN_GUID);
	return pluginGUID;
}

void PluginGame5::pluginStartAction(QWidget* pParent)
{
	if (!g_pApp)
	{
		g_pApp = static_cast<Application*>(qApp);
		g_pModel  = g_pApp->getMainWndUI()->getModel();
		g_pTracer = g_pApp->getTracer();
		kernel    = g_pApp->getKernel();
	}

	QMenu* pluginMenu = findPluginMenu(pParent);
	if (!pluginMenu)
	{
		pluginMenu = new QMenu(PLUGIN_MENU_TEXT, pParent);
		pluginMenu->setObjectName(PLUGIN_MENU_NAME);
	}
	QMenuBar* menuBar = pParent->findChild<QMenuBar*>(RDO_MENUBAR_NAME);
	ASSERT(menuBar);
	menuBar->addMenu(pluginMenu);

	QAction* action = new QAction(getPluginName() + " ver " + getVersion(), pluginMenu);
	action->setObjectName(PLUGIN_ACTION_NAME);
	connect(action, &QAction::triggered, this, &PluginGame5::pluginSlot);
	pluginMenu->addAction(action);

	m_generateSituationDlg = NULL;
	m_graphDlg = NULL;
}

void PluginGame5::pluginStopAction(QWidget* pParent)
{
	QMenu* pluginMenu = findPluginMenu(pParent);
	ASSERT(pluginMenu);

	QAction* pluignAction = pluginMenu->findChild<QAction*>(PLUGIN_ACTION_NAME);
	ASSERT(pluignAction);
	delete pluignAction;

	if (pluginMenu->isEmpty())
	{
		delete pluginMenu;
	}

	ASSERT(m_generateSituationDlg);
	delete m_generateSituationDlg;
	ASSERT(m_graphDlg);
	delete m_graphDlg;
	QToolBar* pluginGame5ToolBar = pParent->findChild<QToolBar*>(PLUGIN_TOOLBAR_NAME);
	ASSERT(pluginGame5ToolBar);
	delete pluginGame5ToolBar;
}

void PluginGame5::pluginSlot()
{
	QWidget* pParent = qobject_cast<QWidget*>(sender()-> //action
	                                          parent()-> //QMenu
	                                          parent()); //QMainWindow
	MainWindow* pMainWindow = (MainWindow*)pParent;
	if (pMainWindow->getModel()->getTab())
	{
		bool canStart = true;
		if (pMainWindow->getModel()->isModify())
		{
			const int ret = QMessageBox::question(g_pApp->getMainWnd(),
			                                      "RAO-Studio",
			                                      "Модель будет перезаписана!\nНесохраненные изменения будут потеряны\nХотите сохранить изменения?",
			                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
			                                      QMessageBox::Save
			);
			switch (ret)
			{
				case QMessageBox::Save    : pMainWindow->actFileSaveAll->trigger(); break;
				case QMessageBox::Discard : break;
				default: canStart = false; break;
			}
		}
		else
		{
			if (!pMainWindow->getModel()->isEmpty())
			{
				const int ret = QMessageBox::warning(g_pApp->getMainWnd(),
				                                     "RAO-Studio",
				                                     "Модель будет перезаписана!",
				                                     QMessageBox::Ok | QMessageBox::Cancel,
				                                     QMessageBox::Ok);
				if (ret == QMessageBox::Cancel)
				{
					canStart = false;
				}
			}
		}
		if (canStart)
		{
			backUpModel(pMainWindow->getModel());
			initDialogs(pParent);
			initToolBar(pMainWindow);
			QAction* senderAction = qobject_cast<QAction*>(sender());
			senderAction->setEnabled(false);
		}
	}
	else
	{
		pMainWindow->actFileNew->trigger();
	}
}

void PluginGame5::backUpModel(rdo::gui::model::Model* pModel) const
{
	const QString modelFullName = pModel->getFullName();
	const QString modelFolder   = modelFullName.section(QRegExp("\\\\|/"), 0, -2) + "/";
	const QString backupFolder  = modelFolder + "backup" + QDateTime::currentDateTime().toString("_yyyy-MM-dd_HH.mm.ss") + "/";
	QDir makeDir;
	if (makeDir.mkpath(backupFolder))
	{
		for (size_t i = 0; i < RDOFileType_ENUM_SIZE; i++)
		{
			const QString fileFormat  = QString::fromStdString(
					rdoModelObjects::getFileTypeString(rdoModelObjects::RDOFileType(i)));
			const QString fileName    = modelFolder  + pModel->getName() + "." + fileFormat.toLower();
			const QString newFileName = backupFolder + pModel->getName() + "." + fileFormat.toLower();
			if (QFile::exists(fileName))
			{
				QFile::copy(fileName, newFileName);
			}
		}
	}
}

QMenu* PluginGame5::findPluginMenu(QWidget* pParent) const
{
	QMenu* pluginMenu = pParent->findChild<QMenu*>(PLUGIN_MENU_NAME);
	if (!pluginMenu)
	{
		QMenuBar* menuBar = pParent->findChild<QMenuBar*>(RDO_MENUBAR_NAME);
		ASSERT(menuBar);
		for (auto action: menuBar->actions())
		{
			if (action->text() == PLUGIN_MENU_TEXT)
			{
				pluginMenu = action->menu();
				break;
			}
		}
	}
	return pluginMenu;
}

void PluginGame5::initToolBar(MainWindow* pParent) const
{
	QToolBar* pluginToolBar = new QToolBar(pParent);
	pluginToolBar->setObjectName(PLUGIN_TOOLBAR_NAME);

	QAction* graphDlgAction             = new QAction(pParent);
	QAction* generateSituationDlgAction = new QAction(pParent);

	generateSituationDlgAction->setText("Расставить фишки");
	QPixmap pluginGenerateDlgActionPixmap(":/res/images/gen_sit_dialog.png");
	generateSituationDlgAction->setIcon(QIcon(pluginGenerateDlgActionPixmap));

	graphDlgAction->setText("Построить граф");
	QPixmap graphDlgPixmapD(":/res/images/graph_dialog_d.png");
	QPixmap graphDlgPixmap (":/res/images/graph_dialog.png");
	QIcon graphDlgIcon(graphDlgPixmap);
	graphDlgIcon.addPixmap(graphDlgPixmapD, QIcon::Disabled);
	graphDlgAction->setIcon(graphDlgIcon);
	graphDlgAction->setEnabled(false);

	pluginToolBar->addAction(generateSituationDlgAction);
	pluginToolBar->addAction(graphDlgAction);
	pluginToolBar->setIconSize(QSize(16, 15));

	pParent->addToolBar(Qt::TopToolBarArea, pluginToolBar);

	connect(generateSituationDlgAction, &QAction::triggered,
	        m_generateSituationDlg    , &PluginGame5GenerateSituationDialog::onPluginAction);
	connect(graphDlgAction, &QAction::triggered,
	        this          , &PluginGame5::reemitGraphDlgAction);

	connect(this      , &PluginGame5::onGraphDlgAction,
	        m_graphDlg, &PluginGame5GraphDialog::onPluginAction);
	connect(this          , &PluginGame5::setGraphDlgActionEnabled,
	        graphDlgAction, &QAction::setEnabled);
}

void PluginGame5::initDialogs(QWidget* pParent)
{
	m_generateSituationDlg = new PluginGame5GenerateSituationDialog(pParent);
	m_graphDlg  = new PluginGame5GraphDialog(pParent);
	
	connect(m_generateSituationDlg, &QDialog::accepted,
	        this                  , &PluginGame5::reemitGraphDlgActionEnabled);
}

void PluginGame5::reemitGraphDlgAction()
{
	emit onGraphDlgAction(m_generateSituationDlg->getBoardState());
}

void PluginGame5::reemitGraphDlgActionEnabled()
{
	emit setGraphDlgActionEnabled(true);
}
