/*!
  \copyright (c) RDO-Team, 2012
  \file      main.cpp
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      05.11.2012
  \brief     Тест создания структры базы данных
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <conio.h>
#include <QtSql\QtSql>
#include <QtCore\QCoreApplication>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator\compiler\parser\src\db\init_struct_db.h"
// --------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	InitSructDB db;

	std::cout << "to be continued...\n";
	getch();
	return 0;
}
