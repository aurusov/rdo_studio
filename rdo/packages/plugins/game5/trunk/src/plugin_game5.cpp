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
#include <QtGui>
#include <QMenuBar>
#include <QMessageBox>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/plugin_game5.h"
#include "app/rdo_studio/plugins/game5/src/plugin_game5_dialog.h"
#include "app/rdo_studio/src/application.h"
// --------------------------------------------------------------------------------

QString plugin::getPluginName()
{
	return "game5_debug";
}

QString plugin::getAuthor()
{
	return "Chernov";
}

QString plugin::getVersion()
{
	return "0.1.3";
}

QUuid plugin::getGUID()
{
	QUuid plgnGUID("{9D9CAAE2-324A-43db-8A4C-E8B12FD6C546}");
	return plgnGUID;
}

void plugin::plgnStartAction(QWidget * p_Parent)
{
	if (!g_pApp)
	{
		g_pApp = static_cast<Application*>(qApp);
	}
	QMenu * menu = p_Parent->findChild<QMenu *>("createdMenu");
	if (!menu) {
		QMenuBar *menuBar = p_Parent->findChild<QMenuBar *>("menubar");
		if(menuBar) {
			bool defined = false;
			QMenu *defMenu;
			foreach(QAction *a, menuBar->actions()) {
				if (a->text() == "Плагины" && !defined) {
					defined = true;
					defMenu = a->menu();
				}
			}
			if (!defined) {
				menu = new QMenu("Плагины",p_Parent);
				menu->setObjectName("createdMenu");
			}
			else {
				menu = defMenu;
			}
			menuBar->addMenu(menu);
		}
		else {
			QMessageBox::warning(p_Parent, "Plugin " + getPluginName() + " ver " + getVersion(),
			                     "Не найдено меню");
		}
	}
	if(menu) {
		QAction *action = new QAction(getPluginName() + " ver " + getVersion(), menu);
		action->setObjectName("createdAction");
		connect(action, SIGNAL(triggered()), this, SLOT(pluginSlot()));
		menu->addAction(action);
	}
}

void plugin::plgnStopAction (QWidget * p_Parent)
{
	QMenu * createdMenu = p_Parent->findChild<QMenu *>("createdMenu");
	QAction * createdAction;
	if (createdMenu) {
		createdAction = createdMenu->findChild<QAction *>("createdAction");
		if(createdAction) {
			createdAction->disconnect();
			createdAction->~QAction();
		}
		if (createdMenu->isEmpty()) {
			createdMenu->~QMenu();
		}
	}
	else {
		QMenuBar *menuBar = p_Parent->findChild<QMenuBar *>("menuBar");
		bool defined = false;
		QMenu *defMenu;
		foreach(QAction *a, menuBar->actions()) {
			if (a->text() == "Плагины" && !defined) {
				defined = true;
				defMenu = a->menu();
			}
			createdAction = defMenu->findChild<QAction *>("createdAction");
			if(createdAction) {
				createdAction->disconnect();
				createdAction->~QAction();
			}
		}
	}
}

void plugin::pluginSlot ()
{
	QWidget * p_parent = qobject_cast<QWidget *>(sender()-> //action
	                                             parent()-> //QMenu
	                                             parent()); //QMainWindow ?
	game5Dialog D(p_parent);
	D.exec();
}