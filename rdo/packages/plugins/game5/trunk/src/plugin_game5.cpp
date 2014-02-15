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
#include <boost/foreach.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/plugin_game5.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/tracer/tracer.h"
#include "app/rdo_studio/src/main_window.h"
#include "app/rdo_studio/src/model/model_tab_ctrl.h"
#include "utils/src/common/model_objects.h"
// --------------------------------------------------------------------------------

QString PluginGame5::getPluginName()
{
	return "game5_debug";
}

QString PluginGame5::getAuthor()
{
	return "Chernov";
}

QString PluginGame5::getVersion()
{
	return "0.2.0";
}

QUuid PluginGame5::getGUID()
{
	QUuid pluginGUID("{5315750C-964B-4ed2-96FE-21FC6226942B}");
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
	QMenu* menu = pParent->findChild<QMenu*>("createdMenu");
	if (!menu)
	{
		QMenuBar* menuBar = pParent->findChild<QMenuBar*>("menubar");
		if (menuBar)
		{
			QMenu* defMenu = NULL;
			BOOST_FOREACH(QAction* action, menuBar->actions())
			{
				if (action->text() == "Плагины")
				{
					defMenu = action->menu();
					break;
				}
			}
			if (defMenu)
			{
				menu = defMenu;
			}
			else
			{
				menu = new QMenu("Плагины", pParent);
				menu->setObjectName("createdMenu");
			}
			menuBar->addMenu(menu);
		}
		else
		{
			QMessageBox::warning(pParent, "Plugin " + getPluginName() + " ver " + getVersion(),
			                     "Не найдено меню");
		}
	}
	if (menu)
	{
		QAction* action = new QAction(getPluginName() + " ver " + getVersion(), menu);
		action->setObjectName("createdAction");
		connect(action, &QAction::triggered, this, &PluginGame5::pluginSlot);
		menu->addAction(action);
	}
	genSitDlg = NULL;
	graphDlg  = NULL;
}

void PluginGame5::pluginStopAction(QWidget* pParent)
{
	QMenu* pluginMenu = pParent->findChild<QMenu*>("createdMenu");
	QAction* pluignAction = NULL;
	if (pluginMenu)
	{
		pluignAction = pluginMenu->findChild<QAction*>("createdAction");
	}
	else
	{
		QMenuBar* menuBar = pParent->findChild<QMenuBar*>("menuBar");
		bool defined = false;
		BOOST_FOREACH(QAction* action, menuBar->actions())
		{
			if (!defined && action->text() == "Плагины")
			{
				defined = true;
				pluginMenu = action->menu();
				pluignAction = pluginMenu->findChild<QAction*>("createdAction");
			}
		}
	}
	if (pluignAction)
	{
		delete pluignAction;
	}
	if (pluginMenu->isEmpty())
	{
		delete pluginMenu;
	}
	if (genSitDlg)
	{
		delete genSitDlg;
	}
	if (graphDlg)
	{
		delete graphDlg;
	}
	QToolBar* pluginGame5ToolBar = pParent->findChild<QToolBar*>("pluginGame5ToolBar");
	if (pluginGame5ToolBar)
	{
		delete pluginGame5ToolBar;
	}
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
		if(pMainWindow->getModel()->isModify())
		{
			int ret = QMessageBox::question(g_pApp->getMainWnd(),
			                                "RAO-Studio",
			                                "Модель была изменена!\nХотите сохранить изменения?",
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
				int ret = QMessageBox::warning(g_pApp->getMainWnd(),
				                               "RAO-Studio",
				                               "Текущая модель будет перезаписана!",
				                               QMessageBox::Ok | QMessageBox::Cancel,
				                               QMessageBox::Ok
				);
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
			QAction* senderAct = qobject_cast<QAction*>(sender());
			senderAct->setEnabled(false);
		}
	}
	else
	{
		pMainWindow->actFileNew->trigger();
	}
}

void PluginGame5::backUpModel(rdo::gui::model::Model* pModel)
{
	QString modelFullName = pModel->getFullName();
	QString modelFolder   = modelFullName.section(QRegExp("\\\\|/"), 0, -2) + "/";
	QString backupFolder  = modelFolder + "backup" + QDateTime::currentDateTime().toString("_yyyy-MM-dd_HH.mm.ss") + "/";
	QDir makeDir;
	if (makeDir.mkpath(backupFolder))
	{
		for (int i = 0; i < 13; i++)
		{
			QString fileFormat = QString::fromStdString(getFileTypeString(rdoModelObjects::RDOFileType(i)));
			QString fileName     = modelFolder + pModel->getName() + ".";
			        fileName    += fileFormat.toLower();
			QString newFileName  = backupFolder + pModel->getName() + ".";
			        newFileName += fileFormat.toLower();
			if (QFile::exists(fileName))
			{
				QFile::copy(fileName, newFileName);
			}
		}
	}
}

void PluginGame5::initToolBar(MainWindow* pParent)
{
	QToolBar* pluginToolBar = new QToolBar(pParent);
	pluginToolBar->setObjectName("pluginGame5ToolBar");

	QAction* pluginGraphDlgAction    = new QAction(pParent);
	QAction* pluginGenerateDlgAction = new QAction(pParent);

	pluginGenerateDlgAction->setObjectName("pluginGenerateDlgAction");
	pluginGenerateDlgAction->setText("Расставить фишки");
	QPixmap pluginGenerateDlgActionPixmap(":/res/images/gen_sit_dialog.png");
	pluginGenerateDlgAction->setIcon(QIcon(pluginGenerateDlgActionPixmap));

	pluginGraphDlgAction->setObjectName("pluginGraphDlgAction");
	pluginGraphDlgAction->setText("Построить граф");
	QPixmap graphDlgPixmapD(":/res/images/graph_dialog_d.png");
	QPixmap graphDlgPixmap (":/res/images/graph_dialog.png");
	QIcon graphDlgIcon(graphDlgPixmap);
	graphDlgIcon.addPixmap(graphDlgPixmapD, QIcon::Disabled);
	pluginGraphDlgAction->setIcon(graphDlgIcon);
	pluginGraphDlgAction->setEnabled(false);

	pluginToolBar->addAction(pluginGenerateDlgAction);
	pluginToolBar->addAction(pluginGraphDlgAction);
	pluginToolBar->setIconSize(QSize(16, 15));

	pParent->addToolBar(Qt::TopToolBarArea,pluginToolBar);

	connect(pluginGenerateDlgAction , &QAction::triggered, genSitDlg, &PluginGame5GenerateSituationDialog::onPluginAction);
	connect(pluginGraphDlgAction    , &QAction::triggered, this     , &PluginGame5::reemitGraphDlgAction);

	connect(this, &PluginGame5::onGraphDlgAction        , graphDlg            , &PluginGame5GraphDialog::onPluginAction);
	connect(this, &PluginGame5::setGraphDlgActionEnabled, pluginGraphDlgAction, &QAction::setEnabled);
}

void PluginGame5::initDialogs(QWidget* pParent)
{
	genSitDlg = new PluginGame5GenerateSituationDialog(pParent);
	graphDlg  = new PluginGame5GraphDialog(pParent);
	
	connect(genSitDlg, &QDialog::accepted, this, &PluginGame5::reemitGraphDlgActionEnabled);
}

void PluginGame5::reemitGraphDlgAction()
{
	emit onGraphDlgAction(genSitDlg->getBoardState());
}

void PluginGame5::reemitGraphDlgActionEnabled()
{
	emit setGraphDlgActionEnabled(true);
}