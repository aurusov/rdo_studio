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
#include <QMessageBox>
#include <QBitmap>
#include <QDir>
#include <boost/foreach.hpp>
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
	QUuid plgnGUID("{5315750C-964B-4ed2-96FE-21FC6226942B}");
	return plgnGUID;
}

void PluginGame5::plgnStartAction(QWidget* pParent)
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
			bool defined = false;
			QMenu* defMenu;
			BOOST_FOREACH(QAction* action, menuBar->actions())
			{
				if (action->text() == "Плагины" && !defined)
				{
					defined = true;
					defMenu = action->menu();
				}
			}
			if (!defined)
			{
				menu = new QMenu("Плагины", pParent);
				menu->setObjectName("createdMenu");
			}
			else
			{
				menu = defMenu;
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

void PluginGame5::plgnStopAction(QWidget* pParent)
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

	QAction* plgnGraphDlg = new QAction(pParent);
	QAction* plgnGnrtDlg = new QAction(pParent);

	plgnGnrtDlg->setObjectName("plgnGnrtDlg");
	plgnGnrtDlg->setText("Расставить фишки");
	QPixmap gnrtDlgPixmap(":/res/images/gen_sit_dialog.bmp");
	gnrtDlgPixmap.setMask(gnrtDlgPixmap.createMaskFromColor(QColor(255, 0, 255)));
	plgnGnrtDlg->setIcon(QIcon(gnrtDlgPixmap));

	plgnGraphDlg->setObjectName("plgnGraphDlg");
	plgnGraphDlg->setText("Построить граф");
	QPixmap graphDlgPixmapD(":/res/images/graph_dialog_d.bmp");
	QPixmap graphDlgPixmap (":/res/images/graph_dialog.bmp");
	graphDlgPixmapD.setMask(graphDlgPixmapD.createMaskFromColor(QColor(255, 0, 255)));
	graphDlgPixmap .setMask(graphDlgPixmap .createMaskFromColor(QColor(255, 0, 255)));
	QIcon graphDlgIcon(graphDlgPixmap);
	graphDlgIcon.addPixmap(graphDlgPixmapD, QIcon::Disabled);
	plgnGraphDlg->setIcon(graphDlgIcon);
	plgnGraphDlg->setEnabled(false);

	pluginToolBar->addAction(plgnGnrtDlg);
	pluginToolBar->addAction(plgnGraphDlg);
	pluginToolBar->setIconSize(QSize(16, 15));

	pParent->addToolBar(Qt::TopToolBarArea,pluginToolBar);

	connect(plgnGnrtDlg , &QAction::triggered, genSitDlg, &PluginGame5GenerateSituationDialog::onPlgnAction);
	connect(plgnGraphDlg, &QAction::triggered, this , &PluginGame5::reemitGraphDlgAction);

	connect(this, &PluginGame5::onGraphDlgAction        , graphDlg    , &PluginGame5GraphDialog::onPlgnAction);
	connect(this, &PluginGame5::setGraphDlgActionEnabled, plgnGraphDlg, &QAction::setEnabled);
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