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
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/plugin_game5.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_dialog.h"
#include "app/rdo_studio/src/application.h"
#include "app/rdo_studio/src/tracer/tracer.h"
#include "app/rdo_studio/src/main_window.h"
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
	return "0.1.3";
}

QUuid PluginGame5::getGUID()
{
	QUuid plgnGUID("{9D9CAAE2-324A-43db-8A4C-E8B12FD6C546}");
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
		connect(action, SIGNAL(triggered()), this, SLOT(pluginSlot()));
		menu->addAction(action);
	}
}

void PluginGame5::plgnStopAction(QWidget* pParent)
{
	QMenu* createdMenu = pParent->findChild<QMenu*>("createdMenu");
	QAction* createdAction;
	if (createdMenu)
	{
		createdAction = createdMenu->findChild<QAction*>("createdAction");
		if (createdAction)
		{
			createdAction->disconnect();
			createdAction->~QAction();
		}
		if (createdMenu->isEmpty())
		{
			createdMenu->~QMenu();
		}
	}
	else
	{
		QMenuBar* menuBar = pParent->findChild<QMenuBar*>("menuBar");
		bool defined = false;
		QMenu* defMenu;
		BOOST_FOREACH(QAction* action, menuBar->actions())
		{
			if (action->text() == "Плагины" && !defined)
			{
				defined = true;
				defMenu = action->menu();
			}
			createdAction = defMenu->findChild<QAction*>("createdAction");
			if (createdAction)
			{
				createdAction->disconnect();
				createdAction->~QAction();
			}
		}
	}
}

void PluginGame5::pluginSlot()
{
	QWidget* pParent = qobject_cast<QWidget*>(sender()-> //action
	                                          parent()-> //QMenu
	                                          parent()); //QMainWindow
	PluginGame5GenerateSituationDialog D(pParent);
	D.exec();
}